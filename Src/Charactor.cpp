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
	* �L�����N�^�[�N���X
	*/

	/**
	* �f�X�g���N�^
	*/
	Charactor::~Charactor() {
		//�L�����N�^�[���X�g����폜�t���O
		destroy = true;
		//�̂̃p�[�c���폜
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
		printf("�L�����N�^�[�폜\n");
		/*
		
		*/
	};
	/**
	* �e�X�g�p�X�V.
	*/
	struct UpdateTest
	{
		void operator()(Entity::Entity& entity, double delta)
		{
			
		}

	};

	/**
	* �m�[�h�p�X�V.
	*/
	void Charactor::UpdateNode(Node* n)
	{
		
	};

	/**
	*�L�����N�^�[�}�b�v�͈͓�����
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
	*�L�����N�^�[�}�b�v�z�u
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
	*�L�����N�^�[�z�u�\���W
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
	*�L�����N�^�[�A�b�v�f�[�g�X�V
	*/
	void Charactor::Update() {
		if (updfnc != nullptr) {
			updfnc(this);
		}
		return;
		
	}

	/**
	*�L�����N�^�[�쐬
	*/
	Charactor* Charactor::CreateChara() {
		GameEngine& game = GameEngine::Instance();
		glm::vec3 pos;
		RotateVelocity({0.0f,0.0f,0.0f});
		Scale({0.5f,0.5f, 0.5f});
		//��
		pos = glm::vec3(0, 0, 0);
		if (pBody = game.AddEntity(
			GameState::EntityGroupId_Cube, pos, "Toroid", "", UpdateTest())) {
			pBody->Scale({ 1.0f, 1.0f, 0.5f });
			pBody->RotateVelocity({ 0.0f, 0.0f, 0.0f });
			pBody->SetRotationOrder(Entity::Entity::order::XZY);
			addNode(pBody);
		}
;
		//��
		pos = glm::vec3(0, 1.6f, 0);
		if (pHead = game.AddEntity(
			GameState::EntityGroupId_Enemy, pos, "Toroid", "Res/Enemy.bmp", UpdateTest())) {
			pHead->Scale({ 0.6f, 0.6f, 0.6f });
			pHead->RotateVelocity({ 0.0f, 0.0f, 0.0f });
			pHead->SetRotationOrder(Entity::Entity::order::XYZ);
			addNode(pHead);
		}

		//�E��Entity
		pos = glm::vec3(1, 0, 0);
		if (pRarm = game.AddEntity(
			GameState::EntityGroupId_Cube, pos, "Toroid", "", UpdateTest())) {
			pRarm->Velocity({ 0.0f, 0.0f, 0.0f });
			pRarm->Scale({ 1.0f, 0.2f, 0.2f });
			pRarm->SetRotationOrder(Entity::Entity::order::XZY);
			
			//�E��Node
			pRarmNode = new Node();
			pRarmNode->Position({ 1.0f, 1.0f, 0.0f });

			pRarmNode->addNode(pRarm);
			pBody->addNode(pRarmNode);
		}
		
		//����
		pos = glm::vec3(-1, 0, 0);
		if (pLarm = game.AddEntity(
			GameState::EntityGroupId_Cube, pos, "Toroid", "", UpdateTest())) {
			pLarm->Velocity({ 0.0f, 0.0f, 0.0f });
			pLarm->Scale({ 1.0f, 0.2f, 0.2f });
			pLarm->SetRotationOrder(Entity::Entity::order::XYZ);
			

			//����Node
			pLarmNode = new Node();
			pLarmNode->Position({ -1.0f, 1.0f, 0.0f });

			pLarmNode->addNode(pLarm);
			pBody->addNode(pLarmNode);
		}

		//�E��
		pos = glm::vec3(0, -1, 0);
		if (pRleg = game.AddEntity(
			GameState::EntityGroupId_Cube, pos, "Toroid", "", UpdateTest())) {
			pRleg->Velocity({ 0.0f, 0.0f, 0.0f });
			pRleg->Scale({ 0.3f, 1.0f, 0.3f });
			pRleg->SetRotationOrder(Entity::Entity::order::YXZ);
			//�E��Node
			pRlegNode = new Node();
			pRlegNode->Position({ 0.7f, -1.0f, 0.0f });
			
			pRlegNode->addNode(pRleg);
			pBody->addNode(pRlegNode);
		}

		//����
		pos = glm::vec3(0, -1, 0);
		if (pLleg = game.AddEntity(
			GameState::EntityGroupId_Cube, pos, "Toroid", "", UpdateTest())) {
			pLleg->Velocity({ 0.0f, 0.0f, 0.0f });
			pLleg->Scale({ 0.3f, 1.0f, 0.3f });
			pLleg->SetRotationOrder(Entity::Entity::order::YXZ);
			
			//�E��Node
			pLlegNode = new Node();
			pLlegNode->Position({ -0.7f, -1.0f, 0.0f });

			pLlegNode->addNode(pLleg);
			pBody->addNode(pLlegNode);
		}

		if (!(pBody && pRarm && pLarm && pRleg && pLleg)) {
			std::cerr << "WARNING in Caractor::Create: �L�����N�^�[�̍쐬�Ɏ��s." << std::endl;
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
	*�A�C�e���N���X
	*/
	Item::~Item() {
		//�L�����N�^�[���X�g����폜�t���O
		destroy = true;
		//�̂̃p�[�c���폜
		pImg->removeNode();
		GameEngine::Instance().RemoveEntity(pImg);
		removeNode();
		printf("�A�C�e���폜\n");
	};

	/**
	*�A�C�e���}�b�v�͈͓�����
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
	*�A�C�e���}�b�v�z�u
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
	*�A�C�e���z�u�\���W
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
	*�A�C�e���A�b�v�f�[�g�X�V
	*/
	void Item::Update() {
		if (updfnc != nullptr) {
			updfnc(this);
		}
		return;

	}

	/**
	*�A�C�e���쐬
	*/
	Item* Item::CreateItem(char* tex) {
		GameEngine& game = GameEngine::Instance();
		glm::vec3 pos;
		RotateVelocity({ 0.0f,0.0f,0.0f });
		Scale({ 0.5f,0.5f, 0.5f });
		//�C���[�W�摜
		pos = glm::vec3(0, 0, 0);
		if (pImg = game.AddEntity(
			GameState::EntityGroupId_Image, pos, "Toroid", tex, UpdateTest())) {
			pImg->Scale({ 1.0f, 1.0f, 1.0f });
			pImg->RotateVelocity({ 0.0f, 10.0f, 0.0f });
			pImg->SetRotationOrder(Entity::Entity::order::YXZ);
			addNode(pImg);
		}

		if (!pImg) {
			std::cerr << "WARNING in Caractor::Create: �L�����N�^�[�̍쐬�Ɏ��s." << std::endl;
			pImg->Destroy();
			
			return nullptr;
		}

		pImg->Rotation({ 0.0f, 0.0f, 0.0f });
		
		ItemList.push_back(this);
		return this;

	}

	/**
	*�A�C�e���폜�A�j���[�V����
	*/
	void Item::DeleteAnimation() {

	}

	/**
	*�A�C�e���擾����
	*/
	void Item::GetItem(Charactor* c) {
		printf("��ꃁ�\�b�h\n");
	}
	
}
