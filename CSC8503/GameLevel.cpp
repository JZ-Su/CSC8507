#include "GameLevel.h"
#include "BasicExamples.h"

using namespace NCL;

GameLevel::GameLevel(GameTechRenderer* render) : BasicExamples(render) {
	CreateLevel1();
	CreateLevel2();
	CreateLevel3();
	CreateLevel4();
}

void GameLevel::AddLevelToWorld(GameWorld* world, Level l) {
	for (auto element : l.objectList) {
		world->AddGameObject(element);
		element->SetWorldID(world->GetWorldIDCounter());
		world->SetWorldIDCounter(world->GetWorldIDCounter() + 1);
		world->SetWorldStateID(world->GetWorldStateID() + 1);
	}
}

void GameLevel::CreateLevel1() {
	level1.objectList.push_back(CreateCube(Vector3(), Vector3(10, 10, 10), 0.0f));

	level1.listSize = level1.objectList.size();
}

void GameLevel::CreateLevel2() {
	level2.objectList.push_back(CreateCube(Vector3(), Vector3(), 0.0f));

	level2.listSize = level2.objectList.size();
}

void GameLevel::CreateLevel3() {
	level3.objectList.push_back(CreateCube(Vector3(), Vector3(), 0.0f));

	level3.listSize = level3.objectList.size();
}

void GameLevel::CreateLevel4() {
	level4.objectList.push_back(CreateCube(Vector3(), Vector3(), 0.0f));

	level4.listSize = level4.objectList.size();
}
