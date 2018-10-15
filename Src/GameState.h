/**
* @file GameState.h
*/
#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED
#include "Entity.h"
#include "Constants.h"
#include "Charactor.h"
#include "LoadState.h"

namespace GameState {
	/// �G���e�B�e�B�̏Փ˃O���[�vID.
	enum EntityGroupId
	{
		EntityGroupId_Display,
		EntityGroupId_Cube,
		EntityGroupId_Tile,
		EntityGroupId_Stair1,
		EntityGroupId_Stair2,
		EntityGroupId_Image,
		EntityGroupId_Enemy,
	};

	/// �����v���C���[��ID.
	enum AudioPlayerId
	{
		AudioPlayerId_Shot, ///< ���@�̃V���b�g��.
		AudioPlayerId_Bomb, ///< ������.
		AudioPlayerId_BGM, ///<�@BGM.
		AudioPlayerId_Max, ///< ���������\�Ȑ�.

		AudioPlayerId_UI = AudioPlayerId_Shot, ///<�@���[�U�[�C���^�[�t�F�C�X���쉹.
	};

	/*
	* �^�C�g�����.
	*/
	class Title
	{
	public:
		explicit Title(Entity::Entity* p = nullptr) : pSpaceSphere(p) {}
		void operator()(double delta);
	private:
		Entity::Entity* pSpaceSphere = nullptr;
		float timer = 0;
	};

	/**
	* ���C���Q�[�����.
	*/
	class MainGame
	{
	public:
		explicit MainGame();
		void operator()(double delta);
		
		static int time;
		static int timev;
		static int timecount;
		static int sec;
		static int stageno;
		//�Q�[�����[�h�@0���͑҂��@1�ړ������@2�U�������@3�G�U���J�n�@4�U���I���ҋ@
		static int gamemode;
		//�ړ��L�������X�g
		static std::vector<Char::Charactor*> MoveCharList;
		//�U���L�������X�g
		static std::vector<Char::Charactor*> AttackCharList;

	private:
		bool isInitialized = false;
		double interval = 0;
		Entity::Entity* pPlayer = nullptr;
		Entity::Entity* pSpaceSphere = nullptr;

		Entity::Entity* test;
		Char::Charactor* player = nullptr;

		bool enemyaction = false;

		int start = 180;
	};


}

#endif