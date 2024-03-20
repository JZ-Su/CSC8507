#include "GameLevel.h"
#include "BasicExamples.h"
#include "RenderObject.h"
#include "Boss.h"
#include "OGLRenderer.h"
#include "TextureLoader.h"
using namespace NCL;

GameLevel::GameLevel(GameTechRenderer* render) : BasicExamples(render) {
	CreateGeneric();
	CreateConnectionLevel();
	CreateLevel1();
	CreateLevel2();
	CreateLevel3();
	CreateLevel4();
}

void GameLevel::CreateGeneric() {
	/*player = CreatePlayer(Vector3(0, 10, 30), Vector3(2, 2, 2), 45.0f);
	playerList.push_back(player);*/
	for (int i = 0; i < 4; i++) {
		Player* playertemp = CreatePlayer(Vector3((0 + i) * 2, 10, 30), Vector3(2, 2, 2), 45.0f);
		playerList.push_back(playertemp);
	}
	
	player = playerList[0];
	Generic.AddObject(player);
	cameraCollision = CreateCube(Vector3(), Vector3(1, 1, 1), 10.0f);
	Generic.AddObject(cameraCollision);
	//Level 4 player: 
	//Generic.AddObject(CreatePlayer(Vector3(-70, 10, -50), Vector3(1, 1, 1)));
}

void GameLevel::AddLevelToWorld(GameWorld* world, Level l) {
	for (auto element : l.objectList) {
		world->AddGameObject(element);
	}
}

void GameLevel::AddLevelToWorld(GameWorld* world, int i, bool isRotate, bool isReverse) {
	if (i > 5) i = i % 5 + 1;
	else if (i <= 1) i = 1;

	if (!isRotate) {
		if (isReverse) {
			level4_basic = level4_reverse[0];
			level4_diff  = level4_reverse[i];
		}
		else {
			level4_basic = level4_normal[0];
			level4_diff  = level4_normal[i];
		}

		beginArea = level4_basic.objectList[7];
		beginArea->isEnable = true;
		if (i == 1) {
			trueEndArea = level4_basic.objectList[12];
			trueEndArea->isEnable = false;
			falseEndArea = level4_basic.objectList[2];
			falseEndArea->isEnable = false;
		}
		else {
			trueEndArea = level4_basic.objectList[2];
			trueEndArea->isEnable = false;
			falseEndArea = level4_basic.objectList[12];
			falseEndArea->isEnable = false;
		}
		door = (Door*)level4_basic.objectList[14];
		door->Activation();
		AddLevelToWorld(world, level4_basic);
		AddLevelToWorld(world, level4_diff);
	}
	else {
		if (isReverse) {
			level4r_basic = level4_reverse_rotate[0];
			level4r_diff  = level4_reverse_rotate[i];
		}
		else {
			level4r_basic = level4_rotate[0];
			level4r_diff  = level4_rotate[i];
		}

		beginArea = level4r_basic.objectList[7];
		beginArea->isEnable = true;
		if (i == 1) {
			trueEndArea = level4r_basic.objectList[12];
			trueEndArea->isEnable = false;
			falseEndArea = level4r_basic.objectList[2];
			falseEndArea->isEnable = false;
		}
		else {
			trueEndArea = level4r_basic.objectList[2];
			trueEndArea->isEnable = false;
			falseEndArea = level4r_basic.objectList[12];
			falseEndArea->isEnable = false;
		}
		door = (Door*)level4r_basic.objectList[14];
		door->Activation();
		AddLevelToWorld(world, level4r_basic);
		AddLevelToWorld(world, level4r_diff);
	}
}

void GameLevel::RemoveLevel(GameWorld* world, Level* l, bool andClear, bool andDelete) {
	for (auto element : (*l).objectList) {
		world->RemoveGameObject(element, andDelete);
	}
	if (andClear) {
		(*l).objectList.clear();
	}
}

vector<GameObject*> GameLevel::CreatePortal(const Vector3& position) {
	vector<GameObject*> vec;
	vec.push_back(CreateCube(position + Vector3(-4, -0.5, 0), Vector3(1, 6.5, 1), 0.0f));
	vec.push_back(CreateCube(position + Vector3(4, -0.5, 0),  Vector3(1, 6.5, 1), 0.0f));
	vec.push_back(CreateCube(position + Vector3(0, 7, 0),     Vector3(5, 1, 1),   0.0f));
	vec.push_back(CreateCube(position, Vector3(4, 7, 0.1), 0.0f));
	vec.back()->SetCollisionResponse(false);
	vec.back()->GetRenderObject()->SetColour(Debug::GREEN);
	return vec; 
}

void GameLevel::CreateConnectionLevel() {
	connection.AddObject(CreateCube(Vector3(0, 0, 0),    Vector3(20, 1, 75),  0.0f));
	connection.AddObject(CreateCube(Vector3(-20, 10, 0), Vector3(10, 10, 75), 0.0f));
	connection.AddObject(CreateCube(Vector3(20, 10, 0),  Vector3(10, 10, 75), 0.0f));
	connection.AddObject(CreateCube(Vector3(0, 10, -75), Vector3(10, 10, 10), 0.0f));
	connection.AddObject(CreateCube(Vector3(0, 10, 75),  Vector3(10, 10, 10), 0.0f));

	vector<GameObject*> vec = CreatePortal(Vector3(0, 7, -65));
	for (const auto& ele : vec) {
		connection.AddObject(ele);
	}
	connection.portal = connection.objectList.back();
}

