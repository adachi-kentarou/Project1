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
		bool tile = false;///��
		int x = 0;///x���W
		int y = 0;///y���W
		int stair = 0;///�K�i�@0�K�i�����@1�K�i���i�@2�K�i��i
		Char::Charactor* chara = nullptr;///���W�ɂ���L�����N�^�[
		Char::Item* item = nullptr;///���W�ɂ���A�C�e��
	};
	
	struct MapPoint {
		int x = 0;
		int y = 0;
		int z = 0;
		int v = 0;
	};

	//�}�b�v�쐻�����蔻��p�f�[�^
	struct MapInitData {
		int x = 0;///x����
		int y = 0;///y����
		int z = 0;///z����
		int x1 = 0;///x�i�s�����m�F�n�_
		int y1 = 0;///y�i�s�����m�F�n�_
		int z1 = 0;///z�i�s�����m�F�n�_
		int x2 = 0;///x�i�s�����m�F�I�_
		int y2 = 0;///y�i�s�����m�F�I�_
		int z2 = 0;///z�i�s�����m�F�I�_

	};
	
	//bool loadflg = false;
	void CreateMap();
	bool MapIn(int x, int y, int z);
	bool MapInJudge(int x, int y, int z);
	bool MapSetJudge(int x, int y, int z);
	/**
	* ���C���Q�[�����.
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