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
	/// エンティティの衝突グループID.
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

	/// 音声プレイヤーのID.
	enum AudioPlayerId
	{
		AudioPlayerId_Shot, ///< 自機のショット音.
		AudioPlayerId_Bomb, ///< 爆発音.
		AudioPlayerId_BGM, ///<　BGM.
		AudioPlayerId_Max, ///< 同時発音可能な数.

		AudioPlayerId_UI = AudioPlayerId_Shot, ///<　ユーザーインターフェイス操作音.
	};

	/*
	* タイトル画面.
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
	* メインゲーム画面.
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
		//ゲームモード　0入力待ち　1移動処理　2攻撃処理　3敵攻撃開始　4攻撃終了待機
		static int gamemode;
		//移動キャラリスト
		static std::vector<Char::Charactor*> MoveCharList;
		//攻撃キャラリスト
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