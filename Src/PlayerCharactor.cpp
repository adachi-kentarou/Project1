#include "Constants.h"
#include "../Res/Audio/SampleCueSheet.h"
#include "Charactor.h"
#include "PlayerCharactor.h"
#include "EnemyCharactor.h"
#include "GameEngine.h"
#include "GameState.h"

/**
*�f�X�g���N�^
*/
PlayerCharactor::~PlayerCharactor() {
	Char::Charactor::Player = nullptr;
};

void PlayerCharactor::UpdateFnc(PlayerCharactor* n) {

	GameEngine& game = GameEngine::Instance();
	const GamePad gamepad = game.GetGamePad();
	if (n->movecount == 0) {
		//�I��
		n->Velocity({ 0.0f,0.0f,0.0f });
		n->RotateVelocity({ 0.0f,0.0f,0.0f });

	}
	if (GameState::MainGame::gamemode == 0 && n->movecount == 0) {
		
		
		//�J�E���g��0�Ȃ�΃L�[����
		//�v���C���[����
		
		glm::vec3 vec;
		glm::vec3 rot;
		if (gamepad.mouseClick) {
			//�Q�[�����[�h���U���ɕύX
			//GameState::MainGame::gamemode = 2;
			//GameState::MainGame::AttackCharList.push_back(static_cast<Char::Charactor*>(n));
			n->Attack();
			
			goto ANIMATION;
		}
		else if (gamepad.buttons & GamePad::DPAD_LEFT) {
			//vec.x = 0.1f;
			rot.y = 4.5f;
			if (n->vec == 3) {
				n->vec = 0;
			}
			else {
				n->vec++;
			}
		}
		else if (gamepad.buttons & GamePad::DPAD_RIGHT) {
			//vec.x = -0.1f;
			rot.y = -4.5f;
			if (n->vec == 0) {
				n->vec = 3;
			}
			else {
				n->vec--;
			}
		} 
		else if ((gamepad.buttons & GamePad::DPAD_UP) || (gamepad.buttons & GamePad::DPAD_DOWN)) {
			//������⃊�X�g�@0=���Ȃ� 1=���i�@2=�オ�� 3=������
			int direction[3] = { 0,0,0 };
			int updown;
			if (gamepad.buttons & GamePad::DPAD_UP) {
				updown = 1;
			}
			else {
				updown = -1;
			}
			for (int i = 0; i < 3; i++) {
				int v = n->vec + i - 1;
				if (v > 3)v = 0;
				if (v < 0)v = 3;

				LoadState::MapData map1;
				bool map1flg = n->MapInJudge(n->vecx[v] * updown, 0, n->vecz[v] * updown);
				if (map1flg) {
					map1 = LoadState::LoadGame::mapdata[n->pos.x + (n->vecx[v] * updown)][n->pos.y][n->pos.z + (n->vecz[v] * updown)];

				}
				LoadState::MapData map2;
				bool map2flg = n->MapInJudge(n->vecx[v] * updown, 1, n->vecz[v] * updown);
				if (map2flg) {
					map2 = LoadState::LoadGame::mapdata[n->pos.x + (n->vecx[v] * updown)][n->pos.y + 1][n->pos.z + (n->vecz[v] * updown)];
				}
				LoadState::MapData map3;
				bool map3flg = n->MapInJudge(n->vecx[v] * updown, -1, n->vecz[v] * updown);
				if (map3flg) {
					map3 = LoadState::LoadGame::mapdata[n->pos.x + (n->vecx[v] * updown)][n->pos.y - 1][n->pos.z + (n->vecz[v] * updown)];
				}
				//���i
				if (map1flg && map1.chara == nullptr && map1.tile && map1.stair != 2) {
					direction[i] = 1;

				}
				else if (map2flg && map2.chara == nullptr && map2.tile && map2.stair != 2) {
					//�K�i�オ��
					direction[i] = 2;

				}
				else if (map3flg && map3.chara == nullptr && map3.tile) {
					//�K�i������
					direction[i] = 3;

				}

			}

			int dir = 0;
			
			if (direction[1]) {
				//���i
				dir = direction[1];
			}
			else if (direction[2] && !direction[0]) {
				//�E��]
				dir = direction[2];
				rot.y = 4.5f;
				n->vec += 1;
				if (n->vec == 4) { n->vec = 0; }
			}
			else if (direction[0] && !direction[2]) {
				//����]
				dir = direction[0];
				rot.y = -4.5f;
				n->vec -= 1;
				if (n->vec == -1) { n->vec = 3; }
			}
			//���i
			if (dir == 1) {
				n->bepos.x = n->pos.x;
				n->bepos.y = n->pos.y;
				n->bepos.z = n->pos.z;

				LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].chara = nullptr;

				n->pos.x += n->vecx[n->vec] * updown;
				n->pos.z += n->vecz[n->vec] * updown;

				LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].chara = n;

				vec.z = n->vecz[n->vec] * 0.1f * updown;
				vec.x = n->vecx[n->vec] * 0.1f * updown;

			}
			else if (dir == 2) {
				//�K�i�オ��
				n->bepos.x = n->pos.x;
				n->bepos.y = n->pos.y;
				n->bepos.z = n->pos.z;

				LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].chara = nullptr;

				n->pos.x += n->vecx[n->vec] * updown;
				n->pos.y += 1;
				n->pos.z += n->vecz[n->vec] * updown;

				LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].chara = n;

				vec.z = n->vecz[n->vec] * 0.1f * updown;
				vec.y = 0.1f;
				vec.x = n->vecx[n->vec] * 0.1f * updown;

			}
			else if (dir == 3) {
				//�K�i������
				n->bepos.x = n->pos.x;
				n->bepos.y = n->pos.y;
				n->bepos.z = n->pos.z;

				LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].chara = nullptr;

				n->pos.x += n->vecx[n->vec] * updown;
				n->pos.y -= 1;
				n->pos.z += n->vecz[n->vec] * updown;

				LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].chara = n;

				vec.z = n->vecz[n->vec] * 0.1f * updown;
				vec.y = -0.0f;
				vec.x = n->vecx[n->vec] * 0.1f * updown;

			}

		}
		if (gamepad.mouseClick) {
		
		}
		else if (vec.x || vec.z || rot.y) {
			//�ړ���]������Ȃ�A�j���[�V�����X�^�[�g
			n->movecount = 20;
			n->animType = 1;
		}
		if (vec.x || vec.z) {
			n->EnemyAction();
			if (LoadState::LoadGame::mapdata[n->bepos.x][n->bepos.y][n->bepos.z].stair == 1) {
				//�ړ��O���K�i
				if (n->pos.y > n->bepos.y) {
					vec.y = 0.1f;
				}
				else {
					vec.y = -0.1f;
				}
			}
		}
		n->Velocity(vec);
		n->RotateVelocity(rot);
	}
