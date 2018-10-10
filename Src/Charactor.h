/**
* @file Charactor.h
*/
#ifndef CHARACTOR_INCLUDED
#define CHARACTOR_INCLUDED
#include "Node.h"
#include "Entity.h"
#include <memory>
#include <vector>

namespace Char {
	struct MapPoint {
		int x = 0;
		int y = 0;
		int z = 0;
	};
	class Charactor : public Node {
	public:
		~Charactor();
		//キャラクターの向き
		enum vector {
			front,
			right,
			back,
			left
		};
		//移動アニメーション用カウント
		int movecount = 0;

		//座標
		struct position {
			unsigned int x = 0;
			unsigned int y = 0;
			unsigned int z = 0;
		};
		struct position pos;
		struct position bepos;

		const int vecz[4] = { 1,0,-1,0 };
		const int vecx[4] = { 0,1,0,-1 };

		//0=前　1=右 2=後 3=左
		unsigned int vec = 0;

		Entity::Entity* pBody;
		Entity::Entity* pHead;
		Entity::Entity* pRarm;
		Entity::Entity* pLarm;
		Entity::Entity* pRleg;
		Entity::Entity* pLleg;
		
		Node* pRarmNode;
		Node* pLarmNode;
		Node* pRlegNode;
		Node* pLlegNode;

		Charactor* CreateChara();
		static std::vector<Char::Charactor*> CharList;
		
		static Charactor* Player;

		void UpdateNode(Node*);

		virtual void Update();

		virtual void Attack(){};
		
		bool MapInJudge(int x = 0,int y = 0,int z = 0);
		void SetMapPosition(int x, int y, int z);
		
		int Hp() { return hp; };
		void Hp(int h) { hp = h; };

		bool Destroy() { return destroy; };
		
		MapPoint GetMapPosition();

		int animationcount = 0;
		void SetFunc(std::function<void(Node*)> fnc) { updfnc = fnc; };

		std::function<void(Node*)> updfnc = nullptr;

		int animType = 0;

		int actCount = 1;

		virtual void DamageAnimation() {};
	private:
		int hp = 0;
		bool destroy = false;

	};
	class Item : public Node {
	public:
		~Item();
		//テクスチャー
		char* tex = "Res/Life.bmp";
		//座標
		struct position {
			unsigned int x = 0;
			unsigned int y = 0;
			unsigned int z = 0;
		};
		struct position pos;

		Item* CreateItem();

		static std::vector<Char::Item*> ItemList;

		virtual void Update();
		std::function<void(Node*)> updfnc = nullptr;

		Entity::Entity* pImg;

		bool MapInJudge(int x = 0, int y = 0, int z = 0);
		void SetMapPosition(int x, int y, int z);
		bool Destroy() { return destroy; };
		MapPoint GetMapPosition();

		int animType = 0;
		//移動アニメーション用カウント
		int movecount = 0;

		virtual void DeleteAnimation();

		virtual void GetItem(Charactor* c);
	private:
		bool destroy = false;

	};
	
}




#endif