void GameLevel::CreateLevel1() {
	level1.AddObject(CreateFloor(Vector3(0, -50, 0), Vector3(200, 50, 200), 0.0f));
	level1.AddObject(CreateCeiling(Vector3(0, 62, 0), Vector3(150, 2, 150), 0.0f));
	//layers
	level1.AddObject(CreateAABB(Vector3(-80, 5, -56), Vector3(20, 5, 44), 0.0f));
	level1.AddObject(CreateAABB(Vector3(80, 5, -56), Vector3(20, 5, 44), 0));
	level1.AddObject(CreateAABB(Vector3(0, 5, 45), Vector3(40, 5, 55), 0.0f));
	level1.AddObject(CreateAABB(Vector3(0, 15, 90), Vector3(40, 5, 10), 0.0f));
	level1.AddObject(CreateAABB(Vector3(0, 25, 95), Vector3(20, 5, 5.1), 0.0f));
	level1.AddObject(CreateLayer(Vector3(0, 0, 41), Vector3(40, 10, 45), 0.0f));
	level1.AddObject(CreateLayer(Vector3(0, 10, 125), Vector3(40, 10, 40), 0.0f));
	level1.AddObject(CreateLayer(Vector3(0, 20, 112), Vector3(20, 10, 20), 0.0f));
	level1.AddObject(CreateLayer(Vector3(-100, 0, -51), Vector3(40, 10, 45), 0.0f));
	level1.AddObject(CreateLayer(Vector3(100, 0, -51), Vector3(40, 10, 45), 0.0f));
	
	CreateLevel1_Handrail();

	//wall
	level1.AddObject(CreateBigWall(Vector3(0, 30, 150), Vector3(200, 30, 50), 0.0f));
	level1.AddObject(CreateBigWall(Vector3(0, 30, -150), Vector3(200, 30, 50), 0.0f));
	level1.AddObject(CreateBigWall(Vector3(150, 30, 0), Vector3(50, 30, 200), 0.0f));
	level1.AddObject(CreateBigWall(Vector3(-150, 30, 0), Vector3(50, 30, 200), 0.0f));

	level1.AddObject(CreateBigWall(Vector3(90, 30, 97), Vector3(50, 30, 43), 0.0f));
	level1.AddObject(CreateBigWall(Vector3(-90, 30, 97), Vector3(50, 30, 43), 0.0f));
	//room
	level1.AddObject(CreateBigWall(Vector3(-20, 5, -20), Vector3(1, 5, 10), 0.0f));
	level1.AddObject(CreateBigWall(Vector3(-20, 5, -50), Vector3(1, 5, 10), 0.0f));
	level1.AddObject(CreateBigWall(Vector3(20, 5, -20), Vector3(1, 5, 10), 0.0f));
	level1.AddObject(CreateBigWall(Vector3(20, 5, -50), Vector3(1, 5, 10), 0.0f));
	level1.AddObject(CreateBigWall(Vector3(0, 5, -59), Vector3(19, 5, 1), 0.0f));
	//columns
	CreateLevel1_Columns();
	//stairs
	CreateLevel1_Stairs();
	//book shelfs
	CreateLevel1_BookShelfs();
	//small shelfs
	CreateLevel1_SmallShelfs();
	//tables
	CreateLevel1_Tables();

	level1.AddObject(CreateWallLight(Vector3(38, 40, 80), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateWallLight(Vector3(-38, 40, 80), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateWallLight(Vector3(98, 30, -90), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateWallLight(Vector3(-98, 30, -90), 0.0f, Vector3(0, 1, 0),90));
	level1.AddObject(CreateHangLight(Vector3(0, 53, 0), Vector3(5, 5, 5), 0.0f));

	//level1.AddObject(CreateLight(Vector3(0, 40, 0), Vector4(1.0f, 0.8f, 0.3f, 1.0f), 70.0f, false, true));
	level1.AddObject(CreateLight(Vector3(0, 40, 0),  Vector4(1.0f, 0.8f, 0.5f, 1.0f), 90.0f, true, true));
	level1.AddObject(CreateLight(Vector3(38, 40, 80), Vector4(0.1f, 0.05f, 0.0f, 1.0f), 25.0f, true, false));	
	level1.AddObject(CreateLight(Vector3(-38, 40, 80), Vector4(0.1f, 0.05f, 0.0f, 1.0f), 25.0f, true, false));
	level1.AddObject(CreateLight(Vector3(90, 30, -90), Vector4(0.1f, 0.05f, 0.0f, 1.0f), 70.0f, true, false));
	level1.AddObject(CreateLight(Vector3(-90, 30, -90), Vector4(0.1f, 0.05f, 0.0f, 1.0f), 70.0f, true, false));
	//level1.AddObject(CreateLight(Vector3(0, 30, 0), Vector4(1.0f, 0.8f, 0.5f, 1.0f), 70.0f, true, false));
	CreateLevel1_Coins();

	level1.AddObject(ghost = CreateGhostAI(Vector3(-85,10,-30), Vector3(5, 5, 5),player, 0.0f,Vector3(-85, 10, -20),Vector3(-85,10,-70)));
	level1.AddObject(ghost2 = CreateGhostAI(Vector3(85, 10, -30), Vector3(5, 5, 5), player, 0.0f, Vector3(85, 10, -20), Vector3(85, 10, -70)));
	vector<GameObject*> port = CreatePortal(Vector3(0, 35, 100));
	level1.AddObject(port);
	level1.portal = level1.objectList.back();
}

void GameLevel::CreateLevel2() {
	//level2.AddObject(CreateCeiling(Vector3(160, 60, 160), Vector3(240, 1, 240), 0));
	
	level2.AddObject(CreateFloor(Vector3(150, -2, 150), Vector3(200, 2, 200), 0.0f));
	level2.AddObject(CreateLight(Vector3(160, 100, 160), Vector4(1.0f, 0.8f, 0.5f, 1.0f), 300.0f, true, true));

	vector<Vector4> pixelData = render->LoadMap();
	const int size = 64;
	bool isScaned[size][size] = { false };
	vector<Vector4> wallData; // Vector4(x, z, width/length, 0: isVertical / 1: isHorizontal)
	vector<Vector3> doorData; // Vector3(x, z, rotation)
	for (int i = 0; i < size; i++) { // scan each pixel
		for (int j = 0; j < size; j++) {
			if (isScaned[i][j]) continue;

			if (pixelData[i * size + j] == Debug::BLACK && !isScaned[i][j]) {
				isScaned[i][j] = true;
			}
			// Wall
			else if (pixelData[i * size + j] == Debug::WHITE && !isScaned[i][j]) {
				isScaned[i][j] = true;
				// Horizontal
				if (pixelData[i * size + j + 1] == Debug::WHITE && !isScaned[i][j + 1]) {
					int width = 1;
					while (pixelData[i * size + j + width] == Debug::WHITE && !isScaned[i][j + width]) {
						isScaned[i][j + width] = true;
						++width;
					}
					wallData.push_back(Vector4(i + 0.5, j + (float)width / 2, width, 1));
				}
				// Vertical
				else if (pixelData[(i + 1) * size + j] == Debug::WHITE && !isScaned[i + 1][j]) {
					if (i == 56) {
						std::cout << std::endl;
					}
					int length = 1;
					while (pixelData[(i + length) * size + j] == Debug::WHITE && !isScaned[i + length][j]) {
						isScaned[i + length][j] = true;
						++length;
					}
					wallData.push_back(Vector4(i + (float)length / 2, j + 0.5, length, 0));
				}
				// One cube
				else {
					wallData.push_back(Vector4(i + 0.5, j + 0.5, 1, 0));
				}
			}
			// door, rotate -90
			else if (pixelData[i * size + j] == Debug::YELLOW && !isScaned[i][j]) {
				for (int k = 0; k < 4; k++) {
					isScaned[i][j + k] = true;
				}
				doorData.push_back(Vector3(i + 0.5, j + 2, -90));
			}
			// rotate 270
			else if (pixelData[i * size + j] == Debug::RED && !isScaned[i][j]) {
				for (int k = 0; k < 4; k++) {
					isScaned[i][j + k] = true;
				}
				doorData.push_back(Vector3(i + 0.5, j + 2, 270));
			}
			// rotate 90
			else if (pixelData[i * size + j] == Debug::MAGENTA && !isScaned[i][j]) {
				for (int k = 0; k < 4; k++) {
					isScaned[i][j + k] = true;
				}
				doorData.push_back(Vector3(i + 0.5, j + 2, 90));
			}
			// rotate -180
			else if (pixelData[i * size + j] == Debug::BLUE && !isScaned[i][j]) {
				for (int k = 0; k < 4; k++) {
					isScaned[i + k][j] = true;
				}
				doorData.push_back(Vector3(i + 2, j + 0.5, -180));
			}
			// rotate 180
			else if (pixelData[i * size + j] == Debug::GREEN && !isScaned[i][j]) {
				for (int k = 0; k < 4; k++) {
					isScaned[i + k][j] = true;
				}
				doorData.push_back(Vector3(i + 2, j + 0.5, 180));
			}
		}
	}

	int height = 10;
	for (auto element : wallData) {
		if (element.w == 1) {
			level2.AddObject(CreateGreenWall(Vector3(element.x * 5, height, element.y * 5), Vector3(2.5, height, element.z * 2.5), 0.0f));
		}
		else {
			level2.AddObject(CreateGreenWall(Vector3(element.x * 5, height, element.y * 5), Vector3(element.z * 2.5, height, 2.5), 0.0f));
		}
	}
	for (auto element : doorData) {
		Door* door = CreateDoor(Vector3(element.x * 5, height, element.y * 5), Vector3(4 * 2.5, height, 1 * 2.5), 0.0f, element.z, 30.0);
		l2_Doors.push_back(door);
		level2.AddObject(door);
	}
	level2.AddObject(CreateBigWall(Vector3(0, 20, 160), Vector3(5, 40, 160), 0.0f));
	level2.AddObject(CreateBigWall(Vector3(320, 20, 160), Vector3(5, 40, 160), 0.0f));
	level2.AddObject(CreateBigWall(Vector3(160, 20, -5), Vector3(160, 40, 5), 0.0f));
	level2.AddObject(CreateBigWall(Vector3(160, 20, 315), Vector3(160, 40, 5), 0.0f));

	vector<GameObject*>port = CreatePortal(Vector3(300, 7, 0));
	level2.AddObject(port);
	level2.portal = level2.objectList.back();
}

void GameLevel::CreateLevel3() {
	//level3.AddObject(CreateLight(Vector3(0, 40, 0), Vector4(1.0f, 0.8f, 0.3f, 1.0f), 130.0f, false, true));
	level3.AddObject(CreateLight(Vector3(0, 30, 0), Vector4(1.0f, 0.8f, 0.5f, 1.0f), 90.0f, true, true));

	level3.AddObject(CreateCeiling(Vector3(0, 62, 0), Vector3(150, 2, 150), 0.0f));
	level3.AddObject(CreateHangLight(Vector3(0, 53, 0), Vector3(5, 5, 5), 0.0f));

	// boss = CreateBoss(Vector3(0, -2, -60), Vector3(10, 10, 10), player, 0.0f);
	// level3.AddObject(CreateCube(Vector3(0, -2, 0), Vector3(100, 2, 100), 0.0f));
	//level3.AddObject(CreateCube(Vector3(70, 0, 77), Vector3(10, 10,5 ), 0.0f));
	level3.AddObject(CreateBigWall(Vector3(150, 30, 0), Vector3(50, 30, 200), 0.0f));
	level3.AddObject(CreateBigWall(Vector3(-150, 30, 0), Vector3(50, 30, 200), 0.0f));
	level3.AddObject(CreateBigWall(Vector3(0, 30, -150), Vector3(200, 30, 50), 0.0f));
	level3.AddObject(CreateBigWall(Vector3(0, 30, 150), Vector3(200, 30, 50), 0.0f));
	//level3.AddObject(CreateCube(Vector3(0, 0, 0), Vector3(5, 5, 5), 0.0f));

	//player = CreateSphere(Vector3(20, 5, 20), 5, 0.0f);
	//level3.AddObject(player);
	//testAI = CreateAItest(Vector3(0, 0, 0), Vector3(5, 5, 5), player, 0.0f);
	//level3.AddObject(testAI);
	level3.AddObject(CreateFloor(Vector3(0, -40, 0), Vector3(200, 40, 200), 0.0f));
	//level3.AddObject(boss=CreateBoss(Vector3(0, -2, -60), Vector3(10, 10, 10), 0.0f));
	//BossBehaviourTree(boss,player);
	//
	boss = CreateBoss(Vector3(0, 1, -40), Vector3(6, 6, 6), player, 0.01f);
	iceCubeBullet = CreateIceCubeBullet(Vector3(20, -98, 0), Vector3(3, 3, 3), 5.0f);
	fireBallBullet = CreateFireBallBullet(Vector3(0, -98, 0), 2, 5.0f);

	level3.AddObject(boss);
	level3.AddObject(iceCubeBullet);
	level3.AddObject(fireBallBullet);
	level3.AddObject(shield = CreateShield(Vector3(0, -30, 20), Vector3(1, 1, 1), 5.0f));
	// level3.AddObject(boss);
	/*static_cast<Boss*>(boss)->NCL::CSC8503::Boss::BossBehaviourTree(player);*/

	level3.AddObject(CreatRedbottle(Vector3(10, -2, 10), Vector3(3, 3, 3), 0.0f));

	vector<GameObject*> port = CreatePortal(Vector3(90, 7, 90));
	level3.AddObject(port);
	level3.portal = level3.objectList.back();
}

void GameLevel::CreateLevel4() {
	CreateLevel4_Normal();
	CreateLevel4_Reverse();
	CreateLevel4_Rotate();
	CreateLevel4_RR();
}

void GameLevel::CreateLevel4_Normal() {
	// ... <-
	// .
	// .
	// .
	// .
	// ...     .->
	//   ....... 
	Vector3 floorDimensions = Vector3(10, 1, 10);
	Level l0;
	//floor
	l0.AddObject(CreateCube(Vector3(-50, -0.02, -50), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-70, -0.02, -50), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-70, -0.02, -30), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-70, -0.02, -10), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-70, -0.02, 10), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-70, -0.02, 30), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-70, -0.02, 50), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-50, -0.02, 50), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-50, -0.02, 70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-30, -0.02, 70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-10, -0.02, 70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(10,  -0.02, 70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(30,  -0.02, 70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(50,  -0.02, 70), floorDimensions, 0.0f));
	//door
	l0.AddObject(CreateDoor(Vector3(-60, 5, 50), Vector3(10, 10, 1), 0.0f, -90, 20));
	//wall
	l0.AddObject(CreateBigWall(Vector3(-70, 10, -70), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(-90, 10, -50), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(-90, 10,   0), Vector3(10, 20, 40), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(-90, 10,  50), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(-50, 10,   0), Vector3(10, 20, 40), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(-70, 10,  70), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(-50, 10,  90), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(  0, 10,  50), Vector3(40, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(  0, 10,  90), Vector3(40, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3( 50, 10,  90), Vector3(10, 20, 10), 0.0f));

	l0.AddObject(CreateFloor(Vector3(0, -0.1, 0), Vector3(100, 1, 100)));
	l0.AddObject(CreateLight(Vector3(0, 19, 65), Vector4(1.0f, 0.8f, 0.5f, 1.0f), 50.0f, true, true));
	l0.AddObject(CreateLight(Vector3(-65, 19, 0), Vector4(0.2f, 0.1f, 0.1f, 1.0f), 50.0f, true, false));
	level4_normal.emplace_back(l0);
	
	Level l1;
	level4_normal.emplace_back(l1);

	Level l2;
	//other objects
	l2.AddObject(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_normal.emplace_back(l2);

	Level l3;
	//other objects
	l3.AddObject(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_normal.emplace_back(l3);

	Level l4;
	//other objects
	l4.AddObject(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_normal.emplace_back(l4);

	Level l5;
	//other objects
	l5.AddObject(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_normal.emplace_back(l5);
}

void GameLevel::CreateLevel4_Rotate() {
	//    .......
	// <- .     ...
	//            .
	//            .
	//            .
	//            .
	//       -> ...
	Vector3 floorDimensions = Vector3(10, 1, 10);
	Level l0;
	//floor
	l0.AddObject(CreateCube(Vector3(50, -0.02, 50), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(70, -0.02, 50), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(70, -0.02, 30), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(70, -0.02, 10), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(70, -0.02, -10), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(70, -0.02, -30), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(70, -0.02, -50), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(50, -0.02, -50), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(50, -0.02, -70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(30, -0.02, -70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(10, -0.02, -70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-10,-0.02, -70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-30,-0.02, -70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-50,-0.02, -70), floorDimensions, 0.0f));
	//door
	l0.AddObject(CreateDoor(Vector3(60, 5, -50), Vector3(10, 10, 1), 0.0f, -270, 20));
	//wall
	l0.AddObject(CreateBigWall(Vector3(70, 10, 70),  Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(90, 10, 50),  Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(90, 10, 0),   Vector3(10, 20, 40), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(90, 10, -50), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(70, 10, -70), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(50, 10, 0),	 Vector3(10, 20, 40), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(0,  10, -50), Vector3(40, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(50, 10, -90), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(0,  10, -90), Vector3(40, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(-50,10, -90), Vector3(10, 20, 10), 0.0f));

	l0.AddObject(CreateFloor(Vector3(0, -0.1, 0), Vector3(100, 1, 100)));
	l0.AddObject(CreateLight(Vector3(0, 19, -65), Vector4(1.0f, 0.8f, 0.5f, 1.0f), 50.0f, true, true));
	l0.AddObject(CreateLight(Vector3(65, 19, 0), Vector4(0.2f, 0.1f, 0.1f, 1.0f), 50.0f, true, false));
	level4_rotate.emplace_back(l0);

	Level l1;
	level4_rotate.emplace_back(l1);

	Level l2;
	//other objects
	l2.AddObject(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_rotate.emplace_back(l2);

	Level l3;
	//other objects
	l3.AddObject(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_rotate.emplace_back(l3);

	Level l4;
	//other objects
	l4.AddObject(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_rotate.emplace_back(l4);

	Level l5;
	//other objects
	l5.AddObject(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_rotate.emplace_back(l5);
}

void GameLevel::CreateLevel4_Reverse() {
	// ... ->
	// .
	// .
	// .
	// .  
	// ...     .<-
	//   .......
	Vector3 floorDimensions = Vector3(10, 1, 10);
	Level l0;
	//floor
	l0.AddObject(CreateCube(Vector3(-50, -0.02, -50), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(50,  -0.02, 70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(30,  -0.02, 70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(10,  -0.02, 70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-10, -0.02, 70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-30, -0.02, 70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-50, -0.02, 70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-50, -0.02, 50), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-70, -0.02, 50), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-70, -0.02, 30), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-70, -0.02, 10), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-70, -0.02, -10), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-70, -0.02, -30), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-70, -0.02, -50), floorDimensions, 0.0f));
	//door
	l0.AddObject(CreateDoor(Vector3(-50, 5, 60), Vector3(10, 10, 1), 0, 180, 20));
	//wall
	l0.AddObject(CreateBigWall(Vector3(-70, 10, -70), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(-90, 10, -50), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(-90, 10, 0),  Vector3(10, 20, 40), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(-90, 10, 50), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(-50, 10, 0),  Vector3(10, 20, 40), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(-70, 10, 70), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(-50, 10, 90), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(0,   10, 50), Vector3(40, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(0,   10, 90), Vector3(40, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(50,  10, 90), Vector3(10, 20, 10), 0.0f));

	l0.AddObject(CreateFloor(Vector3(0, -0.1, 0), Vector3(100, 1, 100)));
	l0.AddObject(CreateLight(Vector3(-65, 19, 0), Vector4(1.0f, 0.8f, 0.5f, 1.0f), 50.0f, true, true));
	l0.AddObject(CreateLight(Vector3(0, 19, 65), Vector4(0.2f, 0.1f, 0.1f, 1.0f), 50.0f, true, false));

	level4_reverse.emplace_back(l0);

	Level l1;
	level4_reverse.emplace_back(l1);

	Level l2;
	//other objects
	l2.AddObject(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_reverse.emplace_back(l2);

	Level l3;
	//other objects
	l3.AddObject(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_reverse.emplace_back(l3);

	Level l4;
	//other objects
	l4.AddObject(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_reverse.emplace_back(l4);

	Level l5;
	//other objects
	l5.AddObject(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_reverse.emplace_back(l5);

}

void GameLevel::CreateLevel4_RR() {
	//    .......
	// -> .     ...
	//            .
	//            .
	//            .
	//            .
	//       <- ...
	Vector3 floorDimensions = Vector3(10, 1, 10);

	Level l0;
	//floor
	l0.AddObject(CreateCube(Vector3(50, -0.02, 50), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-50,-0.02, -70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-30,-0.02, -70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(-10,-0.02, -70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(10, -0.02, -70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(30, -0.02, -70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(50, -0.02, -70), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(50, -0.02, -50), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(70, -0.02, -50), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(70, -0.02, -30), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(70, -0.02, -10), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(70, -0.02, 10), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(70, -0.02, 30), floorDimensions, 0.0f));
	l0.AddObject(CreateCube(Vector3(70, -0.02, 50), floorDimensions, 0.0f));
	//door
	l0.AddObject(CreateDoor(Vector3(50, 5, -60), Vector3(10, 10, 1), 0, 0, 20));
	//wall
	l0.AddObject(CreateBigWall(Vector3(70, 10, 70), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(90, 10, 50), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(90, 10, 0),  Vector3(10, 20, 40), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(90, 10, -50), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(70, 10, -70), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(50, 10, 0),   Vector3(10, 20, 40), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(50, 10, -90), Vector3(10, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(0,  10, -50), Vector3(40, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(0,  10, -90), Vector3(40, 20, 10), 0.0f));
	l0.AddObject(CreateBigWall(Vector3(-50,10, -90), Vector3(10, 20, 10), 0.0f));

	l0.AddObject(CreateFloor(Vector3(0, -0.1, 0), Vector3(100, 1, 100)));
	l0.AddObject(CreateLight(Vector3(0, 19, -65),  Vector4(0.2f, 0.1f, 0.1f, 1.0f), 50.0f, true, false));
	l0.AddObject(CreateLight(Vector3(65, 19, 0), Vector4(1.0f, 0.8f, 0.5f, 1.0f), 50.0f, true, true));

	level4_reverse_rotate.emplace_back(l0);

	Level l1;
	level4_reverse_rotate.emplace_back(l1);

	Level l2;
	//other objects
	l2.AddObject(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_reverse_rotate.emplace_back(l2);

	Level l3;
	//other objects
	l3.AddObject(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_reverse_rotate.emplace_back(l3);

	Level l4;
	//other objects
	l4.AddObject(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_reverse_rotate.emplace_back(l4);

	Level l5;
	//other objects
	l5.AddObject(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_reverse_rotate.emplace_back(l5);
}

void GameLevel::CreateLevel1_Tables() {
	level1.AddObject(CreateTable(Vector3(20, 13, 70), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateAABB(Vector3(20, 11.5, 70), Vector3(2, 1.6, 6), 0.0f));

	level1.AddObject(CreateTable(Vector3(-20, 13, 70), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateAABB(Vector3(-20, 11.5, 70), Vector3(2, 1.5, 6), 0.0f));
	level1.AddObject(CreateTable(Vector3(15, 13, 33), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateAABB(Vector3(15, 11.5, 33), Vector3(2, 1.5, 6), 0.0f));
	level1.AddObject(CreateTable(Vector3(-15, 13, 33), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateAABB(Vector3(-15, 11.5, 33), Vector3(2, 1.5, 6), 0.0f));
	level1.AddObject(CreateTable(Vector3(15, 13, 3), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateAABB(Vector3(15, 11.5, 3), Vector3(2, 1.5, 6), 0.0f));
	level1.AddObject(CreateTable(Vector3(-15, 13, 3), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateAABB(Vector3(-15, 11.5, 3), Vector3(2, 1.5, 6), 0.0f));

	level1.AddObject(CreateTable(Vector3(-35, 3, -50), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateAABB(Vector3(-35, 1.5, -50), Vector3(2, 1.5, 6), 0.0f));
	level1.AddObject(CreateTable(Vector3(-10, 3, -50), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateAABB(Vector3(-10, 1.5, -50), Vector3(2, 1.5, 6), 0.0f));
	level1.AddObject(CreateTable(Vector3(10, 3, -50), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateAABB(Vector3(10, 1.5, -50), Vector3(2, 1.5, 6), 0.0f));
	level1.AddObject(CreateTable(Vector3(-10, 3, -20), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateAABB(Vector3(-10, 1.5, -20), Vector3(2, 1.5, 6), 0.0f));
	level1.AddObject(CreateTable(Vector3(10, 3, -20), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateAABB(Vector3(10, 1.5, -20), Vector3(2, 1.5, 6), 0.0f));

	level1.AddObject(CreateTable(Vector3(90, 3, 35), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateAABB(Vector3(90, 1.5, 35), Vector3(2, 1.5, 6), 0.0f));
	level1.AddObject(CreateTable(Vector3(68, 13, -83), 0.0f));
	level1.AddObject(CreateAABB(Vector3(68, 11.5, -83), Vector3(6, 1.5, 2), 0.0f));
	level1.AddObject(CreateTable(Vector3(68, 13, -69), 0.0f));
	level1.AddObject(CreateAABB(Vector3(68, 11.5, -69), Vector3(6, 1.5, 2), 0.0f));
	level1.AddObject(CreateTable(Vector3(68, 13, -43), 0.0f));
	level1.AddObject(CreateAABB(Vector3(68, 11.5, -43), Vector3(6, 1.5, 2), 0.0f));
	level1.AddObject(CreateTable(Vector3(68, 13, -29), 0.0f));
	level1.AddObject(CreateAABB(Vector3(68, 11.5, -29), Vector3(6, 1.5, 2), 0.0f));
	level1.AddObject(CreateTable(Vector3(-68, 13, -83), 0.0f));
	level1.AddObject(CreateAABB(Vector3(-68, 11.5, -83), Vector3(6, 1.5, 2), 0.0f));
	level1.AddObject(CreateTable(Vector3(-68, 13, -69), 0.0f));
	level1.AddObject(CreateAABB(Vector3(-68, 11.5, -69), Vector3(6, 1.5, 2), 0.0f));
	level1.AddObject(CreateTable(Vector3(-68, 13, -43), 0.0f));
	level1.AddObject(CreateAABB(Vector3(-68, 11.5, -43), Vector3(6, 1.5, 2), 0.0f));
	level1.AddObject(CreateTable(Vector3(-68, 13, -29), 0.0f));
	level1.AddObject(CreateAABB(Vector3(-68, 11.5, -29), Vector3(6, 1.5, 2), 0.0f));
	level1.AddObject(CreateTable(Vector3(-98, 13, -78), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateAABB(Vector3(-98, 11.5, -78), Vector3(2, 1.5, 6), 0.0f));
}

void GameLevel::CreateLevel1_SmallShelfs() {
	level1.AddObject(CreateBookshelf(Vector3(-44, -6, 0), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-52, -6, 0), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-38, -6, -60), 0.0f));

	level1.AddObject(CreateBookshelf(Vector3(46, -6, -45), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(54, -6, -45), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(38, -6, -45), 0.0f));

	level1.AddObject(CreateBookshelf(Vector3(66, 4, -56), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(74, 4, -56), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-66, 4, -56), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-74, 4, -56), 0.0f));
}

void GameLevel::CreateLevel1_BookShelfs() {
	level1.AddObject(CreateBookshelf(Vector3(0, 10, 60), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(8, 10, 60), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-8, 10, 60), 0.0f));

	level1.AddObject(CreateBookshelf(Vector3(0, 10, 20), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(8, 10, 20), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-8, 10, 20), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-25, 10, 36), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-25, 10, 28), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(25, 10, 36), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(25, 10, 28), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-25, 10, 8), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-25, 10, 0), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(25, 10, 8), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(25, 10, 0), 0.0f, Vector3(0, 1, 0), 90));

	level1.AddObject(CreateBookshelf(Vector3(-52, 0, 33), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-44, 0, 33), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-60, 0, 33), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-96, 0, 33), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-88, 0, 33), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-61, 0, -8), 0.0f));

	level1.AddObject(CreateBookshelf(Vector3(-50, 0, -28), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-50, 0, -44), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-50, 0, -52), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-30, 0, -60), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-39, 0, -15), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-39, 0, -23), 0.0f, Vector3(0, 1, 0), 90));

	level1.AddObject(CreateBookshelf(Vector3(-24, 0, -96), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-24, 0, -88), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(24, 0, -96), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(24, 0, -88), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-50, 0, -96), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-50, 0, -88), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(0, 0, -16), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(0, 0, -24), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(0, 0, -44), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(0, 0, -52), 0.0f, Vector3(0, 1, 0), 90));

	level1.AddObject(CreateBookshelf(Vector3(50, 0, -96), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(50, 0, -88), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(50, 0, -80), 0.0f, Vector3(0, 1, 0), 90));

	level1.AddObject(CreateBookshelf(Vector3(61, 0, -8), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(61, 0, 0), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(61, 0, 8), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(71, 0, 22), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(71, 0, 30), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(71, 0, 38), 0.0f, Vector3(0, 1, 0), 90));

	level1.AddObject(CreateBookshelf(Vector3(66, 10, -96), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(74, 10, -96), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(66, 10, -17), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-66, 10, -96), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-74, 10, -96), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-66, 10, -17), 0.0f));

	level1.AddObject(CreateBookshelf(Vector3(90, 10, -86), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(90, 10, -78), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(90, 10, -70), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(90, 10, -44), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(90, 10, -36), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(90, 10, -28), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-90, 10, -86), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-90, 10, -78), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-90, 10, -70), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-90, 10, -44), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-90, 10, -36), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-90, 10, -28), 0.0f, Vector3(0, 1, 0), 90));
}

void GameLevel::CreateLevel1_Columns() {
	level1.AddObject(CreateColumn(Vector3(-50, 0, -60), 0.0f));
	level1.AddObject(CreateCapsule(Vector3(-50, 20, -60), 60, 4, 0.0f));

	level1.AddObject(CreateColumn(Vector3(-50, 0, -20), 0.0f));
	level1.AddObject(CreateCapsule(Vector3(-50, 20, -20), 60, 4, 0.0f));

	level1.AddObject(CreateColumn(Vector3(-50, 0, 20), 0.0f));
	level1.AddObject(CreateCapsule(Vector3(-50, 20, 20), 60, 4, 0.0f));

	level1.AddObject(CreateColumn(Vector3(50, 0, -60), 0.0f));
	level1.AddObject(CreateCapsule(Vector3(50, 20, -60), 60, 4, 0.0f));

	level1.AddObject(CreateColumn(Vector3(50, 0, -20), 0.0f));
	level1.AddObject(CreateCapsule(Vector3(50, 20, -20), 60, 4, 0.0f));

	level1.AddObject(CreateColumn(Vector3(50, 0, 20), 0.0f));
	level1.AddObject(CreateCapsule(Vector3(50, 20, 20), 60, 4, 0.0f));
}

void GameLevel::CreateLevel1_Stairs() {
	level1.AddObject(CreateStairs(Vector3(30, 20, 95), Vector3(1.25, 2, 1.25), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateCubeOBB(Vector3(20, 15.9, 95), Vector3(10, 10, 5), 0.0f, Vector3(0, 0, 1), -45));
	level1.AddObject(CreateStairs(Vector3(-30, 20, 95), Vector3(1.25, 2, 1.25), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateCubeOBB(Vector3(-20, 15.9, 95), Vector3(10, 10, 5), 0.0f, Vector3(0, 0, 1), 45));

	level1.AddObject(CreateStairs(Vector3(35, 10, 64), Vector3(1.25, 2, 2), 0.0f, Vector3(0, 1, 0), 180));
	level1.AddObject(CreateCubeOBB(Vector3(35, 6.35, 76.4), Vector3(5, 10, 10), 0.0f, Vector3(1, 0, 0), -30));
	level1.AddObject(CreateStairs(Vector3(-35, 10, 64), Vector3(1.25, 2, 2), 0.0f, Vector3(0, 1, 0), 180));
	level1.AddObject(CreateCubeOBB(Vector3(-35, 6.35, 76.4), Vector3(5, 10, 10), 0.0f, Vector3(1, 0, 0), -30));

	level1.AddObject(CreateStairs(Vector3(56, 0, 44), Vector3(2.5, 2, 2), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateCubeOBB(Vector3(43.7, -3.7, 44), Vector3(10, 10, 10), 0.0f, Vector3(0, 0, 1), -30));
	level1.AddObject(CreateStairs(Vector3(-56, 0, 44), Vector3(2.5, 2, 2), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateCubeOBB(Vector3(-43.7, -3.7, 44), Vector3(10, 10, 10), 0.0f, Vector3(0, 0, 1), 30));
	level1.AddObject(CreateStairs(Vector3(  83, 0, 4), Vector3(2.5, 2, 2), 0.0f));
	level1.AddObject(CreateCubeOBB(Vector3( 83, -3.7, -8.3), Vector3(10, 10, 10), 0.0f, Vector3(1, 0, 0), 30));
	level1.AddObject(CreateStairs(Vector3( -83, 0, 4), Vector3(2.5, 2, 2), 0.0f));
	level1.AddObject(CreateCubeOBB(Vector3(-83, -3.7, -8.3), Vector3(10, 10, 10), 0.0f, Vector3(1, 0, 0), 30));
}

void GameLevel::CreateLevel1_Handrail() {
	level1.AddObject(CreateHandrail(Vector3(8.2, 33.5, 91), 0.0f, Vector3(0, 1, 0), 0));
	level1.AddObject(CreateHandrail(Vector3(-8.2, 33.5, 91), 0.0f, Vector3(0, 1, 0), 180));
	level1.AddObject(CreateHandrail(Vector3(8, 23.5, 81), 0.0f, Vector3(0, 1, 0), 0));
	level1.AddObject(CreateHandrail(Vector3(24, 23.5, 81), 0.0f, Vector3(0, 1, 0), 0));
	level1.AddObject(CreateHandrail(Vector3(-8, 23.5, 81), 0.0f, Vector3(0, 1, 0), 180));
	level1.AddObject(CreateHandrail(Vector3(-24, 23.5, 81), 0.0f, Vector3(0, 1, 0), 180));
	level1.AddObject(CreateHandrail(Vector3(16, 13.5, -8), 0.0f, Vector3(0, 1, 0), 0));
	level1.AddObject(CreateHandrail(Vector3(0, 13.5, -8), 0.0f, Vector3(0, 1, 0), 0));
	level1.AddObject(CreateHandrail(Vector3(31.5, 13.5, -8), 0.0f, Vector3(0, 1, 0), 0));
	level1.AddObject(CreateHandrail(Vector3(-16, 13.5, -8), 0.0f, Vector3(0, 1, 0), 180));
	level1.AddObject(CreateHandrail(Vector3(-31.5, 13.5, -8), 0.0f, Vector3(0, 1, 0), 180));
	level1.AddObject(CreateHandrail(Vector3(-38.5, 13.5, 0), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(-38.5, 13.5, 16), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(-38.5, 13.5, 29), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(38.5, 13.5, 0), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(38.5, 13.5, 16), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(38.5, 13.5, 29), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(-98, 13.5, -13), 0.0f, Vector3(0, 1, 0), 0));
	level1.AddObject(CreateHandrail(Vector3(-68, 13.5, -13), 0.0f, Vector3(0, 1, 0), 180));
	level1.AddObject(CreateHandrail(Vector3(-61, 13.5, -20), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(-61, 13.5, -36), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(-61, 13.5, -52), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(-61, 13.5, -68), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(-61, 13.5, -84), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(-61, 13.5, -98), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(98, 13.5, -13), 0.0f, Vector3(0, 1, 0), 180));
	level1.AddObject(CreateHandrail(Vector3(68, 13.5, -13), 0.0f, Vector3(0, 1, 0), 0));
	level1.AddObject(CreateHandrail(Vector3(61, 13.5, -20), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(61, 13.5, -36), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(61, 13.5, -52), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(61, 13.5, -68), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(61, 13.5, -84), 0.0f, Vector3(0, 1, 0), -90));
	level1.AddObject(CreateHandrail(Vector3(61, 13.5, -98), 0.0f, Vector3(0, 1, 0), -90));
}


void GameLevel::CreateLevel1_Coins() {
	Vector3 dimensions = Vector3(0.5, 0.5, 0.5);

	coinList.push_back(CreateCoin(Vector3( 95, 5,  -5), dimensions));
	//coinList.push_back(CreateCoin(Vector3( 80, 5,  35), dimensions));
	//coinList.push_back(CreateCoin(Vector3( 55, 5, -90), dimensions));
	//coinList.push_back(CreateCoin(Vector3( 50, 5,   0), dimensions));
	//coinList.push_back(CreateCoin(Vector3( 37, 5, -90), dimensions));
	//coinList.push_back(CreateCoin(Vector3( 32, 5, -58), dimensions));
	//coinList.push_back(CreateCoin(Vector3( 20, 5, -35), dimensions));
	//coinList.push_back(CreateCoin(Vector3(  0, 5, -70), dimensions));
	//coinList.push_back(CreateCoin(Vector3(-40, 5, -90), dimensions));
	//coinList.push_back(CreateCoin(Vector3(-43, 5, -50), dimensions));
	//coinList.push_back(CreateCoin(Vector3(-90, 5,  40), dimensions));

	//coinList.push_back(CreateCoin(Vector3( 20, 15,  20), dimensions));
	//coinList.push_back(CreateCoin(Vector3(  0, 15,  70), dimensions));
	//coinList.push_back(CreateCoin(Vector3(  0, 15,  35), dimensions));
	//coinList.push_back(CreateCoin(Vector3(  0, 15,  -5), dimensions));
	//coinList.push_back(CreateCoin(Vector3(-30, 15,   5), dimensions));

	//coinList.push_back(CreateCoin(Vector3(-67, 15, -35), dimensions));
	//coinList.push_back(CreateCoin(Vector3(-67, 15, -90), dimensions));
	//coinList.push_back(CreateCoin(Vector3(-90, 15, -55), dimensions));
	//coinList.push_back(CreateCoin(Vector3(-95, 15, -35), dimensions));

	//coinList.push_back(CreateCoin(Vector3( 68, 15, -35), dimensions));
	//coinList.push_back(CreateCoin(Vector3( 68, 15, -75), dimensions));
	//coinList.push_back(CreateCoin(Vector3( 95, 15, -35), dimensions));

	level1.AddObject(coinList);
}

bool GameLevel::CheckCoinList() {
	for (auto element : coinList) {
		if (element->IsActive()) return false;
	}
	return true;
}

