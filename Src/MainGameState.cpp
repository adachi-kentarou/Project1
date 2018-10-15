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
	//<- ここにMain.cppから以下の変数、構造体、関数をカット&ペーストしてください.
	//<-   collisionDataList変数
	/// 衝突形状リスト.
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

	//<-   UpdateToroid構造体
	/**
	* 敵の円盤の状態を更新する.
	*/
	struct UpdateToroid
	{
		void operator()(Entity::Entity& entity, double delta)
		{
			// 範囲外に出たら削除する.
			const glm::vec3 pos = entity.Position();
			if (std::abs(pos.x) > 40.0f || std::abs(pos.z) > 40.0f) {
				GameEngine::Instance().RemoveEntity(&entity);
				return;
			}
		}

	};

	//<-   UpdatePlayerShot構造体
	/**
	* 自機の弾の更新.
	*/
	struct UpdatePlayerShot
	{
		void operator()(Entity::Entity& entity, double delta)
		{
			// 範囲外に出たら削除する.
			const glm::vec3 pos = entity.Position();
			if (std::abs(pos.x) > 40 || pos.z < -4 || pos.z > 40) {
				entity.Destroy();
				return;
			}
		}
	};

	//<-   UpdateBlast構造体
	/**
	* 爆発の更新.
	*/
	struct UpdateBlast
	{
		void operator()(Entity::Entity& entity, double delta) {
			timer += delta;
			if (timer >= 0.5) {
				entity.Destroy();
				return;
			}
			const float variation = static_cast<float>(timer * 4); // 変化量.
			entity.Scale(glm::vec3(static_cast<float>(1 + variation))); // 徐々に拡大する.
																		// 時間経過で色と透明度を変化させる.
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
	* 自機の弾と敵の衝突処理.
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
	* メインゲーム画面のコンストラクタ.
	*/
	//MainGame::MainGame(Entity::Entity* p) : pSpaceSphere(p)
	MainGame::MainGame()
	{
		GameEngine& game = GameEngine::Instance();

	}

	/**
	* メインゲーム画面の更新.
	*/
	void MainGame::operator()(double delta)
	{
		//<- ここにMain.cppのUpdate::operator()の内容をカット&ペーストしてください. 
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

			//プレイヤー配置
			if (player != nullptr) { delete player; }
			player = game.CreatePlayerChara();
			p = player->GetMapPosition();
			player->SetMapPosition(p.x, p.y, p.z);
			GameEngine::world->addNode(player);

			//敵配置
			for (int i = 0; i < STAGE_ENEMY + STAGE_ENEMY_UP * GameState::MainGame::stageno;i++) {
				c = game.CreateEnemyChara();
				p = c->GetMapPosition();
				c->SetMapPosition(p.x, p.y, p.z);
				GameEngine::world->addNode(c);
			}

			//アイテム配置
			for (int i = 0; i < STAGE_ITEM + STAGE_ITEM_UP * GameState::MainGame::stageno; i++) {
				const std::uniform_real_distribution<float> rnd(0.0f, 1.0f);

				if (rnd(game.Rand()) < 0.5f) {
					//回復アイテム
					it = game.CreateLifeItemChara();
				}
				else {
					//時間延長アイテム
					it = game.CreateTimeItemChara();
				}
				
				
				p = it->GetMapPosition();
				it->SetMapPosition(p.x, p.y, p.z);
				GameEngine::world->addNode(it);
			}
			
			interval = 1;
		}

		char str[16];

		//視点座標
		glm::vec3 pos = game.Camera().position;

		//計算
		double window_half_width = WINDOW_WIDTH / 2;
		double window_half_height = WINDOW_HEIGHT / 2;

		double posX = gamepad.posX - window_half_width;
		double posY = gamepad.posY - window_half_height;
		
		//角度補正値
		double hosei = 0;
		
		//X軸角度計算
		double radian = glm::radians(-LOOK_RADIAN * (posY / window_half_height) + hosei);
		//X軸ターゲットY座標
		double XtargetY = std::sin(radian) * 8;
		//X軸ターゲットX座標
		double XtargetX = std::cos(radian) * 8;

		//Y軸角度計算
		radian = (-LOOK_RADIAN * (posX / window_half_width) + player->Rotation().y) * (std::_Pi / 180);
		//Y軸ターゲットY座標
		double YtargetY = std::sin(radian) * XtargetX;
		//Y軸ターゲットX座標
		double YtargetX = std::cos(radian) * XtargetX;
		
		//カメラアングル変更
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
		
		//ステージ表示
		snprintf(str, 16, "STAGE:%d", GameState::MainGame::stageno);
		game.FontScale(glm::vec2(1.0f, 1.0f));
		game.AddString(glm::vec2(600.0f, 8.0f), str);


		//HP表示
		if (Char::Charactor::Player->Hp() < 6) {
			game.FontColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		snprintf(str, 16, "HP:%d", Char::Charactor::Player->Hp());
		game.FontScale(glm::vec2(1.0f, 1.0f));
		game.AddString(glm::vec2(400.0f, 8.0f), str);
		game.FontColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		//ゲーム終了条件

		//時間加算
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

		//ゲームスタート
		if (start) {
			game.FontColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
			game.FontScale(glm::vec2(3.0f, 3.0f));
			game.AddString(glm::vec2(130.0f, 260.0f), "GAME START");
			start--;
		}


		//ゲームオーバー
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

		//タイムアップ
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

		//ゲームクリア
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