ANIMATION:;
	if(n->movecount != 0) {
		if (n->animType == 0) {
			//�U���A�j���[�V����
			if (n->movecount == 15) {
				//�r�U�艺�낵
				n->pBody->RotateVelocity({ 0.0f,-20.0f,0.0f });
				n->pRarmNode->RotateVelocity({ 30.0f,0.0f,0.0f });

				//�U����m�F
				for (int i = -1; i < 2;i++) {
					if (n->MapInJudge(n->vecx[n->vec], i, n->vecz[n->vec]) && LoadState::LoadGame::mapdata[n->pos.x + (n->vecx[n->vec])][n->pos.y + i][n->pos.z + (n->vecz[n->vec])].chara != nullptr) {
						EnemyCharactor* e = static_cast<EnemyCharactor*>(LoadState::LoadGame::mapdata[n->pos.x + (n->vecx[n->vec])][n->pos.y + i][n->pos.z + (n->vecz[n->vec])].chara);
						if (e->Hp()) {
							//HP������΍U��
							e->DamageAnimation();
						}
					}
				}
				

			}
			else if (n->movecount == 9) {
				//�r�߂�
				n->pBody->RotateVelocity({ 0.0f,10.0f,0.0f });
				n->pRarmNode->RotateVelocity({ 0.0f,0.0f,0.0f });
			}
			else if (n->movecount == 3) {
				//�I��
				n->pBody->RotateVelocity({ 0.0f,0.0f,0.0f });
				//n->EnemyAction();
			}
			else if (n->movecount == 1) {
				if (GameState::MainGame::gamemode != 0) GameState::MainGame::gamemode = 0;
				n->EnemyAction();
			}
		}
		else if(n->animType == 1){
			//�ړ��A�j���[�V����
			if (n->movecount == 10 && (n->Velocity().x || n->Velocity().y || n->Velocity().z)) {
				glm::vec3 vec = n->Velocity();

				if (LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].stair == 1) {
					//�ړ��悪�K�i�̏ꍇ
					if (n->pos.y >= n->bepos.y) {
						//�㏸
						vec.y = 0.1f;
					}
					else {
						//���~
						vec.y = -0.1f;
					}

				}
				else {
					//�ړ��悪���ʂ̏ꍇ
					vec.y = 0.0f;
				}
				n->Velocity(vec);
			}
			else if (n->movecount == 1) {
				//�A�C�e���m�F
				if (LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].item != nullptr) {
					printf("�A�C�e���Q�b�g\n");
					LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].item->DeleteAnimation();
					LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].item->GetItem(n);
					LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].item = nullptr;


				}
				
			}
			else if (n->movecount == 0) {
				//�I��
				n->Velocity({ 0.0f,0.0f,0.0f });
				n->RotateVelocity({ 0.0f,0.0f,0.0f });

			}
			if (n->movecount > 0) {

				//�J�E���g��0�łȂ���Έړ��A�j���[�V����

				//�A�j���[�V�����J�E���^�[
				n->animationcount += 9;
				if (n->animationcount > 2147483647) {
					n->animationcount = 0;
				}

				double rad = std::sin(glm::radians((float)(n->animationcount % 360)));
				n->pRlegNode->Rotation({ rad * 60.0f, 0.0f, 0.0f });
				n->pLlegNode->Rotation({ -rad * 60.0f, 0.0f, 0.0f });
				n->pRarmNode->Rotation({ -rad * 60.0f, 0.0f, 300.0f });
				n->pLarmNode->Rotation({ rad * 60.0f, 0.0f, 60.0f });
			}
			
		}
		else if (n->animType == 2) {
			//�_���[�W�A�j���[�V����
			if (n->movecount == 8) {
				//�r�U�艺�낵
				n->pRarmNode->RotateVelocity({ 0.0f, 0.0f, -20.0f });
				n->pLarmNode->RotateVelocity({ 0.0f, 0.0f, 20.0f });
				n->RotateVelocity({ -5.0f, 0.0f, 0.0f });
			}
			else if (n->movecount == 1) {
				//�I��
				n->pRarmNode->RotateVelocity({ 0.0f, 0.0f, 0.0f });
				n->pLarmNode->RotateVelocity({ 0.0f, 0.0f, 0.0f });
				n->RotateVelocity({ 0.0f, 0.0f, 0.0f });

				//�L�����N�^�[�폜�m�F
				n->Hp(n->Hp() - 1);
				if (n->Hp() <= 0) {
					LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].chara = nullptr;

					n->DeleteAnimation();
				}
			}
			else if (n->animType == 3) {
				//�Q�[���I�[�o�[�A�j���[�V����
				if (n->movecount == 1) {
					//�I��
					n->Velocity({ 0.0f,0.0f,0.0f });
					n->RotateVelocity({ 0.0f,0.0f, 0.0f });
				}
			}
		}
		n->movecount--;
		//printf("%f\n", n->Position().x);
	}

	glm::vec3 pos = n->Position();

	//�J�������W�ύX
	GameEngine::CameraData camera = game.Camera();
	camera.position = pos + glm::vec3(0.0f, 0.0f, 0.0f);
	game.Camera(camera);

}

