/**
* @file Charactor.h
*/
#include "Constants.h"
#include "Charactor.h"
#include "GameState.h"
#include "GameEngine.h"
std::vector<Char::Charactor*> Char::Charactor::CharList;
std::vector<Char::Item*> Char::Item::ItemList;
Char::Charactor* Char::Charactor::Player = nullptr;
struct aaaa {
	void operator()(){}
};
namespace Char {
	/**
	* キャラクタークラス
	*/

	/**
	* デストラクタ
	*/
	Charactor::~Charactor() {
		//キャラクターリストから削除フラグ
		destroy = true;
		//体のパーツを削除
		pBody->removeNode();
		GameEngine::Instance().RemoveEntity(pBody);
		pHead->removeNode();
		GameEngine::Instance().RemoveEntity(pHead);
		pRarm->removeNode();
		GameEngine::Instance().RemoveEntity(pRarm);
		pLarm->removeNode();
		GameEngine::Instance().RemoveEntity(pLarm);
		pRleg->removeNode();
		GameEngine::Instance().RemoveEntity(pRleg);
		pLleg->removeNode();
		GameEngine::Instance().RemoveEntity(pLleg);
		removeNode();
		if (pRarmNode != nullptr) { delete pRarmNode; };
		if (pRarmNode != nullptr) { delete pLarmNode; };
		if (pRarmNode != nullptr) { delete pRlegNode; };
		if (pRarmNode != nullptr) { delete pLlegNode; };
		printf("キャラクター削除\n");
		/*
		
		*/
	};
	/**
	* テスト用更新.
	*/
	struct UpdateTest
	{
		void operator()(Entity::Entity& entity, double delta)
		{
			
		}

	};

	/**
	* ノード用更新.
	*/
	void Charactor::UpdateNode(Node* n)
	{
		
	};

	/**
	*キャラクターマップ範囲内判定
	*/
	bool Charactor::MapInJudge(int x, int y, int z) {
		if(pos.x + x >= 0 &&
			pos.x + x < MAP_MAX_X &&
			pos.y + y >= 0 &&
			pos.y + y < MAP_MAX_Y &&
			pos.z + z >= 0 &&
			pos.z + z < MAP_MAX_Z
			){
			return true;
		}
		return false;
	}

	/**
	*キャラクターマップ配置
	*/
	void Charactor::SetMapPosition(int x = 0, int y = 0, int z = 0) {
		if (x >= 0 &&
			x < MAP_MAX_X &&
			y >= 0 &&
			y < MAP_MAX_Y &&
			z >= 0 &&
			z < MAP_MAX_Z) {
			pos.x = x;
			pos.y = y;
			pos.z = z;
			LoadState::LoadGame::mapdata[x][y][z].chara = this;
			Position(glm::vec3(x * 2, y * 2 + 1.4f + (LoadState::LoadGame::mapdata[x][y][z].stair == 1 ? 1.0f : 0.0f), z * 2));
		}
	}

