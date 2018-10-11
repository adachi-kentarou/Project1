#include "../Res/Audio/SampleCueSheet.h"
#include "Item.h"
#include "GameEngine.h"
#include "GameState.h"

/**
*���C�t�A�C�e��
*/
void LifeItem::UpdateFnc(LifeItem* n) {
	if (n->movecount != 0) {
		if (n->animType == 1) {
			if (n->movecount == 1) {
				//�I��
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
*�L�����N�^�[�A�b�v�f�[�g�X�V
*/
void LifeItem::Update() {
	if (updfnc != nullptr) {
		updfnc(this);
	}
	return;
}

/**
*�폜�A�j���[�V�����X�^�[�g�ݒ�
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
*�A�C�e���擾����
*/
void LifeItem::GetItem(Char::Charactor* c) {
	printf("HP��\n");
	c->Hp(c->Hp() + 2);
}

/**
*�^�C���A�C�e��
*/
void TimeItem::UpdateFnc(TimeItem* n) {
	if (n->movecount != 0) {
		if (n->animType == 1) {
			if (n->movecount == 1) {
				//�I��
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
*�L�����N�^�[�A�b�v�f�[�g�X�V
*/
void TimeItem::Update() {
	if (updfnc != nullptr) {
		updfnc(this);
	}
	return;
}

/**
*�폜�A�j���[�V�����X�^�[�g�ݒ�
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
*�A�C�e���擾����
*/
void TimeItem::GetItem(Char::Charactor* c) {
	printf("���ԉ���\n");
	GameState::MainGame::timecount += 5;
}