/**
*�L�����N�^�[�A�b�v�f�[�g�X�V
*/
void PlayerCharactor::Update() {
	if (updfnc != nullptr) {
		updfnc(this);
	}
	return;
}

/*
*�L�����N�^�[�U��
*/
void PlayerCharactor::Attack() {
	GameEngine& game = GameEngine::Instance();
	GameState::MainGame::gamemode = 2;
	//�U��������Ȃ�A�j���[�V�����X�^�[�g
	movecount = 21;
	animType = 0;
	Velocity({ 0.0f,0.0f,0.0f });
	RotateVelocity({ 0.0f,0.0f,0.0f });
	pBody->RotateVelocity({ 0.0f,10.0f,0.0f });
	pRarmNode->RotateVelocity({ -30.0f,0.0f,0.0f });

	game.PlayAudio(GameState::AudioPlayerId::AudioPlayerId_Shot, CRI_SAMPLECUESHEET_PLAYERSHOT);
}

/*
*�L�����N�^�[�s������
*/
void PlayerCharactor::EnemyAction() {
	GameState::MainGame::gamemode = 1;
	for (int itr = 0; Char::Charactor::CharList.begin() + itr < Char::Charactor::CharList.end(); itr++) {
		
		auto itrr = Char::Charactor::CharList.begin() + itr;
		//�v���C���[�L�����Ȃ��΂�
		if (*itrr == this) continue;

		if (static_cast<Char::Charactor*>(*itrr)->Hp()) {
			static_cast<EnemyCharactor*>(*itrr)->Action();
		}
	}
}

/**
*�_���[�W�A�j���[�V�����X�^�[�g�ݒ�
*/
void PlayerCharactor::DamageAnimation() {
	pRarmNode->RotateVelocity({ 0.0f, 0.0f, 20.0f });
	pLarmNode->RotateVelocity({ 0.0f, 0.0f, -20.0f });
	RotateVelocity({ 5.0f, 0.0f, 0.0f });
	movecount = 14;
	animType = 2;
}

/**
*�폜�A�j���[�V�����X�^�[�g�ݒ�
*/
void PlayerCharactor::DeleteAnimation() {
	GameEngine & game = GameEngine::Instance();
	game.PlayAudio(GameState::AudioPlayerId::AudioPlayerId_Bomb, CRI_SAMPLECUESHEET_BOMB);
	Velocity({ 0.0f,-0.15f,0.0f });
	RotateVelocity({ 10.0f,0.0f, 0.0f });
	movecount = 9;
	animType = 3;
}