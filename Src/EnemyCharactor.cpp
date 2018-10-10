#include "../Res/Audio/SampleCueSheet.h"
#include "EnemyCharactor.h"
#include "GameEngine.h"
#include "GameState.h"

void EnemyCharactor::UpdateFnc(EnemyCharactor* n) {
	if (n->movecount == 0) {
		//終了
		n->Velocity({ 0.0f,0.0f,0.0f });
		n->RotateVelocity({ 0.0f,0.0f,0.0f });
	}
	if (n->movecount != 0) {
		if (n->animType == 0) {
			//ダメージアニメーション
			if (n->movecount == 7) {
				//腕振り下ろし
				n->pRarmNode->RotateVelocity({ 0.0f, 0.0f, -20.0f });
				n->pLarmNode->RotateVelocity({ 0.0f, 0.0f, 20.0f });
				n->RotateVelocity({ -5.0f, 0.0f, 0.0f });
			}
			else if (n->movecount == 1) {
				//終了
				n->pRarmNode->RotateVelocity({ 0.0f, 0.0f, 0.0f });
				n->pLarmNode->RotateVelocity({ 0.0f, 0.0f, 0.0f });
				n->RotateVelocity({ 0.0f, 0.0f, 0.0f });
				
				//キャラクター削除確認
				n->Hp(n->Hp() - 1);
				if (n->Hp() <= 0) {
					LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].chara = nullptr;

					n->DeleteAnimation();
				}
			}
		}
		else if(n->animType == 1) {
			//爆発アニメーション
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
				//移動アニメーション
				if (n->movecount == 10) {

						glm::vec3 vec = n->Velocity();
					if (LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].stair == 1) {
						//移動先が階段の場合
						if (n->pos.y >= n->bepos.y) {
							//上昇
							vec.y = 0.1f;
						}
						else {
							//下降
							vec.y = -0.1f;
						}

					}
					else {
						//移動先が平面の場合
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
					//終了
					//n->Velocity({ 0.0f,0.0f,0.0f });
					//n->RotateVelocity({ 0.0f,0.0f,0.0f });
					if (GameState::MainGame::AttackCharList.size()) {
						//リストがあれば攻撃開始
						if (GameState::MainGame::gamemode != 3) GameState::MainGame::gamemode = 3;
					}
					else {
						//リストが無ければプレイヤー入力待ち
						if (GameState::MainGame::gamemode != 0) GameState::MainGame::gamemode = 0;
					}
					
				}
				if (n->movecount > 0) {
					//カウントが0でなければ移動アニメーション

					//アニメーションカウンター
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
					//終了
			
					if (GameState::MainGame::AttackCharList.size()) {
						//リストがあれば攻撃開始
						if (GameState::MainGame::gamemode != 3) GameState::MainGame::gamemode = 3;
					}
					else {
						//リストが無ければプレイヤー入力待ち
						if (GameState::MainGame::gamemode != 0) GameState::MainGame::gamemode = 0;
					}
				}
			}
			
			
		}
		else if (n->animType == 3) {
			//攻撃アニメーション
			if (n->movecount == 15) {
				//腕振り下ろし
				n->pBody->RotateVelocity({ 0.0f,-20.0f,0.0f });
				n->pRarmNode->RotateVelocity({ 30.0f,0.0f,0.0f });

				//攻撃先確認
				for (int i = -1; i < 2; i++) {
					if (n->MapInJudge(n->vecx[n->vec], i, n->vecz[n->vec]) && LoadState::LoadGame::mapdata[n->pos.x + (n->vecx[n->vec])][n->pos.y + i][n->pos.z + (n->vecz[n->vec])].chara != nullptr) {
						Charactor* e = static_cast<Charactor*>(LoadState::LoadGame::mapdata[n->pos.x + (n->vecx[n->vec])][n->pos.y + i][n->pos.z + (n->vecz[n->vec])].chara);
						if (e->Hp()) {
							//HPがあれば攻撃
							e->DamageAnimation();
						}
					}
				}


			}
			else if (n->movecount == 9) {
				//腕戻し
				n->pBody->RotateVelocity({ 0.0f,10.0f,0.0f });
				n->pRarmNode->RotateVelocity({ 0.0f,0.0f,0.0f });
			}
			else if (n->movecount == 3) {
				//終了
				n->pBody->RotateVelocity({ 0.0f,0.0f,0.0f });
				//n->EnemyAction();
			}
			else if (n->movecount == 1) {
				//リストの先頭を削除
				GameState::MainGame::AttackCharList.erase(GameState::MainGame::AttackCharList.begin());
				if (GameState::MainGame::AttackCharList.size()) {
					//次のキャラクターの攻撃をスタート
					GameState::MainGame::AttackCharList[0]->Attack();
				}
				else {
					//リストが空なのでプレイヤー入力待ちに変更
					if (GameState::MainGame::gamemode != 0) GameState::MainGame::gamemode = 0;
				}
			}
		}
		n->movecount--;
	}
}

