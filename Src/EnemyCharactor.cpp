#include "../Res/Audio/SampleCueSheet.h"
#include "EnemyCharactor.h"
#include "GameEngine.h"
#include "GameState.h"

void EnemyCharactor::UpdateFnc(EnemyCharactor* n) {
	if (n->movecount == 0) {
		//�I��
		n->Velocity({ 0.0f,0.0f,0.0f });
		n->RotateVelocity({ 0.0f,0.0f,0.0f });
	}
	if (n->movecount != 0) {
		if (n->animType == 0) {
			//�_���[�W�A�j���[�V����
			if (n->movecount == 7) {
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
		}
		else if(n->animType == 1) {
			//�����A�j���[�V����
			if (n->movecount == 1) {
				n->ScaleVelocity({ 0.0f, 0.0f, 0.0f });
				n->ColorVelocity({ 0.0f, 0.0f, 0.0f, 0.0f });
				n->removeNode();
				
				if (n != nullptr) { delete n; };
				
				return;
			}
		}
		else if (n->animType == 2) {

			if (!(n->pos.x == n->bepos.x &&
				n->pos.y == n->bepos.y &&
				n->pos.z == n->bepos.z)) {
				//�ړ��A�j���[�V����
				if (n->movecount == 10) {

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
					/*
					if () {

					}
					else {
					n->Velocity({ 0.0f,0.0f, 0.0f });
					}
					*/


				}
				else if (n->movecount == 1) {
					//�I��
					//n->Velocity({ 0.0f,0.0f,0.0f });
					//n->RotateVelocity({ 0.0f,0.0f,0.0f });
					if (GameState::MainGame::AttackCharList.size()) {
						//���X�g������΍U���J�n
						if (GameState::MainGame::gamemode != 3) GameState::MainGame::gamemode = 3;
					}
					else {
						//���X�g��������΃v���C���[���͑҂�
						if (GameState::MainGame::gamemode != 0) GameState::MainGame::gamemode = 0;
					}
					
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
			else {
				if (n->movecount == 1) {
					//�I��
			
					if (GameState::MainGame::AttackCharList.size()) {
						//���X�g������΍U���J�n
						if (GameState::MainGame::gamemode != 3) GameState::MainGame::gamemode = 3;
					}
					else {
						//���X�g��������΃v���C���[���͑҂�
						if (GameState::MainGame::gamemode != 0) GameState::MainGame::gamemode = 0;
					}
				}
			}
			
			
		}
		else if (n->animType == 3) {
			//�U���A�j���[�V����
			if (n->movecount == 15) {
				//�r�U�艺�낵
				n->pBody->RotateVelocity({ 0.0f,-20.0f,0.0f });
				n->pRarmNode->RotateVelocity({ 30.0f,0.0f,0.0f });

				//�U����m�F
				for (int i = -1; i < 2; i++) {
					if (n->MapInJudge(n->vecx[n->vec], i, n->vecz[n->vec]) && LoadState::LoadGame::mapdata[n->pos.x + (n->vecx[n->vec])][n->pos.y + i][n->pos.z + (n->vecz[n->vec])].chara != nullptr) {
						Charactor* e = static_cast<Charactor*>(LoadState::LoadGame::mapdata[n->pos.x + (n->vecx[n->vec])][n->pos.y + i][n->pos.z + (n->vecz[n->vec])].chara);
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
				//���X�g�̐擪���폜
				GameState::MainGame::AttackCharList.erase(GameState::MainGame::AttackCharList.begin());
				if (GameState::MainGame::AttackCharList.size()) {
					//���̃L�����N�^�[�̍U�����X�^�[�g
					GameState::MainGame::AttackCharList[0]->Attack();
				}
				else {
					//���X�g����Ȃ̂Ńv���C���[���͑҂��ɕύX
					if (GameState::MainGame::gamemode != 0) GameState::MainGame::gamemode = 0;
				}
			}
		}
		n->movecount--;
	}
}

/**
*�L�����N�^�[�A�b�v�f�[�g�X�V
*/
void EnemyCharactor::Update() {
	if (updfnc != nullptr) {
		updfnc(this);
	}
	return;
}

/**
*�_���[�W�A�j���[�V�����X�^�[�g�ݒ�
*/
void EnemyCharactor::DamageAnimation() {
	pRarmNode->RotateVelocity({ 0.0f, 0.0f, 20.0f });
	pLarmNode->RotateVelocity({ 0.0f, 0.0f, -20.0f });
	RotateVelocity({ 5.0f, 0.0f, 0.0f });
	movecount = 13;
	animType = 0;
}

/**
*�폜�A�j���[�V�����X�^�[�g�ݒ�
*/
void EnemyCharactor::DeleteAnimation() {
	
	GameEngine & game = GameEngine::Instance();
	game.PlayAudio(GameState::AudioPlayerId::AudioPlayerId_Bomb, CRI_SAMPLECUESHEET_BOMB);
	ScaleVelocity({ 0.2f, 0.2f, 0.2f });
	ColorVelocity({ 0.0f,0.0f, 0.0f, -0.05f });
	movecount = 21;
	animType = 1;
}

/**
*�L�����N�^�[�s��
*/
void EnemyCharactor::Action() {
	GameEngine& game = GameEngine::Instance();
	Velocity({ 0.0f,0.0f,0.0f });
	RotateVelocity({ 0.0f,0.0f,0.0f });
	glm::vec3 vec1;
	float rot1;
	//�i�s�������X�g
	struct param {
		int vec = 0;///����
		int type = 0;///0���i�@1�オ��@2������
	};
	
	std::vector<param> vecList;
	int at = -1;
	for (int i = 0; i < 4; i++) {
		LoadState::MapData map1;
		bool map1flg = MapInJudge(vecx[i], 0, vecz[i]);
		if (map1flg) {
			map1 = LoadState::LoadGame::mapdata[pos.x + (vecx[i])][pos.y][pos.z + (vecz[i])];

		}
		LoadState::MapData map2;
		bool map2flg = MapInJudge(vecx[i], 1, vecz[i]);
		if (map2flg) {
			map2 = LoadState::LoadGame::mapdata[pos.x + (vecx[i])][pos.y + 1][pos.z + (vecz[i])];
		}
		LoadState::MapData map3;
		bool map3flg = MapInJudge(vecx[i], -1, vecz[i]);
		if (map3flg) {
			map3 = LoadState::LoadGame::mapdata[pos.x + (vecx[i])][pos.y - 1][pos.z + (vecz[i])];
		}
		
		//�v���C���[�אڊm�F
		if (map1flg && map1.chara == Char::Charactor::Player ||
			map2flg && map2.chara == Char::Charactor::Player ||
			map3flg && map3.chara == Char::Charactor::Player) {
			at = i;
		}

		//���i
		if (map1flg && map1.chara == nullptr && map1.tile && map1.stair != 2) {
			vecList.push_back({ i,0 });
		}
		else if (map2flg && map2.chara == nullptr && map2.tile && map2.stair != 2) {
			//�K�i�オ��
			vecList.push_back({ i,1 });
		}
		else if (map3flg && map3.chara == nullptr && map3.tile) {
			//�K�i������
			vecList.push_back({ i,2 });
		}
	}

	//��₪2�ȏ�Ȃ�^������₩��폜
	if (vecList.size() > 1) {
		for (auto itr = vecList.begin(); itr < vecList.end();itr++) {
			int v = vec - itr->vec;
			if (v == -2 || v == 2) {
				vecList.erase(itr);
				break;
			}
		}
	}

	

	//�A�j���[�V�����^�C�v����s�ɕύX
	animType = 2;
	movecount = 20;

	bepos.x = pos.x;
	bepos.y = pos.y;
	bepos.z = pos.z;

	//�v���C���[�ɗאڂ��Ă���Ȃ�U�����X�g�ɒǉ�
	if(at != -1)GameState::MainGame::AttackCharList.push_back(this);

	param v;
	if (vecList.size()) {
		const std::uniform_real_distribution<float> rnd(0.0f, 1.0f);
		v = vecList[(int)(rnd(game.Rand()) * vecList.size())];
	}
	
	int r = vec - (at == -1 ? v.vec : at);
	if (r == -2 || r == 2) {
		rot1 = 18.0f;
		RotateVelocity({ 0.0f,9.0f,0.0f });
	}
	else {
		if (r == 0) {
			RotateVelocity({ 0.0f,0.0f,0.0f });
		}
		else if (r == 1 || r == -3) {
			RotateVelocity({ 0.0f,-4.5f,0.0f });
		}
		else if (r == -1 || r == 3) {
			RotateVelocity({ 0.0f,4.5f,0.0f });
		}

	}
	vec = at == -1 ? v.vec : at;

	if (vecList.size() && at == -1) {
		//const std::uniform_real_distribution<float> rnd(0.0f, 1.0f);
		//param v = vecList[(int)(rnd(game.Rand()) * vecList.size())];

		LoadState::LoadGame::mapdata[pos.x][pos.y][pos.z].chara = nullptr;

		pos.x += vecx[v.vec];
		pos.z += vecz[v.vec];

		vec1.x += vecx[v.vec] * 0.1f;
		vec1.z += vecz[v.vec] * 0.1f;
		if (v.type == 1) {
			pos.y += 1;
			vec1.y = 0.1f;
		}
		else if (v.type == 2) {
			pos.y -= 1;
			vec1.y = 0.0f;
		}

		/*
		int r = vec - v.vec;
		if (r == -2 || r == 2) {
			rot1 = 18.0f;
			RotateVelocity({0.0f,9.0f,0.0f});
		}
		else {
			if (r == 0) {
				RotateVelocity({ 0.0f,0.0f,0.0f });
			}
			else if (r == 1 || r == -3) {
				RotateVelocity({ 0.0f,-4.5f,0.0f });
			}
			else if (r == -1 || r == 3) {
				RotateVelocity({ 0.0f,4.5f,0.0f });
			}
			
		}
		vec = v.vec;
		*/
		

		LoadState::LoadGame::mapdata[pos.x][pos.y][pos.z].chara = this;

		if (LoadState::LoadGame::mapdata[bepos.x][bepos.y][bepos.z].stair == 1) {
			//�ړ��O���K�i
			if (pos.y > bepos.y) {
				vec1.y = 0.1f;
			}
			else {
				vec1.y = -0.1f;
			}
		}
	}

	Velocity(vec1);

}

/*
*�L�����N�^�[�U��
*/
void EnemyCharactor::Attack() {
	GameEngine& game = GameEngine::Instance();
	//�U��������Ȃ�A�j���[�V�����X�^�[�g
	movecount = 21;
	animType = 3;
	Velocity({ 0.0f,0.0f,0.0f });
	RotateVelocity({ 0.0f,0.0f,0.0f });
	pBody->RotateVelocity({ 0.0f,10.0f,0.0f });
	pRarmNode->RotateVelocity({ -30.0f,0.0f,0.0f });

	game.PlayAudio(GameState::AudioPlayerId::AudioPlayerId_Shot, CRI_SAMPLECUESHEET_PLAYERSHOT);
}