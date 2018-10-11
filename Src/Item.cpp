#include "../Res/Audio/SampleCueSheet.h"
#include "Item.h"
#include "GameEngine.h"
#include "GameState.h"

/**
*ライフアイテム
*/
void LifeItem::UpdateFnc(LifeItem* n) {
	if (n->movecount != 0) {
		if (n->animType == 1) {
			if (n->movecount == 1) {
				//終了
				n->Velocity({ 0.0f,0.0f,0.0f });
				n->RotateVelocity({ 0.0f,0.0f,0.0f });
				n->removeNode();

				if (n != nullptr) { delete n; };

				return;
			}
		}
		n->movecount--;
	}
	
	
	
}

/**
*キャラクターアップデート更新
*/
void LifeItem::Update() {
	if (updfnc != nullptr) {
		updfnc(this);
	}
	return;
}

/**
*削除アニメーションスタート設定
*/
void LifeItem::DeleteAnimation() {

	GameEngine & game = GameEngine::Instance();
	game.PlayAudio(GameState::AudioPlayerId::AudioPlayerId_UI, CRI_SAMPLECUESHEET_START);
	ScaleVelocity({ 0.2f, 0.2f, 0.2f });
	ColorVelocity({ 0.0f,0.0f, 0.0f, -0.05f });
	movecount = 21;
	animType = 1;
}

/**
*アイテム取得効果
*/
void LifeItem::GetItem(Char::Charactor* c) {
	printf("HP回復\n");
	c->Hp(c->Hp() + 2);
}

/**
*タイムアイテム
*/
void TimeItem::UpdateFnc(TimeItem* n) {
	if (n->movecount != 0) {
		if (n->animType == 1) {
			if (n->movecount == 1) {
				//終了
				n->Velocity({ 0.0f,0.0f,0.0f });
				n->RotateVelocity({ 0.0f,0.0f,0.0f });
				n->removeNode();

				if (n != nullptr) { delete n; };

				return;
			}
		}
		n->movecount--;
	}



}

/**
*キャラクターアップデート更新
*/
void TimeItem::Update() {
	if (updfnc != nullptr) {
		updfnc(this);
	}
	return;
}

/**
*削除アニメーションスタート設定
*/
void TimeItem::DeleteAnimation() {

	GameEngine & game = GameEngine::Instance();
	game.PlayAudio(GameState::AudioPlayerId::AudioPlayerId_UI, CRI_SAMPLECUESHEET_START);
	ScaleVelocity({ 0.2f, 0.2f, 0.2f });
	ColorVelocity({ 0.0f,0.0f, 0.0f, -0.05f });
	movecount = 21;
	animType = 1;
}

/**
*アイテム取得効果
*/
void TimeItem::GetItem(Char::Charactor* c) {
	printf("時間延長\n");
	GameState::MainGame::timecount += 5;
}
