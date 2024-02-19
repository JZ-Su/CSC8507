#include "GameLevel.h"
#include "BasicExamples.h"
#include "RenderObject.h"
using namespace NCL;

GameLevel::GameLevel(GameTechRenderer* render) : BasicExamples(render) {
	CreateLevel1();
	CreateLevel2();
	CreateLevel3();
	CreateLevel4();
	CreateGeneric();
}

void GameLevel::CreateGeneric() {

	Generic.objectList.push_back(CreatePlayer(Vector3(0, 10, 0), Vector3(1, 1, 1)));
	
}

void GameLevel::AddLevelToWorld(GameWorld* world, Level l) {
	for (auto element : l.objectList) {
		world->AddGameObject(element);
		//element->SetWorldID(world->GetWorldIDCounter());
		//world->SetWorldIDCounter(world->GetWorldIDCounter() + 1);
		//world->SetWorldStateID(world->GetWorldStateID() + 1);
	}
}

void GameLevel::RemoveLevel(GameWorld* world, Level l, bool andDelete) {
	for (auto element : l.objectList) {
		world->RemoveGameObject(element, andDelete);
	}
}

void GameLevel::CreateLevel1() {
	level1.objectList.push_back(CreateFloor(Vector3(0, -2, 0), Vector3(100, 2, 100), 0.0f));
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
	level1.objectList.push_back(CreateCapsule(Vector3(-50, 40, -60), 50, 4, 0.0f));
	level1.objectList.push_back(CreateCapsule(Vector3(-50, 40, -20), 50, 4, 0.0f));
	level1.objectList.push_back(CreateCapsule(Vector3(-50, 40, 20), 50, 4, 0.0f));
	level1.objectList.push_back(CreateCapsule(Vector3(50, 40, -60), 50, 4, 0.0f));
	level1.objectList.push_back(CreateCapsule(Vector3(50, 40, -20), 50, 4, 0.0f));
	level1.objectList.push_back(CreateCapsule(Vector3(50, 40, 20), 50, 4, 0.0f));
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
	level1.objectList.push_back(CreateCube(Vector3(0, 15, 60), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(8, 15, 60), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-8, 15, 60), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(0, 15, 20), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(8, 15, 20), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-8, 15, 20), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-25, 15, 36), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-25, 15, 28), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(25, 15, 36), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(25, 15, 28), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-25, 15, 8), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-25, 15, 0), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(25, 15, 8), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(25, 15, 0), Vector3(1, 5, 4), 0.0f));

	level1.objectList.push_back(CreateCube(Vector3(-52, 5, 33), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-44, 5, 33), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-60, 5, 33), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-96, 5, 33), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-88, 5, 33), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-61, 5, -8), Vector3(1, 5, 4), 0.0f));

	level1.objectList.push_back(CreateCube(Vector3(-50, 5, -28), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-50, 5, -44), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-50, 5, -52), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-30, 5, -60), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-39, 5, -14), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-39, 5, -22), Vector3(1, 5, 4), 0.0f));

	level1.objectList.push_back(CreateCube(Vector3(-24, 5, -96), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-24, 5, -88), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(24, 5, -96), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(24, 5, -88), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-50, 5, -96), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-50, 5, -88), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(0, 5, -16), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(0, 5, -24), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(0, 5, -44), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(0, 5, -52), Vector3(1, 5, 4), 0.0f));

	level1.objectList.push_back(CreateCube(Vector3(50, 5, -96), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(50, 5, -88), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(50, 5, -80), Vector3(1, 5, 4), 0.0f));

	level1.objectList.push_back(CreateCube(Vector3(61, 5, -8), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(61, 5, 0), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(61, 5, 8), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(71, 5, 22), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(71, 5, 30), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(71, 5, 38), Vector3(1, 5, 4), 0.0f));

	level1.objectList.push_back(CreateCube(Vector3(66, 15, -96), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(74, 15, -96), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(66, 15, -16), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-66, 15, -96), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-74, 15, -96), Vector3(4, 5, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-66, 15, -16), Vector3(4, 5, 1), 0.0f));
	//small shelfs
	level1.objectList.push_back(CreateCube(Vector3(-44, 2, 0), Vector3(4, 2, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-52, 2, 0), Vector3(4, 2, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-38, 2, -60), Vector3(4, 2, 1), 0.0f));

	level1.objectList.push_back(CreateCube(Vector3(46, 2, -45), Vector3(4, 2, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(54, 2, -45), Vector3(4, 2, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(38, 2, -45), Vector3(4, 2, 1), 0.0f));

	level1.objectList.push_back(CreateCube(Vector3(66, 12, -56), Vector3(4, 2, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(74, 12, -56), Vector3(4, 2, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-66, 12, -56), Vector3(4, 2, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-74, 12, -56), Vector3(4, 2, 1), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(90, 15, -86), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(90, 15, -78), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(90, 15, -70), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(90, 15, -44), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(90, 15, -36), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(90, 15, -28), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-90, 15, -86), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-90, 15, -78), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-90, 15, -70), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-90, 15, -44), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-90, 15, -36), Vector3(1, 5, 4), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-90, 15, -28), Vector3(1, 5, 4), 0.0f));
	//tables
	level1.objectList.push_back(CreateCube(Vector3(20, 11.5, 70), Vector3(2, 1.5, 6), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-20, 11.5, 70), Vector3(2, 1.5, 6), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(15, 11.5, 33), Vector3(2, 1.5, 6), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-15, 11.5, 33), Vector3(2, 1.5, 6), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(15, 11.5, 3), Vector3(2, 1.5, 6), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-15, 11.5, 3), Vector3(2, 1.5, 6), 0.0f));

	level1.objectList.push_back(CreateCube(Vector3(-35, 1.5, -50), Vector3(2, 1.5, 6), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-10, 1.5, -50), Vector3(2, 1.5, 6), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(10, 1.5, -50), Vector3(2, 1.5, 6), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-10, 1.5, -20), Vector3(2, 1.5, 6), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(10, 1.5, -20), Vector3(2, 1.5, 6), 0.0f));

	level1.objectList.push_back(CreateCube(Vector3(90, 1.5, 35), Vector3(2, 1.5, 6), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(68, 11.5, -83), Vector3(6, 1.5, 2), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(68, 11.5, -69), Vector3(6, 1.5, 2), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(68, 11.5, -43), Vector3(6, 1.5, 2), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(68, 11.5, -29), Vector3(6, 1.5, 2), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-68, 11.5, -83), Vector3(6, 1.5, 2), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-68, 11.5, -69), Vector3(6, 1.5, 2), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-68, 11.5, -43), Vector3(6, 1.5, 2), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-68, 11.5, -29), Vector3(6, 1.5, 2), 0.0f));
	level1.objectList.push_back(CreateCube(Vector3(-98, 11.5, -78), Vector3(2, 1.5, 6), 0.0f));

	//level1.objectList.push_back(player);
	//testAI = CreateAItest(Vector3(0, 0, 0), Vector3(5, 5, 5), player, 0.0f);
    //	testAI->GetRenderObject()->SetColour(Debug::BLUE);
	//level1.objectList.push_back(testAI);

	level1.listSize = level1.objectList.size();
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

	level2.listSize = level2.objectList.size();
}

