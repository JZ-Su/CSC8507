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
	player = CreatePlayer(Vector3(0, 10, 6), Vector3(2, 2, 2), 15.0f);
	Generic.AddObject(player);
	//Level 4 player: 
	//Generic.AddObject(CreatePlayer(Vector3(-70, 10, -50), Vector3(1, 1, 1)));
}

void GameLevel::AddLevelToWorld(GameWorld* world, Level l) {
	for (auto element : l.objectList) {
		world->AddGameObject(element);
	}
}

void GameLevel::AddLevelToWorld(GameWorld* world, int i, bool isRotate, bool isReverse) {
	if (i > 5) {
		i = i % 5;
	}
	else if (i < 0) {
		i = 0;
	}
	if (!isRotate) {
		if (isReverse) level4 = level4_reverse[i];
		else level4 = level4_normal[i];

		beginArea = level4.objectList[8];
		if (i == 0) {
			trueEndArea = level4.objectList[13];
			trueEndArea->isEnable = false;
			falseEndArea = level4.objectList[3];
			falseEndArea->isEnable = false;
		}
		else {
			trueEndArea = level4.objectList[3];
			trueEndArea->isEnable = false;
			falseEndArea = level4.objectList[13];
			falseEndArea->isEnable = false;
		}
		AddLevelToWorld(world, level4);
	}
	else {
		if (isReverse) level4r = level4_reverse_rotate[i];
		else level4r = level4_rotate[i];

		beginArea = level4r.objectList[8];
		if (i == 0) {
			trueEndArea = level4r.objectList[13];
			trueEndArea->isEnable = false;
			falseEndArea = level4r.objectList[3];
			falseEndArea->isEnable = false;
		}
		else {
			trueEndArea = level4r.objectList[3];
			trueEndArea->isEnable = false;
			falseEndArea = level4r.objectList[13];
			falseEndArea->isEnable = false;
		}

		AddLevelToWorld(world, level4r);
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
	level1.AddObject(CreateFloor(Vector3(0, -2, 0), Vector3(100, 2, 100), 0.0f));
	level1.AddObject(CreateCeiling(Vector3(0, 62, 0), Vector3(150, 2, 150), 0.0f));
	//layers
	level1.AddObject(CreateLayer(Vector3(-80, 5, -56), Vector3(20, 5, 44), 0.0f));
	//level1.AddObject(CreateLayer(Vector3(0, -5, 0), Vector3(10, 20, 10), 0.0f));
	level1.AddObject(CreateCube(Vector3(80, 5, -56), Vector3(20, 5, 44), 0));
	level1.AddObject(CreateCube(Vector3(0, 5, 45), Vector3(40, 5, 55), 0.0f));
	level1.AddObject(CreateCube(Vector3(0, 15, 90), Vector3(40, 5, 10), 0.0f));
	level1.AddObject(CreateCube(Vector3(0, 25, 95), Vector3(20, 5, 5.1), 0.0f));
	//wall
	level1.AddObject(CreateCube(Vector3(0, 30, 101), Vector3(100, 30, 1), 0.0f));
	level1.AddObject(CreateCube(Vector3(0, 30, -101), Vector3(100, 30, 1), 0.0f));
	level1.AddObject(CreateCube(Vector3(101, 30, 0), Vector3(1, 30, 100), 0.0f));
	level1.AddObject(CreateCube(Vector3(-101, 30, 0), Vector3(1, 30, 100), 0.0f));
	level1.AddObject(CreateCube(Vector3(70, 30, 77), Vector3(30, 30, 23), 0.0f));
	level1.AddObject(CreateCube(Vector3(-70, 30, 77), Vector3(30, 30, 23), 0.0f));
	//room
	level1.AddObject(CreateCube(Vector3(-20, 5, -20), Vector3(1, 5, 10), 0.0f));
	level1.AddObject(CreateCube(Vector3(-20, 5, -50), Vector3(1, 5, 10), 0.0f));
	level1.AddObject(CreateCube(Vector3(20, 5, -20), Vector3(1, 5, 10), 0.0f));
	level1.AddObject(CreateCube(Vector3(20, 5, -50), Vector3(1, 5, 10), 0.0f));
	level1.AddObject(CreateCube(Vector3(0, 5, -59), Vector3(19, 5, 1), 0.0f));
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

	level1.AddObject(ghost = CreateGhost(Vector3(10, 10, 0), Vector3(5, 5, 5), 0.0f));

	level1.AddObject(CreateCoin(Vector3(20, 10, -35), Vector3(0.5, 0.5, 0.5), 0.0f, 0.0f));

	vector<GameObject*> port = CreatePortal(Vector3(0, 35, 100));
	for (const auto& ele : port) {
		level1.AddObject(ele);
	}
	level1.portal = level1.objectList.back();
}

void GameLevel::CreateLevel2() {
	level2.AddObject(CreateCube(Vector3(150, -2, 150), Vector3(150, 2, 150), 0.0f));

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
					wallData.push_back(Vector4(i+0.5, j + (float)width / 2, width, 1));
				}
				// Vertical
				else if (pixelData[(i + 1) * size + j] == Debug::WHITE && !isScaned[i + 1][j]) {
					int length = 1;
					while (pixelData[(i + length) * size + j] == Debug::WHITE && !isScaned[i + length][j]) {
						isScaned[i + length][j] = true;
						++length;
					}
					wallData.push_back(Vector4(i + (float)length / 2, j+0.5, length, 0));
				}
				// One cube
				else {
					wallData.push_back(Vector4(i + 0.5, j + 0.5, 1, 0));
				}
			}
			// door, rotate 0
			else if (pixelData[i * size + j] == Debug::GREEN && !isScaned[i][j]) {
				for (int k = 0; k < 3; k++) {
					isScaned[i + k][j] = true;
				}
				doorData.push_back(Vector3(i + 1.5, j + 0.5, 0));
			}
			// rotate 90
			else if (pixelData[i * size + j] == Debug::RED && !isScaned[i][j]) {
				for (int k = 0; k < 3; k++) {
					isScaned[i][j + k] = true;
				}
				doorData.push_back(Vector3(i + 0.5, j + 1.5, 90));
			}
			// rotate 180
			else if (pixelData[i * size + j] == Debug::BLUE && !isScaned[i][j]) {
				for (int k = 0; k < 3; k++) {
					isScaned[i + k][j] = true;
				}
				doorData.push_back(Vector3(i + 1.5, j + 0.5, 180));
			}
			// rotate 270
			else if (pixelData[i * size + j] == Debug::YELLOW && !isScaned[i][j]) {
				for (int k = 0; k < 3; k++) {
					isScaned[i][j + k] = true;
				}
				doorData.push_back(Vector3(i + 0.5, j + 1.5, 270));
			}
		}
	}

	int height = 10;
	for (auto element : wallData) {
		if (element.w == 1) {
			level2.AddObject(CreateCube(Vector3(element.x *5, height, element.y*5), Vector3(2.5, height, element.z*2.5), 0.0f));
		}
		else {
			level2.AddObject(CreateCube(Vector3(element.x *5, height, element.y*5), Vector3(element.z*2.5, height, 2.5), 0.0f));
		}
	}
	for (auto element : doorData) {
		Door* door = CreateDoor(Vector3(element.x * 5, height, element.y * 5), Vector3(3 * 2.5, height, 1 * 2.5), 0.0f, element.z);
		switch ((int)element.z)
		{
		case 0:
			door->GetRenderObject()->SetColour(Debug::BLACK);
			break;
		case 90:
			door->GetRenderObject()->SetColour(Debug::RED);
			break;
		case 180:
			break;
		case 270:
			door->GetRenderObject()->SetColour(Debug::GREEN);
			break;
		default:
			break;
		}
		l2_Doors.push_back(door);
		level2.AddObject(door);
	}
}