/**
*キャラクターアップデート更新
*/
void EnemyCharactor::Update() {
	if (updfnc != nullptr) {
		updfnc(this);
	}
	return;
}

/**
*ダメージアニメーションスタート設定
*/
void EnemyCharactor::DamageAnimation() {
	pRarmNode->RotateVelocity({ 0.0f, 0.0f, 20.0f });
	pLarmNode->RotateVelocity({ 0.0f, 0.0f, -20.0f });
	RotateVelocity({ 5.0f, 0.0f, 0.0f });
	movecount = 13;
	animType = 0;
}

/**
*削除アニメーションスタート設定
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
*キャラクター行動
*/
void EnemyCharactor::Action() {
	GameEngine& game = GameEngine::Instance();
	Velocity({ 0.0f,0.0f,0.0f });
	RotateVelocity({ 0.0f,0.0f,0.0f });
	glm::vec3 vec1;
	float rot1;
	//進行方向リスト
	struct param {
		int vec = 0;///方向
		int type = 0;///0直進　1上がり　2下がり
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
		
		//プレイヤー隣接確認
		if (map1flg && map1.chara == Char::Charactor::Player ||
			map2flg && map2.chara == Char::Charactor::Player ||
			map3flg && map3.chara == Char::Charactor::Player) {
			at = i;
		}

		//直進
		if (map1flg && map1.chara == nullptr && map1.tile && map1.stair != 2) {
			vecList.push_back({ i,0 });
		}
		else if (map2flg && map2.chara == nullptr && map2.tile && map2.stair != 2) {
			//階段上がり
			vecList.push_back({ i,1 });
		}
		else if (map3flg && map3.chara == nullptr && map3.tile) {
			//階段下がり
			vecList.push_back({ i,2 });
		}
	}

	//候補が2つ以上なら真後ろを候補から削除
	if (vecList.size() > 1) {
		for (auto itr = vecList.begin(); itr < vecList.end();itr++) {
			int v = vec - itr->vec;
			if (v == -2 || v == 2) {
				vecList.erase(itr);
				break;
			}
		}
	}

	

	//アニメーションタイプを歩行に変更
	animType = 2;
	movecount = 20;

	bepos.x = pos.x;
	bepos.y = pos.y;
	bepos.z = pos.z;

	//プレイヤーに隣接しているなら攻撃リストに追加
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
			//移動前が階段
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
*キャラクター攻撃
*/
void EnemyCharactor::Attack() {
	GameEngine& game = GameEngine::Instance();
	//攻撃があるならアニメーションスタート
	movecount = 21;
	animType = 3;
	Velocity({ 0.0f,0.0f,0.0f });
	RotateVelocity({ 0.0f,0.0f,0.0f });
	pBody->RotateVelocity({ 0.0f,10.0f,0.0f });
	pRarmNode->RotateVelocity({ -30.0f,0.0f,0.0f });

	game.PlayAudio(GameState::AudioPlayerId::AudioPlayerId_Shot, CRI_SAMPLECUESHEET_PLAYERSHOT);
}