void GameLevel::CreateLevel3() {
	level3.objectList.push_back(CreateCube(Vector3(0, -2, 0), Vector3(100, 2, 100), 0.0f));
	level3.objectList.push_back(CreateCube(Vector3(70, 0, 77), Vector3(10, 10,5 ), 0.0f));
	//level3.objectList.push_back(CreateCube(Vector3(0, 0, 0), Vector3(5, 5, 5), 0.0f));

	//player = CreateSphere(Vector3(20, 5, 20), 5, 0.0f);
	//level3.objectList.push_back(player);
	//testAI = CreateAItest(Vector3(0, 0, 0), Vector3(5, 5, 5), player, 0.0f);
	//level3.objectList.push_back(testAI);


	UI=CreateQuad(Vector3(10,10,10), Vector3(20,20,20), 0.0f);
	UI->GetRenderObject()->SetColour(Debug::RED);
	level3.objectList.push_back(UI);
	level3.listSize = level3.objectList.size();

	level3.objectList.push_back(CreateFloor(Vector3(0, -2, 0), Vector3(100, 2, 100), 0.0f));
	level3.objectList.push_back(boss=CreateBoss(Vector3(0, -2, -60), Vector3(10, 10, 10), 0.0f));
	//BossBehaviourTree(boss,player);


}

