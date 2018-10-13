#include "Constants.h"
#include "../Res/Audio/SampleCueSheet.h"
#include "Charactor.h"
#include "PlayerCharactor.h"
#include "EnemyCharactor.h"
#include "GameEngine.h"
#include "GameState.h"

/**
*デストラクタ
*/
PlayerCharactor::~PlayerCharactor() {
	Char::Charactor::Player = nullptr;
};

void PlayerCharactor::UpdateFnc(PlayerCharactor* n) {

	GameEngine& game = GameEngine::Instance();
	const GamePad gamepad = game.GetGamePad();
	if (n->movecount == 0) {
		//終了
		n->Velocity({ 0.0f,0.0f,0.0f });
		n->RotateVelocity({ 0.0f,0.0f,0.0f });

	}
	if (GameState::MainGame::gamemode == 0 && n->movecount == 0) {
		
		
		//カウントが0ならばキー入力
		//プレイヤー操作
		
		glm::vec3 vec;
		glm::vec3 rot;
		if (gamepad.mouseClick) {
			//ゲームモードを攻撃に変更
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
			//方向候補リスト　0=候補なし 1=直進　2=上がり 3=下がり
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
				//直進
				if (map1flg && map1.chara == nullptr && map1.tile && map1.stair != 2) {
					direction[i] = 1;

				}
				else if (map2flg && map2.chara == nullptr && map2.tile && map2.stair != 2) {
					//階段上がり
					direction[i] = 2;

				}
				else if (map3flg && map3.chara == nullptr && map3.tile) {
					//階段下がり
					direction[i] = 3;

				}

			}

			int dir = 0;
			
			if (direction[1]) {
				//直進
				dir = direction[1];
			}
			else if (direction[2] && !direction[0]) {
				//右回転
				dir = direction[2];
				rot.y = 4.5f;
				n->vec += 1;
				if (n->vec == 4) { n->vec = 0; }
			}
			else if (direction[0] && !direction[2]) {
				//左回転
				dir = direction[0];
				rot.y = -4.5f;
				n->vec -= 1;
				if (n->vec == -1) { n->vec = 3; }
			}
			//直進
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
				//階段上がり
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
				//階段下がり
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
			//移動回転があるならアニメーションスタート
			n->movecount = 20;
			n->animType = 1;
		}
		if (vec.x || vec.z) {
			n->EnemyAction();
			if (LoadState::LoadGame::mapdata[n->bepos.x][n->bepos.y][n->bepos.z].stair == 1) {
				//移動前が階段
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
			//攻撃アニメーション
			if (n->movecount == 15) {
				//腕振り下ろし
				n->pBody->RotateVelocity({ 0.0f,-20.0f,0.0f });
				n->pRarmNode->RotateVelocity({ 30.0f,0.0f,0.0f });

				//攻撃先確認
				for (int i = -1; i < 2;i++) {
					if (n->MapInJudge(n->vecx[n->vec], i, n->vecz[n->vec]) && LoadState::LoadGame::mapdata[n->pos.x + (n->vecx[n->vec])][n->pos.y + i][n->pos.z + (n->vecz[n->vec])].chara != nullptr) {
						EnemyCharactor* e = static_cast<EnemyCharactor*>(LoadState::LoadGame::mapdata[n->pos.x + (n->vecx[n->vec])][n->pos.y + i][n->pos.z + (n->vecz[n->vec])].chara);
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
				if (GameState::MainGame::gamemode != 0) GameState::MainGame::gamemode = 0;
				n->EnemyAction();
			}
		}
		else if(n->animType == 1){
			//移動アニメーション
			if (n->movecount == 10 && (n->Velocity().x || n->Velocity().y || n->Velocity().z)) {
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
			}
			else if (n->movecount == 1) {
				//アイテム確認
				if (LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].item != nullptr) {
					printf("アイテムゲット\n");
					LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].item->DeleteAnimation();
					LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].item->GetItem(n);
					LoadState::LoadGame::mapdata[n->pos.x][n->pos.y][n->pos.z].item = nullptr;


				}
				
			}
			else if (n->movecount == 0) {
				//終了
				n->Velocity({ 0.0f,0.0f,0.0f });
				n->RotateVelocity({ 0.0f,0.0f,0.0f });

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
		else if (n->animType == 2) {
			//ダメージアニメーション
			if (n->movecount == 8) {
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
			else if (n->animType == 3) {
				//ゲームオーバーアニメーション
				if (n->movecount == 1) {
					//終了
					n->Velocity({ 0.0f,0.0f,0.0f });
					n->RotateVelocity({ 0.0f,0.0f, 0.0f });
				}
			}
		}
		n->movecount--;
		//printf("%f\n", n->Position().x);
	}

	glm::vec3 pos = n->Position();

	//カメラ座標変更
	GameEngine::CameraData camera = game.Camera();
	camera.position = pos + glm::vec3(0.0f, 0.0f, 0.0f);
	game.Camera(camera);

}

/**
*キャラクターアップデート更新
*/
void PlayerCharactor::Update() {
	if (updfnc != nullptr) {
		updfnc(this);
	}
	return;
}

/*
*キャラクター攻撃
*/
void PlayerCharactor::Attack() {
	GameEngine& game = GameEngine::Instance();
	GameState::MainGame::gamemode = 2;
	//攻撃があるならアニメーションスタート
	movecount = 21;
	animType = 0;
	Velocity({ 0.0f,0.0f,0.0f });
	RotateVelocity({ 0.0f,0.0f,0.0f });
	pBody->RotateVelocity({ 0.0f,10.0f,0.0f });
	pRarmNode->RotateVelocity({ -30.0f,0.0f,0.0f });

	game.PlayAudio(GameState::AudioPlayerId::AudioPlayerId_Shot, CRI_SAMPLECUESHEET_PLAYERSHOT);
}

/*
*キャラクター行動決定
*/
void PlayerCharactor::EnemyAction() {
	GameState::MainGame::gamemode = 1;
	for (int itr = 0; Char::Charactor::CharList.begin() + itr < Char::Charactor::CharList.end(); itr++) {
		
		auto itrr = Char::Charactor::CharList.begin() + itr;
		//プレイヤーキャラなら飛ばす
		if (*itrr == this) continue;

		if (static_cast<Char::Charactor*>(*itrr)->Hp()) {
			static_cast<EnemyCharactor*>(*itrr)->Action();
		}
	}
}

/**
*ダメージアニメーションスタート設定
*/
void PlayerCharactor::DamageAnimation() {
	pRarmNode->RotateVelocity({ 0.0f, 0.0f, 20.0f });
	pLarmNode->RotateVelocity({ 0.0f, 0.0f, -20.0f });
	RotateVelocity({ 5.0f, 0.0f, 0.0f });
	movecount = 14;
	animType = 2;
}

/**
*削除アニメーションスタート設定
*/
void PlayerCharactor::DeleteAnimation() {
	GameEngine & game = GameEngine::Instance();
	game.PlayAudio(GameState::AudioPlayerId::AudioPlayerId_Bomb, CRI_SAMPLECUESHEET_BOMB);
	Velocity({ 0.0f,-0.15f,0.0f });
	RotateVelocity({ 10.0f,0.0f, 0.0f });
	movecount = 9;
	animType = 3;
}