void GameLevel::CreateLevel3() {
	// boss = CreateBoss(Vector3(0, -2, -60), Vector3(10, 10, 10), player, 0.0f);
	// level3.AddObject(CreateCube(Vector3(0, -2, 0), Vector3(100, 2, 100), 0.0f));
	//level3.AddObject(CreateCube(Vector3(70, 0, 77), Vector3(10, 10,5 ), 0.0f));
	level3.AddObject(CreateCube(Vector3(100, 30, 0), Vector3(2, 30, 100), 0.0f));
	level3.AddObject(CreateCube(Vector3(-100, 30, 0), Vector3(2, 30, 100), 0.0f));
	level3.AddObject(CreateCube(Vector3(0, 30, -100), Vector3(100, 30, 2), 0.0f));
	level3.AddObject(CreateCube(Vector3(0, 30, 100), Vector3(100, 30, 2), 0.0f));
	//level3.AddObject(CreateCube(Vector3(0, 0, 0), Vector3(5, 5, 5), 0.0f));

	//player = CreateSphere(Vector3(20, 5, 20), 5, 0.0f);
	//level3.AddObject(player);
	//testAI = CreateAItest(Vector3(0, 0, 0), Vector3(5, 5, 5), player, 0.0f);
	//level3.AddObject(testAI);
	level3.AddObject(CreateFloor(Vector3(0, -2, 0), Vector3(100, 2, 100), 0.0f));
	//level3.AddObject(boss=CreateBoss(Vector3(0, -2, -60), Vector3(10, 10, 10), 0.0f));
	//BossBehaviourTree(boss,player);
	//
	boss = CreateBoss(Vector3(0, 1, -60), Vector3(10, 10, 10), player,0.0f);
	iceCubeBullet=CreateIceCubeBullet(Vector3(0,-50,0), Vector3(4, 4, 4), 0.0f);
	level3.AddObject(boss);
	level3.AddObject(iceCubeBullet);
	// level3.AddObject(boss);
	/*static_cast<Boss*>(boss)->NCL::CSC8503::Boss::BossBehaviourTree(player);*/
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

	Level l1;
	//floor
	l1.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	//wall
	l1.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-90, 10,   0), Vector3(10, 10, 40), 0.0f));
	l1.AddObject(CreateCube(Vector3(-90, 10,  50), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 10,   0), Vector3(10, 10, 40), 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 10,  70), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 10,  90), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(  0, 10,  50), Vector3(40, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(  0, 10,  90), Vector3(40, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3( 50, 10,  90), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3( 70, 10,  70), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3( 90, 10,  50), Vector3(10, 10, 10), 0.0f));
	level4_normal.emplace_back(l1);

	Level l2;
	//floor
	l2.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	//wall
	l2.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l2.AddObject(CreateCube(Vector3(-90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(0, 10, 90), Vector3(40, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	//other objects
	l2.AddObject(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_normal.emplace_back(l2);

	Level l3;
	//floor
	l3.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	//wall
	l3.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l3.AddObject(CreateCube(Vector3(-90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(0, 10, 90), Vector3(40, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	//other objects
	l3.AddObject(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_normal.emplace_back(l3);

	Level l4;
	//floor
	l4.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	//wall
	l4.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l4.AddObject(CreateCube(Vector3(-90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(0, 10, 90), Vector3(40, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	//other objects
	l4.AddObject(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_normal.emplace_back(l4);

	Level l5;
	//floor
	l5.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	//wall
	l5.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l5.AddObject(CreateCube(Vector3(-90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(0, 10, 90), Vector3(40, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
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
	Level l1;
	//floor
	l1.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	//wall
	l1.AddObject(CreateCube(Vector3( 50, 10,  90), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3( 70, 10,  70), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3( 90, 10,  50), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3( 90, 10,   0), Vector3(10, 10, 40), 0.0f));
	l1.AddObject(CreateCube(Vector3( 90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3( 70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3( 50, 10,   0), Vector3(10, 10, 40), 0.0f));
	l1.AddObject(CreateCube(Vector3(  0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3( 50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(  0, 10, -90), Vector3(40, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	level4_rotate.emplace_back(l1);

	Level l2;
	//floor
	l2.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	//wall
	l2.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l2.AddObject(CreateCube(Vector3(90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l2.AddObject(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(0, 10, -90), Vector3(40, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	//other objects
	l2.AddObject(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_rotate.emplace_back(l2);

	Level l3;
	//floor
	l3.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	//wall
	l3.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l3.AddObject(CreateCube(Vector3(90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l3.AddObject(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(0, 10, -90), Vector3(40, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	//other objects
	l3.AddObject(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_rotate.emplace_back(l3);

	Level l4;
	//floor
	l4.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	//wall
	l4.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l4.AddObject(CreateCube(Vector3(90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l4.AddObject(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(0, 10, -90), Vector3(40, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	//other objects
	l4.AddObject(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_rotate.emplace_back(l4);

	Level l5;
	//floor
	l5.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	//wall
	l5.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l5.AddObject(CreateCube(Vector3(90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l5.AddObject(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(0, 10, -90), Vector3(40, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
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
	Level l1;
	//floor
	l1.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	//wall
	l1.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l1.AddObject(CreateCube(Vector3(-90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(0, 10, 90), Vector3(40, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	level4_reverse.emplace_back(l1);

	Level l2;
	//floor
	l2.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	//wall
	l2.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l2.AddObject(CreateCube(Vector3(-90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(0, 10, 90), Vector3(40, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	//other objects
	l2.AddObject(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_reverse.emplace_back(l2);

	Level l3;
	//floor
	l3.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	//wall
	l3.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l3.AddObject(CreateCube(Vector3(-90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(0, 10, 90), Vector3(40, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	//other objects
	l3.AddObject(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_reverse.emplace_back(l3);

	Level l4;
	//floor
	l4.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	//wall
	l4.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l4.AddObject(CreateCube(Vector3(-90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(0, 10, 90), Vector3(40, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	//other objects
	l4.AddObject(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_reverse.emplace_back(l4);

	Level l5;
	//floor
	l5.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	//wall
	l5.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l5.AddObject(CreateCube(Vector3(-90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(0, 10, 90), Vector3(40, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
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
	Level l1;
	//floor
	l1.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l1.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	//wall
	l1.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l1.AddObject(CreateCube(Vector3(90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l1.AddObject(CreateCube(Vector3(50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(0, 10, -90), Vector3(40, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l1.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	level4_reverse_rotate.emplace_back(l1);

	Level l2;
	//floor
	l2.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	//wall
	l2.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l2.AddObject(CreateCube(Vector3(90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l2.AddObject(CreateCube(Vector3(50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(0, 10, -90), Vector3(40, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l2.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	//other objects
	l2.AddObject(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_reverse_rotate.emplace_back(l2);

	Level l3;
	//floor
	l3.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	//wall
	l3.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l3.AddObject(CreateCube(Vector3(90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l3.AddObject(CreateCube(Vector3(50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(0, 10, -90), Vector3(40, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l3.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	//other objects
	l3.AddObject(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_reverse_rotate.emplace_back(l3);

	Level l4;
	//floor
	l4.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	//wall
	l4.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l4.AddObject(CreateCube(Vector3(90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l4.AddObject(CreateCube(Vector3(50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(0, 10, -90), Vector3(40, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l4.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
	//other objects
	l4.AddObject(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_reverse_rotate.emplace_back(l4);

	Level l5;
	//floor
	l5.AddObject(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	//wall
	l5.AddObject(CreateCube(Vector3(50, 10, 90), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(90, 10, 50), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 40), 0.0f));
	l5.AddObject(CreateCube(Vector3(90, 10, -50), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l5.AddObject(CreateCube(Vector3(50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(0, 10, -90), Vector3(40, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-50, 10, -90), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l5.AddObject(CreateCube(Vector3(-90, 10, -50), Vector3(10, 10, 10), 0.0f));
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
	level1.AddObject(CreateBookshelf(Vector3(-39, 0, -14), 0.0f, Vector3(0, 1, 0), 90));
	level1.AddObject(CreateBookshelf(Vector3(-39, 0, -22), 0.0f, Vector3(0, 1, 0), 90));

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
	level1.AddObject(CreateBookshelf(Vector3(66, 10, -16), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-66, 10, -96), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-74, 10, -96), 0.0f));
	level1.AddObject(CreateBookshelf(Vector3(-66, 10, -16), 0.0f));

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
	level1.AddObject(CreateColumn(Vector3(-50, 0, -60), Vector3(0.5, 0.343, 0.5), 0.0f));
	level1.AddObject(CreateCapsule(Vector3(-50, 20, -60), 60, 6, 0.0f));
	//level1.AddObject(CreateAABB(Vector3(-50, 1, -60), Vector3(3, 1, 3), 0.0f));

	level1.AddObject(CreateColumn(Vector3(-50, 0, -20), Vector3(0.5, 0.343, 0.5), 0.0f));
	level1.AddObject(CreateCapsule(Vector3(-50, 20, -20), 60, 6, 0.0f));
	//level1.AddObject(CreateAABB(Vector3(-50, 1, -20), Vector3(3, 1, 3), 0.0f));

	level1.AddObject(CreateColumn(Vector3(-50, 0, 20), Vector3(0.5, 0.343, 0.5), 0.0f));
	level1.AddObject(CreateCapsule(Vector3(-50, 20, 20), 60, 6, 0.0f));
	//level1.AddObject(CreateAABB(Vector3(-50, 1, 20), Vector3(3, 1, 3), 0.0f));

	level1.AddObject(CreateColumn(Vector3(50, 0, -60), Vector3(0.5, 0.343, 0.5), 0.0f));
	level1.AddObject(CreateCapsule(Vector3(50, 20, -60), 60, 6, 0.0f));
	//level1.AddObject(CreateAABB(Vector3(50, 1, -60), Vector3(3, 1, 3), 0.0f));

	level1.AddObject(CreateColumn(Vector3(50, 0, -20), Vector3(0.5, 0.343, 0.5), 0.0f));
	level1.AddObject(CreateCapsule(Vector3(50, 20, -20), 60, 6, 0.0f));
	//level1.AddObject(CreateAABB(Vector3(50, 1, -20), Vector3(3, 1, 3), 0.0f));

	level1.AddObject(CreateColumn(Vector3(50, 0, 20), Vector3(0.5, 0.343, 0.5), 0.0f));
	level1.AddObject(CreateCapsule(Vector3(50, 20, 20), 60, 6, 0.0f));
	//level1.AddObject(CreateAABB(Vector3(50, 1, 20), Vector3(3, 1, 3), 0.0f));
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
	level1.AddObject(CreateStairs(Vector3(80, 0, 4), Vector3(2.5, 2, 2), 0.0f));
	level1.AddObject(CreateCubeOBB(Vector3(80, -3.7, -8.3), Vector3(10, 10, 10), 0.0f, Vector3(1, 0, 0), 30));
	level1.AddObject(CreateStairs(Vector3(-80, 0, 4), Vector3(2.5, 2, 2), 0.0f));
	level1.AddObject(CreateCubeOBB(Vector3(-80, -3.7, -8.3), Vector3(10, 10, 10), 0.0f, Vector3(1, 0, 0), 30));
}