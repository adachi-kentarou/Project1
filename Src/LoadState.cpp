#include "GameEngine.h"
#include "Entity.h"
#include "LoadState.h"
#include "Constants.h"
#include  <vector>

namespace LoadState {

	struct MapInitData m[6] = {
		{ 0,0,1,-1,-1,1,1,1,2 },///�O
	{ 1,0,0,1,-1,-1,2,1,1 },///��
	{ 0,0,-1,-1,-1,-2,1,1,-1 },///��
	{ -1,0,0,-2,-1,-1,-1,1,1 },///�E
	{ 0,-1,0,-1,-2,-1,1,-1,1 },///��
	{ 0,1,0,-1,1,-1,1,2,1 },///��
	};
	bool LoadGame::loadflg;
	std::vector<std::vector<std::vector<LoadState::MapData>>> LoadGame::mapdata;



	/// �^�C�g����ʂ̍X�V.
	void LoadGame::operator()(double delta) {
		printf("load\n");
		GameEngine& game = GameEngine::Instance();
		game.Camera({ glm::vec3(0, 20, -8), glm::vec3(0, 0, 12), glm::vec3(0, 0, 1) });

		game.FontScale(glm::vec2(2.0f, 2.0f));
		game.FontColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		game.AddString(glm::vec2(300.0f, 260.0f), "NOW LOADING");

		time++;
		if (time > 30) {
			LoadState::CreateMap();
			printf("change load\n");
		}
		if (loadflg) {
			loadflg = false;
			printf("finish\n");
			game.UpdateFunc(GameState::MainGame());
		}
		

	}

	bool MapIn(int x, int y, int z) {
		if (x >= 0 &&
			x < MAP_MAX_X &&
			y >= 0 &&
			y < MAP_MAX_Y &&
			z >= 0 &&
			z < MAP_MAX_Z
			) {
			return true;
		}
		return false;
	}

	bool MapInJudge(int x, int y, int z) {
		if (MapIn(x,y,z) &&
			LoadGame::mapdata[x][y][z].tile
			) {
			return true;
		}
		return false;
	}

	bool MapSetJudge(int x, int y, int z) {
		if (!MapIn(x, y, z) ||
			!LoadGame::mapdata[x][y][z].tile
			) {
			return true;
		}
		return false;
	}