	/**
	*キャラクター配置可能座標
	*/
	MapPoint Charactor::GetMapPosition() {
		GameEngine& game = GameEngine::Instance();
		const std::uniform_real_distribution<float> rnd(0.0f, 1.0f);
		
		std::vector<MapPoint> m;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				for (int k = 0; k < 10; k++) {
					if (LoadState::LoadGame::mapdata[i][j][k].tile &&
						LoadState::LoadGame::mapdata[i][j][k].stair != 2 &&
						LoadState::LoadGame::mapdata[i][j][k].chara == nullptr &&
						!((int)Char::Charactor::Player->pos.x  - 1 <= i &&
						(int)Char::Charactor::Player->pos.x + 1 >= i &&
						(int)Char::Charactor::Player->pos.y - 1 <= j &&
						(int)Char::Charactor::Player->pos.y + 1 >= j &&
						(int)Char::Charactor::Player->pos.z - 1 <= k &&
						(int)Char::Charactor::Player->pos.z + 1 >= k
						)) {
						m.push_back({i,j,k});
					}
				}
			}
		}
		if (m.size()) {
			return m[(int)(m.size() * rnd(game.Rand()))];
		}
		else {
			return {-1,-1,-1};
		}
		
	}

	/**
	*キャラクターアップデート更新
	*/
	void Charactor::Update() {
		if (updfnc != nullptr) {
			updfnc(this);
		}
		return;
		
	}

	/**
	*キャラクター作成
	*/
	Charactor* Charactor::CreateChara() {
		GameEngine& game = GameEngine::Instance();
		glm::vec3 pos;
		RotateVelocity({0.0f,0.0f,0.0f});
		Scale({0.5f,0.5f, 0.5f});
		//体
		pos = glm::vec3(0, 0, 0);
		if (pBody = game.AddEntity(
			GameState::EntityGroupId_Cube, pos, "Toroid", "", UpdateTest())) {
			pBody->Scale({ 1.0f, 1.0f, 0.5f });
			pBody->RotateVelocity({ 0.0f, 0.0f, 0.0f });
			pBody->SetRotationOrder(Entity::Entity::order::XZY);
			addNode(pBody);
		}
;
		//頭
		pos = glm::vec3(0, 1.6f, 0);
		if (pHead = game.AddEntity(
			GameState::EntityGroupId_Enemy, pos, "Toroid", "Res/Enemy.bmp", UpdateTest())) {
			pHead->Scale({ 0.6f, 0.6f, 0.6f });
			pHead->RotateVelocity({ 0.0f, 0.0f, 0.0f });
			pHead->SetRotationOrder(Entity::Entity::order::XYZ);
			addNode(pHead);
		}

		//右手Entity
		pos = glm::vec3(1, 0, 0);
		if (pRarm = game.AddEntity(
			GameState::EntityGroupId_Cube, pos, "Toroid", "", UpdateTest())) {
			pRarm->Velocity({ 0.0f, 0.0f, 0.0f });
			pRarm->Scale({ 1.0f, 0.2f, 0.2f });
			pRarm->SetRotationOrder(Entity::Entity::order::XZY);
			
			//右手Node
			pRarmNode = new Node();
			pRarmNode->Position({ 1.0f, 1.0f, 0.0f });

			pRarmNode->addNode(pRarm);
			pBody->addNode(pRarmNode);
		}
		
		//左手
		pos = glm::vec3(-1, 0, 0);
		if (pLarm = game.AddEntity(
			GameState::EntityGroupId_Cube, pos, "Toroid", "", UpdateTest())) {
			pLarm->Velocity({ 0.0f, 0.0f, 0.0f });
			pLarm->Scale({ 1.0f, 0.2f, 0.2f });
			pLarm->SetRotationOrder(Entity::Entity::order::XYZ);
			

			//左手Node
			pLarmNode = new Node();
			pLarmNode->Position({ -1.0f, 1.0f, 0.0f });

			pLarmNode->addNode(pLarm);
			pBody->addNode(pLarmNode);
		}

		//右足
		pos = glm::vec3(0, -1, 0);
		if (pRleg = game.AddEntity(
			GameState::EntityGroupId_Cube, pos, "Toroid", "", UpdateTest())) {
			pRleg->Velocity({ 0.0f, 0.0f, 0.0f });
			pRleg->Scale({ 0.3f, 1.0f, 0.3f });
			pRleg->SetRotationOrder(Entity::Entity::order::YXZ);
			//右足Node
			pRlegNode = new Node();
			pRlegNode->Position({ 0.7f, -1.0f, 0.0f });
			
			pRlegNode->addNode(pRleg);
			pBody->addNode(pRlegNode);
		}

		//左足
		pos = glm::vec3(0, -1, 0);
		if (pLleg = game.AddEntity(
			GameState::EntityGroupId_Cube, pos, "Toroid", "", UpdateTest())) {
			pLleg->Velocity({ 0.0f, 0.0f, 0.0f });
			pLleg->Scale({ 0.3f, 1.0f, 0.3f });
			pLleg->SetRotationOrder(Entity::Entity::order::YXZ);
			
			//右足Node
			pLlegNode = new Node();
			pLlegNode->Position({ -0.7f, -1.0f, 0.0f });

			pLlegNode->addNode(pLleg);
			pBody->addNode(pLlegNode);
		}

		if (!(pBody && pRarm && pLarm && pRleg && pLleg)) {
			std::cerr << "WARNING in Caractor::Create: キャラクターの作成に失敗." << std::endl;
			pBody->Destroy();
			pRarm->Destroy();
			pLarm->Destroy();
			pRleg->Destroy();
			pLleg->Destroy();

			return nullptr;
		}

		pRlegNode->Rotation({ 0.0f, 0.0f, 0.0f });
		pLlegNode->Rotation({ 0.0f, 0.0f, 0.0f });
		pRarmNode->Rotation({ 0.0f, 0.0f, 300.0f });
		pLarmNode->Rotation({ 0.0f, 0.0f, 60.0f });

		CharList.push_back(this);
		return this;

	}

	/**
	*アイテムクラス
	*/
	Item::~Item() {
		//キャラクターリストから削除フラグ
		destroy = true;
		//体のパーツを削除
		pImg->removeNode();
		GameEngine::Instance().RemoveEntity(pImg);
		removeNode();
		printf("アイテム削除\n");
	};

	/**
	*アイテムマップ範囲内判定
	*/
	bool Item::MapInJudge(int x, int y, int z) {
		if (pos.x + x >= 0 &&
			pos.x + x < MAP_MAX_X &&
			pos.y + y >= 0 &&
			pos.y + y < MAP_MAX_Y &&
			pos.z + z >= 0 &&
			pos.z + z < MAP_MAX_Z
			) {
			return true;
		}
		return false;
	}

	/**
	*アイテムマップ配置
	*/
	void Item::SetMapPosition(int x = 0, int y = 0, int z = 0) {
		if (x >= 0 &&
			x < MAP_MAX_X &&
			y >= 0 &&
			y < MAP_MAX_Y &&
			z >= 0 &&
			z < MAP_MAX_Z) {
			pos.x = x;
			pos.y = y;
			pos.z = z;
			LoadState::LoadGame::mapdata[x][y][z].item = this;
			Position(glm::vec3(x * 2, y * 2 + 1.4f + (LoadState::LoadGame::mapdata[x][y][z].stair == 1 ? 1.0f : 0.0f), z * 2));
		}
	}

	/**
	*アイテム配置可能座標
	*/
	MapPoint Item::GetMapPosition() {
		GameEngine& game = GameEngine::Instance();
		const std::uniform_real_distribution<float> rnd(0.0f, 1.0f);

		std::vector<MapPoint> m;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				for (int k = 0; k < 10; k++) {
					if (LoadState::LoadGame::mapdata[i][j][k].tile &&
						LoadState::LoadGame::mapdata[i][j][k].stair != 2 &&
						LoadState::LoadGame::mapdata[i][j][k].chara == nullptr &&
						LoadState::LoadGame::mapdata[i][j][k].item == nullptr) {
						m.push_back({ i,j,k });
					}
				}
			}
		}
		if (m.size()) {
			return m[(int)(m.size() * rnd(game.Rand()))];
		}
		else {
			return { -1,-1,-1 };
		}

	}

	/**
	*アイテムアップデート更新
	*/
	void Item::Update() {
		if (updfnc != nullptr) {
			updfnc(this);
		}
		return;

	}

	/**
	*アイテム作成
	*/
	Item* Item::CreateItem(char* tex) {
		GameEngine& game = GameEngine::Instance();
		glm::vec3 pos;
		RotateVelocity({ 0.0f,0.0f,0.0f });
		Scale({ 0.5f,0.5f, 0.5f });
		//イメージ画像
		pos = glm::vec3(0, 0, 0);
		if (pImg = game.AddEntity(
			GameState::EntityGroupId_Image, pos, "Toroid", tex, UpdateTest())) {
			pImg->Scale({ 1.0f, 1.0f, 1.0f });
			pImg->RotateVelocity({ 0.0f, 10.0f, 0.0f });
			pImg->SetRotationOrder(Entity::Entity::order::YXZ);
			addNode(pImg);
		}

		if (!pImg) {
			std::cerr << "WARNING in Caractor::Create: キャラクターの作成に失敗." << std::endl;
			pImg->Destroy();
			
			return nullptr;
		}

		pImg->Rotation({ 0.0f, 0.0f, 0.0f });
		
		ItemList.push_back(this);
		return this;

	}

	/**
	*アイテム削除アニメーション
	*/
	void Item::DeleteAnimation() {

	}

	/**
	*アイテム取得効果
	*/
	void Item::GetItem(Charactor* c) {
		printf("基底メソッド\n");
	}
	
}
