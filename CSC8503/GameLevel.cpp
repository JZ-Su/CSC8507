#include "GameLevel.h"
#include "BasicExamples.h"
#include "RenderObject.h"
using namespace NCL;

GameLevel::GameLevel(GameTechRenderer* render) : BasicExamples(render) {
	CreateGeneric();
	CreateLevel1();
	CreateLevel2();
	CreateLevel3();
	CreateLevel4();
}

void GameLevel::CreateGeneric() {	
	Generic.objectList.push_back(CreatePlayer(Vector3(0, 10, 0), Vector3(4, 4, 4)));
	//Level 4 player: 
	//Generic.objectList.push_back(CreatePlayer(Vector3(-70, 10, -50), Vector3(1, 1, 1)));
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

		beginArea = level4.objectList[7];
		if (i == 0) {
			trueEndArea = level4.objectList[12];
			trueEndArea->isEnable = false;
			falseEndArea = level4.objectList[2];
			falseEndArea->isEnable = false;
		}
		else {
			trueEndArea = level4.objectList[2];
			trueEndArea->isEnable = false;
			falseEndArea = level4.objectList[12];
			falseEndArea->isEnable = false;
		}
		AddLevelToWorld(world, level4);
	}
	else {
		if (isReverse) level4r = level4_reverse_rotate[i];
		else level4r = level4_rotate[i];

		beginArea = level4r.objectList[7];
		if (i == 0) {
			trueEndArea = level4r.objectList[12];
			trueEndArea->isEnable = false;
			falseEndArea = level4r.objectList[2];
			falseEndArea->isEnable = false;
		}
		else {
			trueEndArea = level4r.objectList[2];
			trueEndArea->isEnable = false;
			falseEndArea = level4r.objectList[12];
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

void GameLevel::CreateLevel1() {
	level1.objectList.push_back(CreateFloor(Vector3(0, -2, 0), Vector3(100, 2, 100), 0.0f));
	level1.objectList.push_back(CreateCeiling(Vector3(0, 62, 0), Vector3(150, 2, 150), 0.0f));
	//layers
	level1.objectList.push_back(CreateCube(Vector3(-80, 5, -56), Vector3(20, 5, 44), 0.0f)); 
	level1.objectList.push_back(CreateCube(Vector3(80, 5, -56), Vector3(20, 5, 44), 0));
	level1.objectList.push_back(CreateCube(Vector3(0, 5, 45), Vector3(40, 5, 55), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(0, 15, 90), Vector3(40, 5, 10), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(0, 25, 95), Vector3(20, 5, 5.1), 0.0f));


	//wall
	level1.objectList.push_back(CreateCube(Vector3(0, 30, 101), Vector3(100, 30, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(0, 30, -101), Vector3(100, 30, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(101, 30, 0), Vector3(1, 30, 100), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-101, 30, 0), Vector3(1, 30, 100), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(70, 30, 77), Vector3(30, 30, 23), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-70, 30, 77), Vector3(30, 30, 23), 0.0f));
	//room
	level1.objectList.push_back(CreateCube(Vector3(-20, 5, -20), Vector3(1, 5, 10), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-20, 5, -50), Vector3(1, 5, 10), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(20, 5, -20), Vector3(1, 5, 10), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(20, 5, -50), Vector3(1, 5, 10), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(0, 5, -59), Vector3(19, 5, 1), 0.0f));
	//columns
	CreateLevel1_Columns();
	//stairs
	level1.objectList.push_back(CreateCubeOBB(Vector3(20, 15.9, 95), Vector3(10, 10, 5), 0.0f, Vector3(0, 0, 1),	-45));
	level1.objectList.push_back(CreateCubeOBB(Vector3(-20, 15.9, 95), Vector3(10, 10, 5), 0.0f, Vector3(0, 0, 1),	45));
	level1.objectList.push_back(CreateCubeOBB(Vector3(35, 6.35, 76.4), Vector3(5, 10, 10), 0.0f, Vector3(1, 0, 0),	-30));
	level1.objectList.push_back(CreateCubeOBB(Vector3(-35, 6.35, 76.4), Vector3(5, 10, 10), 0.0f, Vector3(1, 0, 0),	-30));
	level1.objectList.push_back(CreateCubeOBB(Vector3(43.7, -3.7, 44), Vector3(10, 10, 10), 0.0f, Vector3(0, 0, 1),	-30));
	level1.objectList.push_back(CreateCubeOBB(Vector3(-43.7, -3.7, 44), Vector3(10, 10, 10), 0.0f , Vector3(0, 0, 1), 30));
	level1.objectList.push_back(CreateCubeOBB(Vector3(80,  -3.7, -8.3), Vector3(10, 10, 10), 0.0f  , Vector3(1, 0, 0), 30));
	level1.objectList.push_back(CreateCubeOBB(Vector3(-80, -3.7, -8.3), Vector3(10, 10, 10), 0.0f, Vector3(1, 0, 0), 30));
	//book shelfs
	//CreateLevel1_BookShelfs();
	//small shelfs
	//CreateLevel1_SmallShelfs();
	//tables
	//CreateLevel1_Tables();
	level1.objectList.push_back(ghost=CreateGhost(Vector3(10, 10, 0), Vector3(5, 5, 5), 0.0f));

	//level1.objectList.push_back(player);
	testAI = CreateAItest(Vector3(10, 50, 10), Vector3(5, 5, 5), player, 0.0f);
	testAI->GetRenderObject()->SetColour(Debug::BLUE);
	level1.objectList.push_back(testAI);
}

void GameLevel::CreateLevel2() {
	level2.objectList.push_back(CreateCube(Vector3(0, -2, 20), Vector3(60, 2, 80), 0.0f));
	for (int i = 0; i < 7; ++i) {
		Vector3 position = Vector3(-60 + i * 20, 15, 40);
		level2.objectList.push_back(CreateCube(position, Vector3(1, 15, 60), 0.0f));
	}
	for (int i = 0; i < 5; ++i) {
		Vector3 position = Vector3(50, 12.5, i * 20);
		level2.objectList.push_back(CreateCube(position, Vector3(9, 12.5, 1), 0.0f));
		level2.objectList.push_back(CreateCube(Vector3(-50, 12.5, position.z), Vector3(9, 12.5, 1), 0.0f));
		level2.objectList.push_back(CreateCube(Vector3(-10, 12.5, position.z), Vector3(9, 12.5, 1), 0.0f));
		level2.objectList.push_back(CreateCube(Vector3(10, 12.5, position.z), Vector3(9, 12.5, 1), 0.0f));
	}
	level2.objectList.push_back(CreateCube(Vector3(-50, 12.5, -20), Vector3(9, 12.5, 1), 0.0f));
	level2.objectList.push_back(CreateCube(Vector3(50, 12.5, -20), Vector3(9, 12.5, 1), 0.0f));
	level2.objectList.push_back(CreateCube(Vector3(0, 15, -21), Vector3(21, 15, 1), 0.0f));
	level2.objectList.push_back(CreateCube(Vector3(-30, 15, 0), Vector3(9, 15, 1), 0.0f));
	level2.objectList.push_back(CreateCube(Vector3(30, 15, 0), Vector3(9, 15, 1), 0.0f));
	for (int i = 0; i < 3; ++i) {
		Vector3 position = Vector3(-20 + i * 20, 12.5, -50);
		level2.objectList.push_back(CreateCube(position, Vector3(1, 12.5, 9), 0.0f));
	}
	level2.objectList.push_back(CreateCube(Vector3(-41, 15, -51), Vector3(1, 15, 9), 0.0f));
	level2.objectList.push_back(CreateCube(Vector3(41, 15, -51), Vector3(1, 15, 9), 0.0f));
	level2.objectList.push_back(CreateCube(Vector3(-60, 15, -30), Vector3(1, 15, 10), 0.0f));
	level2.objectList.push_back(CreateCube(Vector3(60, 15, -30), Vector3(1, 15, 10), 0.0f));
	level2.objectList.push_back(CreateCube(Vector3(-40, 15, -30), Vector3(1, 15, 10), 0.0f));
	level2.objectList.push_back(CreateCube(Vector3(40, 15, -30), Vector3(1, 15, 10), 0.0f));
	level2.objectList.push_back(CreateCube(Vector3(0, 15, 101), Vector3(60, 15, 1), 0.0f));
	level2.objectList.push_back(CreateCube(Vector3(0, 15, -41), Vector3(60, 15, 1), 0.0f));
	level2.objectList.push_back(CreateCube(Vector3(0, 15, -60), Vector3(40, 15, 1), 0.0f));


	//player = CreateSphere(Vector3(20, 5, 20), 5, 0.0f);
	//level2.objectList.push_back(player);
	//testAI = CreateAItest(Vector3(-10, 10, -10), Vector3(5, 5, 5), player, 1.0f);
	//level2.objectList.push_back(testAI);
}

void GameLevel::CreateLevel3() {	
	// level3.objectList.push_back(CreateCube(Vector3(0, -2, 0), Vector3(100, 2, 100), 0.0f));
	//level3.objectList.push_back(CreateCube(Vector3(70, 0, 77), Vector3(10, 10,5 ), 0.0f));
	level3.objectList.push_back(CreateCube(Vector3(100, 30, 0), Vector3(2, 30, 100), 0.0f));
	level3.objectList.push_back(CreateCube(Vector3(-100, 30, 0), Vector3(2, 30, 100), 0.0f));
	level3.objectList.push_back(CreateCube(Vector3(0, 30, -100), Vector3(100,30, 2), 0.0f));
	level3.objectList.push_back(CreateCube(Vector3(0, 30, 100), Vector3(100, 30, 2), 0.0f));
	//level3.objectList.push_back(CreateCube(Vector3(0, 0, 0), Vector3(5, 5, 5), 0.0f));

	//player = CreateSphere(Vector3(20, 5, 20), 5, 0.0f);
	//level3.objectList.push_back(player);
	//testAI = CreateAItest(Vector3(0, 0, 0), Vector3(5, 5, 5), player, 0.0f);
	//level3.objectList.push_back(testAI);
	level3.objectList.push_back(CreateFloor(Vector3(0, -2, 0), Vector3(100, 2, 100), 0.0f));
	level3.objectList.push_back(boss=CreateBoss(Vector3(0, -2, -60), Vector3(10, 10, 10), 0.0f));

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
	l1.objectList.push_back(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-70, 0,  10), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-70, 0,  30), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-70, 0,  50), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3( 10, 0, 70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3( 30, 0, 70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3( 50, 0, 70), floorDimensions, 0.0f));
	//wall
	l1.objectList.push_back(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-90, 10,   0), Vector3(10, 10, 60), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-50, 10,   0), Vector3(10, 10, 40), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-70, 10,  70), Vector3(10, 10, 10), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(  0, 10,  50), Vector3(40, 10, 10), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(  0, 10,  90), Vector3(60, 10, 10), 0.0f));
	level4_normal.emplace_back(l1);

	Level l2;
	//floor
	l2.objectList.push_back(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	//wall
	l2.objectList.push_back(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(0, 10, 90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l2.objectList.push_back(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_normal.emplace_back(l2);

	Level l3;
	//floor
	l3.objectList.push_back(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	//wall
	l3.objectList.push_back(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(0, 10, 90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l3.objectList.push_back(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_normal.emplace_back(l3);

	Level l4;
	//floor
	l4.objectList.push_back(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	//wall
	l4.objectList.push_back(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(0, 10, 90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l4.objectList.push_back(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_normal.emplace_back(l4);

	Level l5;
	//floor
	l5.objectList.push_back(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	//wall
	l5.objectList.push_back(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(0, 10, 90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l5.objectList.push_back(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
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
	l1.objectList.push_back(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(70, 0,  10), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	//wall
	l1.objectList.push_back(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(0, 10, -90), Vector3(60, 10, 10), 0.0f));
	level4_rotate.emplace_back(l1);

	Level l2;
	//floor
	l2.objectList.push_back(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	//wall
	l2.objectList.push_back(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(0, 10, -90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l2.objectList.push_back(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_rotate.emplace_back(l2);

	Level l3;
	//floor
	l3.objectList.push_back(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	//wall
	l3.objectList.push_back(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(0, 10, -90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l3.objectList.push_back(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_rotate.emplace_back(l3);

	Level l4;
	//floor
	l4.objectList.push_back(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	//wall
	l4.objectList.push_back(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(0, 10, -90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l4.objectList.push_back(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_rotate.emplace_back(l4);

	Level l5;
	//floor
	l5.objectList.push_back(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	//wall
	l5.objectList.push_back(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(0, 10, -90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l5.objectList.push_back(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
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
	l1.objectList.push_back(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	//wall
	l1.objectList.push_back(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(0, 10, 90), Vector3(60, 10, 10), 0.0f));
	level4_reverse.emplace_back(l1);

	Level l2;
	//floor
	l2.objectList.push_back(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	//wall
	l2.objectList.push_back(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(0, 10, 90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l2.objectList.push_back(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_reverse.emplace_back(l2);

	Level l3;
	//floor
	l3.objectList.push_back(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	//wall
	l3.objectList.push_back(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(0, 10, 90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l3.objectList.push_back(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_reverse.emplace_back(l3);

	Level l4;
	//floor
	l4.objectList.push_back(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	//wall
	l4.objectList.push_back(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(0, 10, 90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l4.objectList.push_back(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
	level4_reverse.emplace_back(l4);

	Level l5;
	//floor
	l5.objectList.push_back(CreateCube(Vector3(50, 0, 70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(30, 0, 70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(10, 0, 70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-10, 0, 70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-30, 0, 70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-50, 0, 70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-50, 0, 50), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-70, 0, 50), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-70, 0, 30), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-70, 0, 10), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-70, 0, -10), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-70, 0, -30), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-70, 0, -50), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-50, 0, -50), floorDimensions, 0.0f));
	//wall
	l5.objectList.push_back(CreateCube(Vector3(-70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(0, 10, 50), Vector3(40, 10, 10), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(0, 10, 90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l5.objectList.push_back(CreateCube(Vector3(-50, 5, -50), Vector3(5, 5, 5), 0.0f));
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
	l1.objectList.push_back(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	//wall
	l1.objectList.push_back(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l1.objectList.push_back(CreateCube(Vector3(0, 10, -90), Vector3(60, 10, 10), 0.0f));
	level4_reverse_rotate.emplace_back(l1);

	Level l2;
	//floor
	l2.objectList.push_back(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	//wall
	l2.objectList.push_back(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l2.objectList.push_back(CreateCube(Vector3(0, 10, -90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l2.objectList.push_back(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_reverse_rotate.emplace_back(l2);

	Level l3;
	//floor
	l3.objectList.push_back(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	//wall
	l3.objectList.push_back(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l3.objectList.push_back(CreateCube(Vector3(0, 10, -90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l3.objectList.push_back(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_reverse_rotate.emplace_back(l3);

	Level l4;
	//floor
	l4.objectList.push_back(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	//wall
	l4.objectList.push_back(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l4.objectList.push_back(CreateCube(Vector3(0, 10, -90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l4.objectList.push_back(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_reverse_rotate.emplace_back(l4);

	Level l5;
	//floor
	l5.objectList.push_back(CreateCube(Vector3(-50, 0, -70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-30, 0, -70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(-10, 0, -70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(10, 0, -70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(30, 0, -70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(50, 0, -70), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(50, 0, -50), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(70, 0, -50), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(70, 0, -30), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(70, 0, -10), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(70, 0, 10), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(70, 0, 30), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(70, 0, 50), floorDimensions, 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(50, 0, 50), floorDimensions, 0.0f));
	//wall
	l5.objectList.push_back(CreateCube(Vector3(70, 10, 70), Vector3(10, 10, 10), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(90, 10, 0), Vector3(10, 10, 60), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(50, 10, 0), Vector3(10, 10, 40), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(70, 10, -70), Vector3(10, 10, 10), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(0, 10, -50), Vector3(40, 10, 10), 0.0f));
	l5.objectList.push_back(CreateCube(Vector3(0, 10, -90), Vector3(60, 10, 10), 0.0f));
	//other objects
	l5.objectList.push_back(CreateCube(Vector3(-50, 5, -70), Vector3(5, 5, 5), 0.0f));
	level4_reverse_rotate.emplace_back(l5);
}

void GameLevel::CreateLevel1_Tables() {
	level1.objectList.push_back(CreateTable(Vector3(20, 13, 70), 0.0f, Vector3(0, 1, 0), 90));
	level1.objectList.push_back(CreateAABB(Vector3(20, 11.5, 70), Vector3(2, 1.6, 6), 0.0f));

	//level1.objectList.push_back(CreateTable(Vector3(-20, 13, 70), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateAABB(Vector3(-20, 11.5, 70), Vector3(2, 1.5, 6), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(15, 13, 33), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateAABB(Vector3(15, 11.5, 33), Vector3(2, 1.5, 6), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(-15, 13, 33), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateAABB(Vector3(-15, 11.5, 33), Vector3(2, 1.5, 6), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(15, 13, 3), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateAABB(Vector3(15, 11.5, 3), Vector3(2, 1.5, 6), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(-15, 13, 3), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateAABB(Vector3(-15, 11.5, 3), Vector3(2, 1.5, 6), 0.0f));

	//level1.objectList.push_back(CreateTable(Vector3(-35, 3, -50), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateAABB(Vector3(-35, 1.5, -50), Vector3(2, 1.5, 6), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(-10, 3, -50), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateAABB(Vector3(-10, 1.5, -50), Vector3(2, 1.5, 6), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(10, 3, -50), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateAABB(Vector3(10, 1.5, -50), Vector3(2, 1.5, 6), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(-10, 3, -20), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateAABB(Vector3(-10, 1.5, -20), Vector3(2, 1.5, 6), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(10, 3, -20), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateAABB(Vector3(10, 1.5, -20), Vector3(2, 1.5, 6), 0.0f));

	//level1.objectList.push_back(CreateTable(Vector3(90, 3, 35), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateAABB(Vector3(90, 1.5, 35), Vector3(2, 1.5, 6), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(68, 13, -83), 0.0f));
	//level1.objectList.push_back(CreateAABB(Vector3(68, 11.5, -83), Vector3(6, 1.5, 2), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(68, 13, -69), 0.0f));
	//level1.objectList.push_back(CreateAABB(Vector3(68, 11.5, -69), Vector3(6, 1.5, 2), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(68, 13, -43), 0.0f));
	//level1.objectList.push_back(CreateAABB(Vector3(68, 11.5, -43), Vector3(6, 1.5, 2), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(68, 13, -29), 0.0f));
	//level1.objectList.push_back(CreateAABB(Vector3(68, 11.5, -29), Vector3(6, 1.5, 2), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(-68, 13, -83), 0.0f));
	//level1.objectList.push_back(CreateAABB(Vector3(-68, 11.5, -83), Vector3(6, 1.5, 2), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(-68, 13, -69), 0.0f));
	//level1.objectList.push_back(CreateAABB(Vector3(-68, 11.5, -69), Vector3(6, 1.5, 2), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(-68, 13, -43), 0.0f));
	//level1.objectList.push_back(CreateAABB(Vector3(-68, 11.5, -43), Vector3(6, 1.5, 2), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(-68, 13, -29), 0.0f));
	//level1.objectList.push_back(CreateAABB(Vector3(-68, 11.5, -29), Vector3(6, 1.5, 2), 0.0f));
	//level1.objectList.push_back(CreateTable(Vector3(-98, 13, -78), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateAABB(Vector3(-98, 11.5, -78), Vector3(2, 1.5, 6), 0.0f));
}

void GameLevel::CreateLevel1_SmallShelfs() {
	level1.objectList.push_back(CreateBookshelf(Vector3(-44, -6, 0), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-52, -6, 0), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-38, -6, -60), 0.0f));

	//level1.objectList.push_back(CreateBookshelf(Vector3(46, -6, -45), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(54, -6, -45), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(38, -6, -45), 0.0f));

	//level1.objectList.push_back(CreateBookshelf(Vector3(66, 4, -56), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(74, 4, -56), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-66, 4, -56),0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-74, 4, -56),0.0f));
}

void GameLevel::CreateLevel1_BookShelfs() {
	level1.objectList.push_back(CreateBookshelf(Vector3(0, 10, 60), 0.0f));
	level1.objectList.push_back(CreateBookshelf(Vector3(8, 10, 60),0.0f));
	level1.objectList.push_back(CreateBookshelf(Vector3(-8, 10, 60),0.0f));

	//level1.objectList.push_back(CreateBookshelf(Vector3(0, 10, 20), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(8, 10, 20), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-8, 10, 20), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-25, 10, 36), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-25, 10, 28), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(25, 10, 36), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(25, 10, 28), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-25, 10, 8), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-25, 10, 0), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(25, 10, 8), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(25, 10, 0), 0.0f, Vector3(0, 1, 0), 90));

	//level1.objectList.push_back(CreateBookshelf(Vector3(-52, 0, 33), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-44, 0, 33), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-60, 0, 33), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-96, 0, 33), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-88, 0, 33), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-61, 0, -8), 0.0f));

	//level1.objectList.push_back(CreateBookshelf(Vector3(-50, 0, -28), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-50, 0, -44), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-50, 0, -52), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-30, 0, -60), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-39, 0, -14), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-39, 0, -22), 0.0f, Vector3(0, 1, 0), 90));

	//level1.objectList.push_back(CreateBookshelf(Vector3(-24, 0, -96), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-24, 0, -88), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(24, 0, -96), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(24, 0, -88), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-50, 0, -96), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-50, 0, -88), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(0, 0, -16), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(0, 0, -24), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(0, 0, -44), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(0, 0, -52), 0.0f, Vector3(0, 1, 0), 90));

	//level1.objectList.push_back(CreateBookshelf(Vector3(50, 0, -96), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(50, 0, -88), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(50, 0, -80), 0.0f, Vector3(0, 1, 0), 90));

	//level1.objectList.push_back(CreateBookshelf(Vector3(61, 0, -8), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(61, 0, 0), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(61, 0, 8), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(71, 0, 22), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(71, 0, 30), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(71, 0, 38), 0.0f, Vector3(0, 1, 0), 90));

	//level1.objectList.push_back(CreateBookshelf(Vector3(66, 10, -96), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(74, 10, -96), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(66, 10, -16), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-66, 10, -96), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-74, 10, -96), 0.0f));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-66, 10, -16), 0.0f));

	//level1.objectList.push_back(CreateBookshelf(Vector3(90, 10, -86), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(90, 10, -78), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(90, 10, -70), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(90, 10, -44), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(90, 10, -36), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(90, 10, -28), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-90, 10, -86), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-90, 10, -78), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-90, 10, -70), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-90, 10, -44), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-90, 10, -36), 0.0f, Vector3(0, 1, 0), 90));
	//level1.objectList.push_back(CreateBookshelf(Vector3(-90, 10, -28), 0.0f, Vector3(0, 1, 0), 90));
}

void GameLevel::CreateLevel1_Columns() {
	level1.objectList.push_back(CreateColumn(Vector3(-50, 0, -60), Vector3(0.5, 0.343, 0.5), 0.0f));
	level1.objectList.push_back(CreateCapsule(Vector3(-50, 40, -60), 50, 4, 0.0f));
	level1.objectList.push_back(CreateAABB(Vector3(-50, 1, -60), Vector3(3, 1, 3), 0.0f));

	level1.objectList.push_back(CreateColumn(Vector3(-50, 0, -20), Vector3(0.5, 0.343, 0.5), 0.0f));
	level1.objectList.push_back(CreateCapsule(Vector3(-50, 40, -20), 50, 4, 0.0f));
	level1.objectList.push_back(CreateAABB(Vector3(-50, 1, -20), Vector3(3, 1, 3), 0.0f));

	level1.objectList.push_back(CreateColumn(Vector3(-50, 0, 20), Vector3(0.5, 0.343, 0.5), 0.0f));
	level1.objectList.push_back(CreateCapsule(Vector3(-50, 40, 20), 50, 4, 0.0f));
	level1.objectList.push_back(CreateAABB(Vector3(-50, 1, 20), Vector3(3, 1, 3), 0.0f));

	//level1.objectList.push_back(CreateColumn(Vector3(50, 0, -60), Vector3(0.5, 0.4, 0.5), 0.0f));
	//level1.objectList.push_back(CreateCapsule(Vector3(50, 40, -60), 50, 4, 0.0f));
	//level1.objectList.push_back(CreateAABB(Vector3(50, 1, -60), Vector3(3, 1, 3), 0.0f));

	//level1.objectList.push_back(CreateColumn(Vector3(50, 0, -20), Vector3(0.5, 0.4, 0.5), 0.0f));
	//level1.objectList.push_back(CreateCapsule(Vector3(50, 40, -20), 50, 4, 0.0f));
	//level1.objectList.push_back(CreateAABB(Vector3(50, 1, -20), Vector3(3, 1, 3), 0.0f));

	//level1.objectList.push_back(CreateColumn(Vector3(50, 0, 20), Vector3(0.5, 0.4, 0.5), 0.0f));
	//level1.objectList.push_back(CreateCapsule(Vector3(50, 40, 20), 50, 4, 0.0f));
	//level1.objectList.push_back(CreateAABB(Vector3(50, 1, 20), Vector3(3, 1, 3), 0.0f));
}