void GameLevel::CreateLevel4() {
	level4.objectList.push_back(CreateCube(Vector3(0, -2, 0), Vector3(60, 2, 60), 0.0f));
	level4.objectList.push_back(CreateCube(Vector3(-61, 15, 0), Vector3(1, 15, 50), 0.0f));
	level4.objectList.push_back(CreateCube(Vector3(-49, 15, 0), Vector3(1, 15, 40), 0.0f));
	level4.objectList.push_back(CreateCube(Vector3(-44, 15, 39), Vector3(4, 15, 1), 0.0f));
	level4.objectList.push_back(CreateCube(Vector3(-56, 15, 51), Vector3(4, 15, 1), 0.0f));
	level4.objectList.push_back(CreateCube(Vector3(-51, 15, 55), Vector3(1, 15, 5), 0.0f));
	level4.objectList.push_back(CreateCube(Vector3(-39, 15, 44), Vector3(1, 15, 4), 0.0f));
		 
	level4.objectList.push_back(CreateCube(Vector3(0, 15, 61), Vector3(50, 15, 1), 0.0f));
	level4.objectList.push_back(CreateCube(Vector3(0, 15, 49), Vector3(40, 15, 1), 0.0f));
		 
	level4.objectList.push_back(CreateCube(Vector3(-39, 15, -44), Vector3(1, 15, 4), 0.0f));
	level4.objectList.push_back(CreateCube(Vector3(-44, 15, -39), Vector3(4, 15, 1), 0.0f));
	level4.objectList.push_back(CreateCube(Vector3(-56, 15, -51), Vector3(4, 15, 1), 0.0f));
	level4.objectList.push_back(CreateCube(Vector3(51, 15, 56), Vector3(1, 15, 4), 0.0f));

	//rotation
	//level4.objectList.push_back(CreateCube(Vector3(61, 15, 0), Vector3(1, 15, 50), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(49, 15, 0), Vector3(1, 15, 40), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(44, 15, -39), Vector3(4, 15, 1), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(56, 15, -51), Vector3(4, 15, 1), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(51, 15, -55), Vector3(1, 15, 5), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(39, 15, -44), Vector3(1, 15, 4), 0.0f));

	//level4.objectList.push_back(CreateCube(Vector3(0, 15, -61), Vector3(50, 15, 1), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(0, 15, -49), Vector3(40, 15, 1), 0.0f));

	//level4.objectList.push_back(CreateCube(Vector3(39, 15, 44), Vector3(1, 15, 4), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(44, 15, 39), Vector3(4, 15, 1), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(55, 15, 51), Vector3(5, 15, 1), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(-51, 15, -55), Vector3(1, 15, 5), 0.0f));

	//mirror
	//level4.objectList.push_back(CreateCube(Vector3(0, 15, -61), Vector3(50, 15, 1), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(0, 15, -49), Vector3(40, 15, 1), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(39, 15, -44), Vector3(1, 15, 4), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(51, 15, -56), Vector3(1, 15, 4), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(55, 15, -51), Vector3(5, 15, 1), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(44, 15, -39), Vector3(4, 15, 1), 0.0f));

	//level4.objectList.push_back(CreateCube(Vector3(61, 15, 0), Vector3(1, 15, 50), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(49, 15, 0), Vector3(1, 15, 40), 0.0f));

	//level4.objectList.push_back(CreateCube(Vector3(-44, 15, -39), Vector3(4, 15, 1), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(-39, 15, -44), Vector3(1, 15, 4), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(-51, 15, -55), Vector3(1, 15, 5), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(55, 15, 51), Vector3(5, 15, 1), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(39, 15, 44), Vector3(1, 15, 4), 0.0f));
	//level4.objectList.push_back(CreateCube(Vector3(44, 15, 39), Vector3(4, 15, 1), 0.0f));

	level4.listSize = level4.objectList.size();
}