	void CreateMap() {

		//���C���Q�[���p�����[�^�[������
		
		GameState::MainGame::gamemode = 0;
		GameState::MainGame::time = 0;
		GameState::MainGame::timev = -1;
		GameState::MainGame::timecount = STAGE_TIME + (GameState::MainGame::stageno - 1) * STAGE_TIME_UP;
		GameState::MainGame::sec = 0;
		GameState::MainGame::MoveCharList.clear();
		GameState::MainGame::AttackCharList.clear();

		LoadGame::loadflg = false;
		bool *flg = &LoadGame::loadflg;

		std::thread thr = std::thread([flg] {
			GameEngine& game = GameEngine::Instance();
			game.AmbientLight(glm::vec4(0.4f, 0.4f, 0.4f, 1));
			//�����Ƀ}�b�v�쐬����
			//�}�b�v������
			LoadGame::mapdata.clear();
			
			//�}�b�v�|�C���g���X�g
			std::vector<MapPoint> mapList;
			//�����⃊�X�g
			std::vector<MapPoint> branchList;
			std::vector<MapPoint> branchListTile;
			std::vector<MapPoint> branchListStair;
			
			for (int i = 0; i < MAP_MAX_X;i++) {
				std::vector<std::vector<LoadState::MapData>> first;
				LoadGame::mapdata.push_back(first);
				for (int j = 0; j < MAP_MAX_Y; j++) {
					std::vector<LoadState::MapData> second;
					LoadGame::mapdata[i].push_back(second);
					for (int k = 0; k < MAP_MAX_Z; k++) {
						LoadGame::mapdata[i][j].push_back(LoadState::MapData());
					}
				}
			}

			//�����ݒ�
			const std::uniform_real_distribution<float> rnd(0.0f, 1.0f);
			const std::uniform_real_distribution<float> rnd1(1.0f, LOAD_LENGTH);
			const std::uniform_real_distribution<float> rnd2(0.0f, 2.0f);

			//�X�^�[�g�ʒu�ݒ�
			mapList.push_back({ 
				(int)(rnd(game.Rand()) * 10),
				(int)(rnd(game.Rand()) * 10),
				(int)(rnd(game.Rand()) * 10),
				0
			});
			LoadGame::mapdata[mapList[0].x][mapList[0].y][mapList[0].z].tile = true;
			
			//���̒��i��
			for (int i = 0; i < LOAD_LOOP + LOAD_LOOP_UP * GameState::MainGame::stageno; i++) {
				printf("���[�v��%d**************************\n",i);
				//�����̃}�b�v�|�C���g���X�g���番����𒲂ׂ�
				branchList.clear();
				branchListTile.clear();
				branchListStair.clear();
				//for (auto itr = mapList.begin(); itr < mapList.end(); itr++) {
				for (int iitr = 0; mapList.begin() + iitr < mapList.end(); iitr++) {
					auto itr = mapList.begin() + iitr;
					//6����
					printf("x: %d y:%d z:%d stair%d--------\n",itr->x, itr->y, itr->z,LoadGame::mapdata[itr->x][itr->y][itr->z].stair);
					//���ڑ��K�i����
					int st = ((int)MapInJudge(itr->x + 1, itr->y, itr->z) +
						(int)MapInJudge(itr->x - 1, itr->y, itr->z) +
						(int)MapInJudge(itr->x, itr->y, itr->z + 1) +
						(int)MapInJudge(itr->x, itr->y, itr->z - 1));
					printf("a = %d///////////\n",st);
					//bool st = a <= 1;
					//�}�b�v�ǉ������t���O
					bool chk = true;
					for (int v = 0; v < 6; v++) {
						MapInitData mm = m[v];
						bool jdg = true;
						if (MapIn(itr->x + mm.x, itr->y + mm.y, itr->z + mm.z)) {
							for (int ii = mm.x1; ii <= mm.x2; ii++) {
								for (int jj = mm.y1; jj <= mm.y2; jj++) {
									for (int kk = mm.z1; kk <= mm.z2; kk++) {
										jdg &= MapSetJudge(ii + itr->x, jj + itr->y, kk + itr->z);
										if (!jdg) {
											//���肪���s�����i�K�Ń��[�v���I��
											goto END;
										}
									}
								}
							}
							
							//�S�Ĕ��萬�������ꍇ�̓��X�g�Ƀv�b�V��
							if (v < 4) {
								//��
								if (LoadGame::mapdata[itr->x][itr->y][itr->z].stair != 0) {
									//�K�i�ڑ�
									if (!st) {
										//���ڑ��Ȃ烊�X�g�ɒǉ�
										branchList.push_back({ itr->x + mm.x, itr->y + mm.y, itr->z + mm.z, v });
										chk = false;
									}
								}
								else {
									//���ڑ�
									branchListTile.push_back({ itr->x + mm.x, itr->y + mm.y, itr->z + mm.z, v });
									chk = false;
								}
							}
							else {
								//�K�i
								if (st <= 1 && LoadGame::mapdata[itr->x][itr->y][itr->z].stair == 0) {
									branchListStair.push_back({ itr->x + mm.x, itr->y + mm.y, itr->z + mm.z, v });
									chk = false;
								}
							}
							if(!chk) printf("����%d\n", v);

						END:;
						}
					}
					if (chk) {
						//�ǉ������Ȃ烊�X�g����폜
						printf("�ǉ������̈׍폜 x:%d y:%d z:%d\n",itr->x, itr->y, itr->z);
						mapList.erase(itr);
						iitr--;
						
					}
				}
				printf("�K�i%d �K�i�ڑ�%d ��%d\n",branchListStair.size(),branchList.size(),branchListTile.size());
				int loadLength = 0;
				MapPoint mp;
				if (!branchList.size() && !branchListTile.size() && !branchListStair.size()) {
					//���X�g����
					printf("��△��\n");
					break;
				}
				else if (branchList.size()) {
					//�K�i�ڑ�
					printf("�K�i�ڑ�\n");
					mp = branchList[(int)(branchList.size() * rnd(game.Rand()))];
					loadLength = (int)rnd1(game.Rand());
				}
				else if (branchListTile.size() && branchListStair.size()) {
					//���K�i
					printf("���K�i\n");
					if (rnd(game.Rand()) < TILE_JUDGE) {
						//��
						printf("��\n");
						mp = branchListTile[(int)(branchListTile.size() * rnd(game.Rand()))];
						loadLength = (int)rnd1(game.Rand());
					}
					else {
						//�K�i
						printf("�K�i\n");
						mp = branchListStair[(int)(branchListStair.size() * rnd(game.Rand()))];
						loadLength = 1;
					}
				}
				else if (branchListTile.size()) {
					//��
					printf("��\n");
					mp = branchListTile[(int)(branchListTile.size() * rnd(game.Rand()))];
					loadLength = (int)rnd1(game.Rand());
				}
				else if (branchListStair.size()) {
					//�K�i
					printf("�K�i\n");
					mp = branchListStair[(int)(branchListStair.size() * rnd(game.Rand()))];
					loadLength = 1;
				}
				else {
					//�Y���������쐬�I��
					printf("�K�i�Y�������̈׏I��\n");
					break;
				}
				printf("���̒���%d\n",loadLength);

				//1��̒��i�̒���
				for (int j = 0; j < loadLength; j++) {
					//�����X�g�ɒǉ�
					mapList.push_back(mp);
					printf("�}�b�v���X�g�ɒǉ� x: %d y: %d z: %d\n", mp.x, mp.y, mp.z);
					//�}�b�v�Ƀt���O�𗧂Ă�
					LoadGame::mapdata[mp.x][mp.y][mp.z].tile = true;
					if (mp.v == 4) {
						//���K�i
						LoadGame::mapdata[mp.x][mp.y][mp.z].stair = 1;
						LoadGame::mapdata[mp.x][mp.y + 1][mp.z].stair = 2;
						break;
					}
					else if (mp.v == 5) {
						//��K�i
						LoadGame::mapdata[mp.x][mp.y][mp.z].stair = 2;
						LoadGame::mapdata[mp.x][mp.y - 1][mp.z].stair = 1;
						break;
					}
					MapInitData mm = m[mp.v];
					bool jdg = true;
					if (MapIn(mp.x + mm.x, mp.y + mm.y, mp.z + mm.z)) {
						for (int ii = mm.x1; ii <= mm.x2; ii++) {
							for (int jj = mm.y1; jj <= mm.y2; jj++) {
								for (int kk = mm.z1; kk <= mm.z2; kk++) {
									jdg &= MapSetJudge(ii + mp.x, jj + mp.y, kk + mp.z);
									if (!jdg) {
										//���肪���s�����i�K�Ń��[�v���I��
										goto END1;
									}
								}
							}
						}
						mp.x += mm.x;
						mp.y += mm.y;
						mp.z += mm.z;
						continue;
					}
					

				END1:
					printf("��������~\n");
					break;
				}
			}
			
			for (int i = 0; i < MAP_MAX_X; i++) {
				for (int j = 0; j < MAP_MAX_Y; j++) {
					for (int k = 0; k < MAP_MAX_Z; k++) {
						if (LoadGame::mapdata[i][j][k].tile && LoadGame::mapdata[i][j][k].stair != 2) {
							GameState::EntityGroupId id = GameState::EntityGroupId_Tile;
							glm::vec3 vec({1.0f,1.0f,1.0f});
							glm::vec3 rot({ 0.0f,0.0f,0.0f });

							if (LoadGame::mapdata[i][j][k].stair == 0) {
								id = GameState::EntityGroupId_Tile;
							}
							else {
								//�K�i���i����
								if (MapInJudge(i,j,k - 1)) {
									if (MapInJudge(i + 1, j + 1, k)) {
										id = GameState::EntityGroupId_Stair2;
										vec = glm::vec3({ -1.0f,1.0f,1.0f });
									}
									else if (MapInJudge(i - 1, j + 1, k)) {
										id = GameState::EntityGroupId_Stair2;
									}
									else {
										id = GameState::EntityGroupId_Stair1;
									}
								}
								else if (MapInJudge(i + 1, j, k)) {
									rot = glm::vec3({ 0.0f,270.0f,0.0f });
									if (MapInJudge(i, j + 1, k + 1)) {
										id = GameState::EntityGroupId_Stair2;
										vec = glm::vec3({ -1.0f,1.0f,1.0f });
									}
									else if (MapInJudge(i, j + 1, k - 1)) {
										id = GameState::EntityGroupId_Stair2;
									}
									else {
										id = GameState::EntityGroupId_Stair1;
									}
								}
								else if (MapInJudge(i, j, k + 1)) {
									rot = glm::vec3({ 0.0f,180.0f,0.0f });
									if (MapInJudge(i - 1, j + 1, k)) {
										id = GameState::EntityGroupId_Stair2;
										vec = glm::vec3({ -1.0f,1.0f,1.0f });
									}
									else if (MapInJudge(i + 1, j + 1, k)) {
										id = GameState::EntityGroupId_Stair2;
									}
									else {
										id = GameState::EntityGroupId_Stair1;
									}
								}
								else if (MapInJudge(i - 1, j, k)) {
									rot = glm::vec3({ 0.0f,90.0f,0.0f });
									if (MapInJudge(i, j + 1, k - 1)) {
										id = GameState::EntityGroupId_Stair2;
										vec = glm::vec3({ -1.0f,1.0f,1.0f });
									}
									else if (MapInJudge(i, j + 1, k + 1)) {
										id = GameState::EntityGroupId_Stair2;
									}
									else {
										id = GameState::EntityGroupId_Stair1;
									}
								}
								else {
									//���i�ڑ��������ꍇ
									id = GameState::EntityGroupId_Stair1;
									if (MapInJudge(i, j + 1, k + 1)) {
										rot = glm::vec3({ 0.0f,0.0f,0.0f });
									}
									else if (MapInJudge(i + 1, j + 1, k)) {
						
										rot = glm::vec3({ 0.0f,90.0f,0.0f });
									}
									else if (MapInJudge(i, j + 1, k - 1)) {
										rot = glm::vec3({ 0.0f,180.0f,0.0f });
									}
									else if (MapInJudge(i - 1, j + 1, k)) {
										rot = glm::vec3({ 0.0f,270.0f,0.0f });
									}
								}
							}

							if (Node* tile = game.AddEntity(
								id,
								glm::vec3(i * 2, j * 2 + 0.9f , k * 2),
								"Toroid", "Res/Wall.bmp",
								nullptr)) {
								tile->Scale(vec);
								tile->Rotation(rot);
								GameEngine::world->addNode(tile);
							}
						}
					}
				}
			}
			char* str = "";
			switch (GameState::MainGame::stageno % 3) {
			case 1:
				str = "Res/Star.bmp";
				break;
			case 2:
				str = "Res/Cloud.bmp";
				break;
			case 0:
				str = "Res/SpaceSphere.bmp";
				break;
			default:
				str = "Res/SpaceSphere.bmp";
				break;
			}
			if (Node* background = game.AddEntity(
				GameState::EntityGroupId_Cube,
				glm::vec3(0.0f, 0.0f, 0.0f),
				"Toroid", str,
				nullptr,"NonLighting")) {
				background->Scale({50.0f,50.0f, 50.0f});
				background->RotateVelocity({ 0.0f,0.1f,0.0f });
				
				GameEngine::world->addNode(background);
			}

			//�������I�������t���O��L���ɂ��ăQ�[�����C����
			*flg = true;
		});
		thr.join();
	}
}