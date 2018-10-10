/**
* @file Map.h
*/
#ifndef LOAD_INCLUDED
#define LOAD_INCLUDED
#include "Charactor.h"
#include <vector>
#include <future>
//#include "Map.h"


namespace LoadState {
	//bool loadflg = false;
	struct MapData {
		bool tile = false;///床
		int x = 0;///x座標
		int y = 0;///y座標
		int stair = 0;///階段　0階段無し　1階段下段　2階段上段
		Char::Charactor* chara = nullptr;///座標にいるキャラクター
		Char::Item* item = nullptr;///座標にあるアイテム
	};
	
	struct MapPoint {
		int x = 0;
		int y = 0;
		int z = 0;
		int v = 0;
	};

	//マップ作製当たり判定用データ
	struct MapInitData {
		int x = 0;///x方向
		int y = 0;///y方向
		int z = 0;///z方向
		int x1 = 0;///x進行方向確認始点
		int y1 = 0;///y進行方向確認始点
		int z1 = 0;///z進行方向確認始点
		int x2 = 0;///x進行方向確認終点
		int y2 = 0;///y進行方向確認終点
		int z2 = 0;///z進行方向確認終点

	};
	
	//bool loadflg = false;
	void CreateMap();
	bool MapIn(int x, int y, int z);
	bool MapInJudge(int x, int y, int z);
	bool MapSetJudge(int x, int y, int z);
	/**
	* メインゲーム画面.
	*/
	class LoadGame
	{
	public:
		LoadGame(){};
		void operator()(double delta);
		static bool loadflg;
		static std::vector<std::vector<std::vector<LoadState::MapData>>> mapdata;
		int time = 0;
	private:
		bool isInitialized = false;
		double interval = 0;
	};


}

#endif