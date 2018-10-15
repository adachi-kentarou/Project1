/**
* @file MainGameState.cpp
*/
#include "GameState.h"
#include "GameEngine.h"
#include "../Res/Audio/SampleCueSheet.h"
#include <algorithm>
#include <GLFW/glfw3.h>
#include "Constants.h"
#include "Charactor.h"
#include "LoadState.h"

namespace GameState {
	//<- ������Main.cpp����ȉ��̕ϐ��A�\���́A�֐����J�b�g&�y�[�X�g���Ă�������.
	//<-   collisionDataList�ϐ�
	/// �Փˌ`�󃊃X�g.
	static const Entity::CollisionData collisionDataList[] = {
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, -1.0f), glm::vec3(0.5f, 0.5f, 1.0f) },
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f) },
		{ glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3(0.25f, 0.25f, 0.25f) },
	};

	std::vector<Char::Charactor*> MainGame::MoveCharList;
	std::vector<Char::Charactor*> MainGame::AttackCharList;
	int MainGame::time = 0;
	int MainGame::timev = -1;
	int MainGame::timecount = STAGE_TIME;
	int MainGame::sec = 0;
	int MainGame::gamemode = 0;
	int MainGame::stageno = 1;

	//<-   UpdateToroid�\����
	/**
	* �G�̉~�Ղ̏�Ԃ��X�V����.
	*/
	struct UpdateToroid
	{
		void operator()(Entity::Entity& entity, double delta)
		{
			// �͈͊O�ɏo����폜����.
			const glm::vec3 pos = entity.Position();
			if (std::abs(pos.x) > 40.0f || std::abs(pos.z) > 40.0f) {
				GameEngine::Instance().RemoveEntity(&entity);
				return;
			}
		}

	};

	//<-   UpdatePlayerShot�\����
	/**
	* ���@�̒e�̍X�V.
	*/
	struct UpdatePlayerShot
	{
		void operator()(Entity::Entity& entity, double delta)
		{
			// �͈͊O�ɏo����폜����.
			const glm::vec3 pos = entity.Position();
			if (std::abs(pos.x) > 40 || pos.z < -4 || pos.z > 40) {
				entity.Destroy();
				return;
			}
		}
	};

	//<-   UpdateBlast�\����
	/**
	* �����̍X�V.
	*/
	struct UpdateBlast
	{
		void operator()(Entity::Entity& entity, double delta) {
			timer += delta;
			if (timer >= 0.5) {
				entity.Destroy();
				return;
			}
			const float variation = static_cast<float>(timer * 4); // �ω���.
			entity.Scale(glm::vec3(static_cast<float>(1 + variation))); // ���X�Ɋg�傷��.
																		// ���Ԍo�߂ŐF�Ɠ����x��ω�������.
			static const glm::vec4 color[] = {
				glm::vec4(1.0f, 1.0f, 0.75f, 1),
				glm::vec4(1.0f, 0.5f, 0.1f, 1),
				glm::vec4(0.25f, 0.1f, 0.1f, 0),
			};
			const glm::vec4 col0 = color[static_cast<int>(variation)];
			const glm::vec4 col1 = color[static_cast<int>(variation) + 1];
			const glm::vec4 newColor = glm::mix(col0, col1, std::fmod(variation, 1));
			entity.Color(newColor);
		}

		double timer = 0;
	};

	/**
	* ���@�̒e�ƓG�̏Փˏ���.
	*/
	void PlayerShotAndEnemyCollisionHandler(Entity::Entity& lhs, Entity::Entity& rhs)
	{
		GameEngine& game = GameEngine::Instance();
		if (Entity::Entity* p = game.AddEntity(EntityGroupId_Cube, rhs.Position(),
			"Blast", "Res/Toroid.bmp", UpdateBlast())) {
			const std::uniform_real_distribution<float> rotRange(0.0f, glm::pi<float>() * 2);
			
			game.Variable("score") += 100;
		}

		game.PlayAudio(AudioPlayerId_Bomb, CRI_SAMPLECUESHEET_BOMB);

		lhs.Destroy();
		rhs.Destroy();
	}

	/**
	* ���C���Q�[����ʂ̃R���X�g���N�^.
	*/
	//MainGame::MainGame(Entity::Entity* p) : pSpaceSphere(p)
	MainGame::MainGame()
	{
		GameEngine& game = GameEngine::Instance();

	}

	/**
	* ���C���Q�[����ʂ̍X�V.
	*/
	void MainGame::operator()(double delta)
	{
		//<- ������Main.cpp��Update::operator()�̓��e���J�b�g&�y�[�X�g���Ă�������. 
		GameEngine& game = GameEngine::Instance();
		const GamePad gamepad = game.GetGamePad();

		if (!isInitialized) {
			isInitialized = true; 

			char str[16];
			snprintf(str, 16, "STAGE %d", stageno);
			game.FontColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			game.FontScale(glm::vec2(3.0f, 3.0f));
			game.AddString(glm::vec2(260.0f, 130.0f), str);
			

			game.AmbientLight(glm::vec4(0.4f, 0.4f, 0.4f, 1));
			game.Light(0, { { 40, 100, 10, 1 },{ 12000, 12000, 12000, 1 } });

			game.PlayAudio(AudioPlayerId_BGM, CRI_SAMPLECUESHEET_BGM02);
		}
		
		if (interval <= 0) {
			

			Char::Charactor* c;
			Char::MapPoint p;
			Char::Item* it;

			//�v���C���[�z�u
			if (player != nullptr) { delete player; }
			player = game.CreatePlayerChara();
			p = player->GetMapPosition();
			player->SetMapPosition(p.x, p.y, p.z);
			GameEngine::world->addNode(player);

			//�G�z�u
			for (int i = 0; i < STAGE_ENEMY + STAGE_ENEMY_UP * GameState::MainGame::stageno;i++) {
				c = game.CreateEnemyChara();
				p = c->GetMapPosition();
				c->SetMapPosition(p.x, p.y, p.z);
				GameEngine::world->addNode(c);
			}

			//�A�C�e���z�u
			for (int i = 0; i < STAGE_ITEM + STAGE_ITEM_UP * GameState::MainGame::stageno; i++) {
				const std::uniform_real_distribution<float> rnd(0.0f, 1.0f);

				if (rnd(game.Rand()) < 0.5f) {
					//�񕜃A�C�e��
					it = game.CreateLifeItemChara();
				}
				else {
					//���ԉ����A�C�e��
					it = game.CreateTimeItemChara();
				}
				
				
				p = it->GetMapPosition();
				it->SetMapPosition(p.x, p.y, p.z);
				GameEngine::world->addNode(it);
			}
			
			interval = 1;
		}

		char str[16];

		//���_���W
		glm::vec3 pos = game.Camera().position;

		//�v�Z
		double window_half_width = WINDOW_WIDTH / 2;
		double window_half_height = WINDOW_HEIGHT / 2;

		double posX = gamepad.posX - window_half_width;
		double posY = gamepad.posY - window_half_height;
		
		//�p�x�␳�l
		double hosei = 0;
		
		//X���p�x�v�Z
		double radian = glm::radians(-LOOK_RADIAN * (posY / window_half_height) + hosei);
		//X���^�[�Q�b�gY���W
		double XtargetY = std::sin(radian) * 8;
		//X���^�[�Q�b�gX���W
		double XtargetX = std::cos(radian) * 8;

		//Y���p�x�v�Z
		radian = (-LOOK_RADIAN * (posX / window_half_width) + player->Rotation().y) * (std::_Pi / 180);
		//Y���^�[�Q�b�gY���W
		double YtargetY = std::sin(radian) * XtargetX;
		//Y���^�[�Q�b�gX���W
		double YtargetX = std::cos(radian) * XtargetX;
		
		//�J�����A���O���ύX
		if (gamepad.mouseWheel != 0) {
			if (gamepad.mouseWheel == 1 ) {
				if (MAX_CAMERA_ANGLE > game.zoom) {
					game.zoom += 2;
				}
			}
			else {
				if (MIN_CAMERA_ANGLE < game.zoom) {
					game.zoom -= 2;
				}
			}
		}
		

		game.Camera({ pos - glm::vec3(YtargetY,XtargetY,YtargetX),pos + glm::vec3(YtargetY,XtargetY,YtargetX),{ 0, 1, 0 } });
		
		game.FontColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		snprintf(str, 16, "mousex:%01.0f", gamepad.posX);
		game.FontScale(glm::vec2(0.5f, 0.5f));
		game.AddString(glm::vec2(10.0f, 10.0f), str);

		snprintf(str, 16, "mousey:%01.0f", gamepad.posY);
		game.AddString(glm::vec2(10.0f, 30.0f), str);

		snprintf(str, 16, "zoom:%01.0f", game.zoom);
		game.AddString(glm::vec2(10.0f, 50.0f), str);

		snprintf(str, 16, "pos:x%d y%d z%d", player->pos.x, player->pos.y, player->pos.z);
		game.AddString(glm::vec2(10.0f, 70.0f), str);
		
		snprintf(str, 16, "charactor:%d", Char::Charactor::CharList.size());
		game.AddString(glm::vec2(10.0f, 90.0f), str);
		/*
		snprintf(str, 16, "x:%f", Char::Charactor::Player->Position().x);
		game.AddString(glm::vec2(10.0f, 110.0f), str);
		snprintf(str, 16, "y:%f", Char::Charactor::Player->Position().y);
		game.AddString(glm::vec2(10.0f, 130.0f), str);
		snprintf(str, 16, "z:%f", Char::Charactor::Player->Position().z);
		game.AddString(glm::vec2(10.0f, 150.0f), str);
		*/
		
		//�X�e�[�W�\��
		snprintf(str, 16, "STAGE:%d", GameState::MainGame::stageno);
		game.FontScale(glm::vec2(1.0f, 1.0f));
		game.AddString(glm::vec2(600.0f, 8.0f), str);


		//HP�\��
		if (Char::Charactor::Player->Hp() < 6) {
			game.FontColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		snprintf(str, 16, "HP:%d", Char::Charactor::Player->Hp());
		game.FontScale(glm::vec2(1.0f, 1.0f));
		game.AddString(glm::vec2(400.0f, 8.0f), str);
		game.FontColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		//�Q�[���I������

		//���ԉ��Z
		if (++sec == 60) {
			timecount += timev;
			sec = 0;
		}
		if (timecount < 31) {
			game.FontColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		snprintf(str, 16, "TIME:%d", timecount);
		
		game.FontScale(glm::vec2(1.0f, 1.0f));
		game.AddString(glm::vec2(200.0f, 8.0f), str);
		game.FontColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		//�Q�[���X�^�[�g
		if (start) {
			game.FontColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
			game.FontScale(glm::vec2(3.0f, 3.0f));
			game.AddString(glm::vec2(130.0f, 260.0f), "GAME START");
			start--;
		}


		//�Q�[���I�[�o�[
		if (Char::Charactor::Player->Hp() == 0) {
			if (time == 0) {
				GameState::MainGame::gamemode = 4;
				time = 180;
				timev = 0;
				game.PlayAudio(AudioPlayerId_UI, CRI_SAMPLECUESHEET_START);
			}
			else {
				game.FontColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				game.FontScale(glm::vec2(3.0f, 3.0f));
				game.AddString(glm::vec2(130.0f, 260.0f), "GAME OVER");

				time--;
				if (time == 0) {
					game.StopAudio(GameState::AudioPlayerId::AudioPlayerId_BGM);
					game.reset = true;
					timecount = 100;
					timev = -1;
					sec = 0;
					AttackCharList.clear();
					game.UpdateFunc(GameState::Title());
				}
			}
			return;
		}

		//�^�C���A�b�v
		if (timecount == 0) {
			if (time == 0) {
				GameState::MainGame::gamemode = 4;
				time = 180;
				timev = 0;
				game.PlayAudio(AudioPlayerId_UI, CRI_SAMPLECUESHEET_START);
			}
			else {
				game.FontColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				game.FontScale(glm::vec2(3.0f, 3.0f));
				game.AddString(glm::vec2(130.0f, 260.0f), "TIME UP");
				
				time--;
				if (time == 0) {
					game.StopAudio(GameState::AudioPlayerId::AudioPlayerId_BGM);
					game.reset = true;
					timecount = 100;
					timev = -1;
					sec = 0;
					AttackCharList.clear();
					game.UpdateFunc(GameState::Title());
				}
			}
			return;
		}

		//�Q�[���N���A
		if (Char::Charactor::CharList.size() == 1 && timecount != 0) {
			if (time == 0) {
				GameState::MainGame::gamemode = 4;
				time = 180;
				timev = 0;
				game.PlayAudio(AudioPlayerId_UI, CRI_SAMPLECUESHEET_START);
			}
			else {
				game.FontColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
				game.FontScale(glm::vec2(3.0f, 3.0f));
				if (stageno < STAGE_NO) {
					game.FontColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
					game.AddString(glm::vec2(130.0f, 260.0f), "STAGE CLEAR");
				}
				else {
					game.FontColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
					game.AddString(glm::vec2(130.0f, 260.0f), "GAME CLEAR");
				}
				
				time--;
				if (time == 0) {
					game.StopAudio(GameState::AudioPlayerId::AudioPlayerId_BGM);
					game.reset = true;
					timecount = 100;
					sec = 0;
					AttackCharList.clear();
					
					if (stageno < STAGE_NO) {
						game.UpdateFunc(LoadState::LoadGame());
					}
					else {
						game.UpdateFunc(GameState::Title());
					}
					MainGame::stageno++;
				}
			}
			
			return;
		}
	}

}