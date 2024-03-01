#include "TutorialGame.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "TextureLoader.h"

#include "PositionConstraint.h"
#include "OrientationConstraint.h"
#include "StateGameObject.h"

#include <fstream>
#include <iostream>
#include "Assets.h"
#include "BasicExamples.h"
#include "Boss.h"

using namespace NCL;
using namespace CSC8503;

TutorialGame::TutorialGame() : controller(*Window::GetWindow()->GetKeyboard(), *Window::GetWindow()->GetMouse()) {
	world = new GameWorld();
#ifdef USEVULKAN
	renderer = new GameTechVulkanRenderer(*world);
	renderer->Init();
	renderer->InitStructures();
#else 
	renderer = new GameTechRenderer(*world);
#endif

	physics = new PhysicsSystem(*world);

	forceMagnitude = 10.0f;
	useGravity = true;
	physics->UseGravity(useGravity);
	inSelectionMode = false;

	world->GetMainCamera().SetController(controller);

	controller.MapAxis(0, "Sidestep");
	controller.MapAxis(1, "UpDown");
	controller.MapAxis(2, "Forward");

	controller.MapAxis(3, "XLook");
	controller.MapAxis(4, "YLook");

	//LoadRankingFile();
	//gameState = MainMenu;
	gameState = Start;
	mainMenuState = MainMenu_Start;
	gameMode = TimeLimited;
}
/*

Each of the little demo scenarios used in the game uses the same 2 meshes,
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/

void TutorialGame::InitialiseAssets() {
	InitCamera();
	InitWorld();
}

TutorialGame::~TutorialGame() {
	delete physics;
	delete renderer;
	delete world;
}

void TutorialGame::UpdateGame(float dt) {
	Debug::DrawLine(Vector3(), Vector3(100, 0, 0), Debug::RED);
	Debug::DrawLine(Vector3(), Vector3(0, 100, 0), Debug::GREEN);
	Debug::DrawLine(Vector3(), Vector3(0, 0, 100), Debug::BLUE);
	Debug::DrawCollisionBox(player);
	//gameLevel->GetBoss()->Update(dt);
	player->UpdatePlayer(dt);
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::P)) {
		gameState = Pause;
		return;
	}
	if ((gameMode == TimeLimited && totalTime >= 150) || (gameMode == ScoreRequired && score >= 20)) {
		gameState = Win;
		return;
	}

	totalTime += dt;
	health = player->GetHealth();
	if (!inSelectionMode) {
		world->GetMainCamera().UpdateCamera(dt);
	}
	if (lockedObject != nullptr) {
		LockedObjectMovement(dt);
	}

	UpdateKeys(dt);

	if (useGravity) {
		Debug::Print("(G)ravity on", Vector2(5, 95), Debug::RED);
	}
	else {
		Debug::Print("(G)ravity off", Vector2(5, 95), Debug::RED);
	}
	if (lockedObject == nullptr) {
		Debug::Print("Press F to lock camera", Vector2(5, 80));
	}
	else {
		Debug::Print("Press F to free camera", Vector2(5, 80));
	}

	RayCollision closestCollision;
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::K) && selectionObject) {
		Vector3 rayPos;
		Vector3 rayDir;

		rayDir = selectionObject->GetTransform().GetOrientation() * Vector3(0, 0, -1);

		rayPos = selectionObject->GetTransform().GetPosition();

		Ray r = Ray(rayPos, rayDir);

		if (world->Raycast(r, closestCollision, true, selectionObject)) {
			if (objClosest) {
				objClosest->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
			}
			objClosest = (GameObject*)closestCollision.node;

			objClosest->GetRenderObject()->SetColour(Vector4(1, 0, 1, 1));
		}
	}
	//gameLevel->GetBoss()->Update(dt);
	//UpdateBossAnim(gameLevel->GetBoss(), bossAnimation, dt);

	UpdateGhostAnim(ghost, ghostAnimation, dt);

	//// update player animation
	UpdatePlayerAnim(player, playerIdleAnimation, playerWalkAnimation, dt);

	IceCubeBulletLogic(dt);
	if (gameLevel->GetBoss()->getIsAttack()) {
		ExecuteAttack(dt);
		//gameLevel->GetBoss()->setIsAttack(false);
	}
	SelectObject();
	MoveSelectedObject();


	// Level 1 stuff
	if (currentLevel == 2) {
		UpdateGhostAnim(ghost, ghostAnimation, dt);
	}
	// Level 2
	else if (currentLevel == 4) {
		for (const auto& element : gameLevel->GetL2Doors()) {
			element->Update(dt);
		}
	}
	// Level 3
	else if (currentLevel == 6) {
		gameLevel->GetBoss()->Update(dt);
		UpdateBossAnim(gameLevel->GetBoss(), bossAnimation, dt);
		IceCubeBulletLogic(dt);
	}
	// Level 4
	else if (currentLevel == 8) {
		GameObject* beginDet = gameLevel->GetBeginArea();
		GameObject* trueEndDet = gameLevel->GetTrueEndArea();
		GameObject* falseEndDet = gameLevel->GetFalseEndArea();
		trueEndDet->GetRenderObject()->SetColour(Debug::RED);
		falseEndDet->GetRenderObject()->SetColour(Debug::GREEN);
		if (!physics->GetCollisionDetectionList(beginDet).empty() && physics->GetCollisionDetectionList(beginDet)[0] == player && beginDet->isEnable == true) {
			beginDet->isEnable = false;
			trueEndDet->isEnable = true;
			falseEndDet->isEnable = true;
			if (!hasRotation) {
				GameLevel::RemoveLevel(world, gameLevel->GetLevel4(), true, false);
			}
			else {
				GameLevel::RemoveLevel(world, gameLevel->GetLevel4r(), true, false);
			}
		}
		if (!physics->GetCollisionDetectionList(trueEndDet).empty() && physics->GetCollisionDetectionList(trueEndDet)[0] == player && trueEndDet->isEnable == true) {
			trueEndDet->isEnable = false;
			falseEndDet->isEnable = false;
			if (mapIndex >= 1) {
				hasReverse = !hasReverse;
			}
			score++;
			mapIndex = static_cast<int>(RandomValue(-4, 5));
			gameLevel->AddLevelToWorld(world, mapIndex, hasRotation, hasReverse);
			hasRotation = !hasRotation;
		}
		if (!physics->GetCollisionDetectionList(falseEndDet).empty() && physics->GetCollisionDetectionList(falseEndDet)[0] == player && falseEndDet->isEnable == true) {
			trueEndDet->isEnable = false;
			falseEndDet->isEnable = false;
			if (mapIndex < 1) {
				hasReverse = !hasReverse;
			}
			score = 0;
			mapIndex = 0;
			gameLevel->AddLevelToWorld(world, mapIndex, hasRotation, hasReverse);
			hasRotation = !hasRotation;
		}
	}
	world->UpdateWorld(dt);
	renderer->Update(dt);
	physics->Update(dt);

	Debug::Print("Score: " + std::to_string(score), Vector2(70, 80));
	Debug::Print("Totaltime: " + std::to_string(totalTime), Vector2(70, 85));
	Debug::Print("Press P to Pause!", Vector2(70, 90));

	renderer->Render();
	Debug::UpdateRenderables(dt);

	if (!isDebug) {
		SwitchLevel();
	}

	if (totalTime > 100) totalTime = 0;
}

void TutorialGame::UpdateKeys(float dt) {
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F1)) {
		InitWorld(); //We can reset the simulation at any time with F1
		selectionObject = nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	if (Window::GetKeyboard()->KeyPressed(KeyCodes::G)) {
		useGravity = !useGravity; //Toggle gravity!
		physics->UseGravity(useGravity);
	}
	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F8)) {
		world->ShuffleObjects(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F)) {
		if (lockedObject == nullptr) {
			LockCameraToObject(player);
		}
		else {
			LockCameraToObject(nullptr);
		}
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::H)) {
		gameLevel->GetBoss()->decreaseBossHealth(20.0f);
		std::cout <<"Boss's Health:" << gameLevel->GetBoss()->getBossHealth() << std::endl;
		gameLevel->GetBoss()->SetIsRencentlyHurt(true);
	}

	if (lockedObject) {
		LockedObjectMovement(dt);
	}
	else {
		DebugObjectMovement();
	}
}

void TutorialGame::LockedObjectMovement(float dt) {
	Matrix4 view = world->GetMainCamera().BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();

	Vector3 camRightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!
	//Vector3 xdirAxis = Vector3(camWorld.GetRow(0));
	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!
	Vector3 pos = lockedObject->GetTransform().GetPosition();
	Quaternion ObjOrientation = lockedObject->GetTransform().GetOrientation();

	//fwd axis of camera
	Vector3 UpAxis = Vector3(0, 1, 0);
	Vector3 fwdAxis = Vector3::Cross(camRightAxis, UpAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();


	Vector3 rightAxis = Vector3::Cross(UpAxis, fwdAxis);

	v += (Window::GetMouse()->GetRelativePosition().y);
	v = std::clamp(v, -45.0f, 45.0f);

	h -= (Window::GetMouse()->GetRelativePosition().x);
	if (h < 0) h += 360.0f;
	if (h < 360) h -= 360.0f;

	Matrix4 Myaw = Matrix4::Rotation(h, Vector3(0, 1, 0));
	Matrix4 Mpitch = Matrix4::Rotation(v, Myaw * Vector3(-1, 0, 0));
	Matrix4 Mrot = Mpitch * Myaw;

	Vector3 targetpos = lockedObject->GetTransform().GetPosition();
	Vector3 camdir = Mrot * Vector3(0, 0, -1);

	Vector3 campos = targetpos - camdir * 20.0f;

	Ray collisionRay = Ray(targetpos, -camdir);
	RayCollision collisionRayData;
	//if (world->Raycast(collisionRay, collisionRayData, true, lockedObject))
	//{
	//	if (collisionRayData.rayDistance < 6)
	//		campos = targetpos - camdir * (collisionRayData.rayDistance - 1.0f);
	//}

	if (Window::GetKeyboard()->KeyDown(KeyCodes::W)) {
		player->SetIsWalk(true);
		lockedObject->GetPhysicsObject()->AddForce(-fwdAxis);
		lockedObject->GetTransform().SetOrientation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (Window::GetKeyboard()->KeyDown(KeyCodes::S)) {
		player->SetIsWalk(true);
		lockedObject->GetPhysicsObject()->AddForce(fwdAxis);
		lockedObject->GetTransform().SetOrientation(Quaternion(0.0f, 1.0f, 0.0f, 0.0f));
	}
	else if (Window::GetKeyboard()->KeyDown(KeyCodes::A)) {
		player->SetIsWalk(true);
		lockedObject->GetPhysicsObject()->AddForce(-rightAxis);
	}
	else if (Window::GetKeyboard()->KeyDown(KeyCodes::D)) {
		player->SetIsWalk(true);
		lockedObject->GetPhysicsObject()->AddForce(rightAxis);
	}
	else if (Window::GetKeyboard()->KeyDown(KeyCodes::SPACE)) {
		if (player->GetCanJump())
		{
			Vector3 velocity = lockedObject->GetPhysicsObject()->GetLinearVelocity();
			lockedObject->GetPhysicsObject()->SetLinearVelocity(Vector3(velocity.x, 4, velocity.z));
			player->SetCanJump(false);
			player->setJumpTimer(1.1f);
			player->SetIsJumping(true);
		}
	}
	else {
		if (player->IsJumping()) {
			if (player->updateJumpTimer(dt)) {
				player->SetIsJumping(false);
			}
		}
		player->SetIsWalk(false);
	}
	Matrix4 viewMat = Matrix4::BuildViewMatrix(campos, targetpos, Vector3(0, 1, 0)).Inverse();
	Quaternion q(viewMat);
	float pitch = q.ToEuler().x;
	float yaw = q.ToEuler().y;

	Quaternion lookat = Quaternion::EulerAnglesToQuaternion(0, yaw, 0);

	lockedObject->GetTransform().SetOrientation(lookat);

	world->GetMainCamera().SetPosition(Vector3(campos.x, campos.y + 3.0f, campos.z + 3.0f));
	world->GetMainCamera().SetPitch(pitch);
	world->GetMainCamera().SetYaw(yaw);
	//renderer.UpdateProjMatrixFov(Window::GetMouse()->GetWheelMovement());
	UpdateProjMatrixFov(Window::GetMouse()->GetWheelMovement());
	
}

void TutorialGame::DebugObjectMovement() {
	//If we've selected an object, we can manipulate it with some key presses
	if (inSelectionMode && selectionObject) {
		//Twist the selected object!
		if (Window::GetKeyboard()->KeyDown(KeyCodes::NUMPAD4)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::NUMPAD6)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::NUMPAD8)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::NUMPAD2)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::UP)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::DOWN)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::LEFT)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(-10, 0, 0));
		}
	}
	else {
		if (Window::GetKeyboard()->KeyDown(KeyCodes::NUMPAD4)) {
			player->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::NUMPAD6)) {
			player->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::NUMPAD8)) {
			player->GetPhysicsObject()->AddTorque(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::NUMPAD2)) {
			player->GetPhysicsObject()->AddTorque(Vector3(0, 0, -10));
		}

		else if (Window::GetKeyboard()->KeyDown(KeyCodes::UP)) {
			player->GetPhysicsObject()->AddForce(Vector3(0, 0, -2));
			player->GetTransform().SetOrientation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
			player->SetIsWalk(true);
		}

		else if (Window::GetKeyboard()->KeyDown(KeyCodes::DOWN)) {
			player->GetPhysicsObject()->AddForce(Vector3(0, 0, 2));
			player->GetTransform().SetOrientation(Quaternion(0.0f, 1.0f, 0.0f, 0.0f));
			player->SetIsWalk(true);
		}

		else if (Window::GetKeyboard()->KeyDown(KeyCodes::LEFT)) {
			player->GetPhysicsObject()->AddForce(Vector3(-2, 0, 0));
			player->GetTransform().SetOrientation(Quaternion(0.0f, 1.0f, 0.0f, 1.0f));
			player->SetIsWalk(true);
		}

		else if (Window::GetKeyboard()->KeyDown(KeyCodes::RIGHT)) {
			player->GetPhysicsObject()->AddForce(Vector3(2, 0, 0));
			player->GetTransform().SetOrientation(Quaternion(0.0f, -1.0f, 0.0f, 1.0f));
			player->SetIsWalk(true);
		}
	}
}

/*

A single function to add a large immoveable cube to the bottom of our world

*/

/*
Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple'
physics worlds. You'll probably need another function for the creation of OBB cubes too.
*/

void TutorialGame::InitCamera() {
	world->GetMainCamera().SetNearPlane(0.1f);
	world->GetMainCamera().SetFarPlane(500.0f);
	world->GetMainCamera().SetPitch(-35.0f);
	world->GetMainCamera().SetYaw(0.0f);
	world->GetMainCamera().SetPosition(Vector3(0, 100, 150));
	lockedObject = nullptr;
}

void TutorialGame::InitWorld() {
	world->ClearAndErase();
	physics->Clear();
	blocker = nullptr;

	gameLevel = new GameLevel(renderer);
	gameLevel->AddLevelToWorld(world, gameLevel->GetGeneric());
	player = gameLevel->GetPlayer();
	playerWalkAnimation = gameLevel->getplayerWalkAnimation();
	playerIdleAnimation = gameLevel->getplayerIdleAnimation();
	playerJumpAnimation = gameLevel->getplayerJumpAnimation();
	ghost = gameLevel->getGhost();
	ghostAnimation = gameLevel->getGhostAnimation();

	boss = gameLevel->GetBoss();
	bossAnimation = gameLevel->getBossAnimation();

	/*
		Please switch the debug mode here
	*/
	isDebug = true;
	//isDebug = false;
	if (isDebug) {
		//Level 1
		//currentLevel = 2;
		//gameLevel->AddLevelToWorld(world, *gameLevel->GetLevel1());
		//ghost = gameLevel->getGhost();
		//ghostAnimation = gameLevel->getGhostAnimation();

		//Level 2
		//currentLevel = 4;
		//gameLevel->AddLevelToWorld(world, *gameLevel->GetLevel2());

		//Level 3
		currentLevel = 6;
		gameLevel->AddLevelToWorld(world, *gameLevel->GetLevel3());
		boss = gameLevel->GetBoss();
		bossAnimation = gameLevel->getBossAnimation();
		bossCheersAnimation = gameLevel->getBossCheersAnimation();
		bossShootingAnimation = gameLevel->getBossShootingAnimation();
		iceCubeBullet = gameLevel->getIceCubeBullet();

		//Level 4 initalize function
		//currentLevel = 8;
		//gameLevel->AddLevelToWorld(world, 0, true, false);
		//gameLevel->AddLevelToWorld(world, 0, false, false);
	}
	else {
		currentLevel = 1;
		//gameLevel->AddLevelToWorld(world, *gameLevel->GetLevel1());
		gameLevel->AddLevelToWorld(world, *gameLevel->GetConnection());
		portal = gameLevel->GetConnection()->portal;
		lockedObject = player;
	}

	score = 0;
	totalTime = 0.0f;
}

//void TutorialGame::BridgeConstraintTest() {
//	Vector3 cubeSize = Vector3(5, 5, 5);
//
//	float invCubeMass = 5; //how heavy the middle pieces are
//	int numLinks = 10;
//	float maxDistance = 20; //constraint distance
//	float cubeDistance = 20; //distance between links
//
//	Vector3 startPos = Vector3(10, 10, 10);
//	GameObject* start = AddCubeToWorld(startPos + Vector3(0, 0, 0), cubeSize, 10);
//	GameObject* end = AddCubeToWorld(startPos + Vector3((numLinks + 2) * cubeDistance, 0, 0), cubeSize, 10);
//	GameObject* previous = start;
//
//	for (int i = 0; i < numLinks; i++) {
//		GameObject* block = AddCubeToWorld(startPos + Vector3((i + 1) * cubeDistance, 0, 0), cubeSize, invCubeMass);
//		PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);
//		world->AddConstraint(constraint);
//		previous = block;
//	}
//	PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
//	world->AddConstraint(constraint);
//}

/*
Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around.

*/
bool TutorialGame::SelectObject() {
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		Debug::Print("Press Q to change to camera mode!", Vector2(5, 85));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::Left)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject = nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(world->GetMainCamera());

			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				selectionObject = (GameObject*)closestCollision.node;

				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				return true;
			}
			else {
				return false;
			}
		}
		//if (Window::GetKeyboard()->KeyPressed(NCL::KeyCodes::L)) {
		//	if (selectionObject) {
		//		if (lockedObject == selectionObject) {
		//			lockedObject = nullptr;
		//		}
		//		else {
		//			lockedObject = selectionObject;
		//		}
		//	}
		//}
	}
	else {
		Debug::Print("Press Q to change to select mode!", Vector2(5, 85));
	}
	return false;
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/

void TutorialGame::MoveSelectedObject() {
	Debug::Print("Click Force:" + std::to_string(forceMagnitude), Vector2(5, 90));
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

	if (!selectionObject) {
		return;//we haven't selected anything!
	}
	//Push the selected object!
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::Right)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			if (closestCollision.node == selectionObject) {
				selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);
			}
		}
	}
}

/*
	Game State
*/
void TutorialGame::ShowMainMenu(float dt) {
	//clear the world
	nameString = "";
	lockedObject = nullptr;
	world->ClearAndErase();
	world->GetMainCamera().SetPitch(0.0f);
	world->GetMainCamera().SetYaw(0.0f);
	world->GetMainCamera().SetPosition(Vector3(0, 0, 0));

	Debug::Print("Main Menu", Vector2(30, 30), Debug::BLUE);
	Debug::Print("Start Game", Vector2(30, 40), Debug::BLACK);
	Debug::Print("Show Ranking", Vector2(30, 50), Debug::BLACK);
	Debug::Print("Exit", Vector2(30, 60), Debug::BLACK);
	switch (mainMenuState)
	{
	case NCL::CSC8503::MainMenu_Start:
		Debug::Print("Start Game", Vector2(30, 40), Debug::RED);
		break;
	case NCL::CSC8503::MainMenu_Ranking:
		Debug::Print("Show Ranking", Vector2(30, 50), Debug::RED);
		break;
	case NCL::CSC8503::MainMenu_Exit:
		Debug::Print("Exit", Vector2(30, 60), Debug::RED);
		break;
	default:
		break;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyCodes::UP) && (mainMenuState != MainMenu_Start)) {
		mainMenuState = static_cast<MainMenuState>(static_cast<int>(mainMenuState) - 1);
	}
	else if (Window::GetKeyboard()->KeyPressed(KeyCodes::DOWN) && (mainMenuState != MainMenu_Exit)) {
		mainMenuState = static_cast<MainMenuState>(static_cast<int>(mainMenuState) + 1);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyCodes::RETURN)) {
		switch (mainMenuState)
		{
		case NCL::CSC8503::MainMenu_Start:
			gameState = ModeSelect;
			break;
		case NCL::CSC8503::MainMenu_Ranking:
			rankingPage = TimeLimitedModePage;
			gameState = Ranking;
			break;
		case NCL::CSC8503::MainMenu_Exit:
			gameState = Exit;
			break;
		default:
			break;
		}
	}
	renderer->Render();
	Debug::UpdateRenderables(dt);
}

void TutorialGame::ShowRanking(float dt) {
	Debug::Print("Ranking", Vector2(5, 10), Debug::RED);
	Debug::Print("Player Name:", Vector2(5, 20), Debug::RED);
	if (rankingPage == TimeLimitedModePage) {
		Debug::Print("Score:", Vector2(60, 20), Debug::RED);
		for (int i = 0; i < 5; i++) {
			if (i >= ScoreTable.size()) {
				break;
			}
			else {
				Debug::Print(ScoreTable[i].playerName, Vector2(5, 30 + 10 * i));
				Debug::Print(std::to_string(static_cast<int>(ScoreTable[i].score)), Vector2(60, 30 + 10 * i));
			}
		}
		Debug::Print("Press -> to switch page.", Vector2(5, 80));
		Debug::Print("1 / 2  ->", Vector2(45, 90));
	}
	else {
		Debug::Print("Time Cost:", Vector2(60, 20), Debug::RED);
		for (int i = 0; i < 5; i++) {
			if (i >= TimeTable.size()) {
				break;
			}
			else {
				Debug::Print(TimeTable[i].playerName, Vector2(5, 30 + 10 * i));
				Debug::Print(std::to_string(TimeTable[i].score), Vector2(60, 30 + 10 * i));
			}
		}
		Debug::Print("Press <- to switch page.", Vector2(5, 80));
		Debug::Print("<-  2 / 2", Vector2(37.5, 90));
	}
	Debug::Print("Press Enter to back to main menu", Vector2(5, 85));

	if (Window::GetKeyboard()->KeyPressed(KeyCodes::RIGHT)) rankingPage = ScoreRequiredModePage;
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::LEFT))  rankingPage = TimeLimitedModePage;
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::RETURN)) gameState = MainMenu;

	renderer->Render();
	Debug::UpdateRenderables(dt);
}

void TutorialGame::SelectGameMode(float dt) {
	Debug::Print("Mode Select: ", Vector2(30, 30), Debug::BLUE);
	if (gameMode == TimeLimited) {
		Debug::Print("Time Limited: 150s", Vector2(30, 50), Debug::RED);
		Debug::Print("Score Required: 20", Vector2(30, 60), Debug::BLACK);
	}
	else if (gameMode == ScoreRequired) {
		Debug::Print("Time Limited: 150s", Vector2(30, 50), Debug::BLACK);
		Debug::Print("Score Required: 20", Vector2(30, 60), Debug::RED);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::UP))   gameMode = TimeLimited;
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::DOWN)) gameMode = ScoreRequired;

	if (Window::GetKeyboard()->KeyPressed(KeyCodes::RETURN)) gameState = Start;

	renderer->Render();
	Debug::UpdateRenderables(dt);
}

void TutorialGame::InitGame() {
	InitialiseAssets();
	gameState = OnGoing;
}

void TutorialGame::ShowPause(float dt) {
	Debug::Print("Pause!", Vector2(30, 30), Debug::RED);
	Debug::Print("Press P to continue", Vector2(30, 50), Debug::BLACK);
	Debug::Print("Press M back to main menu", Vector2(30, 70), Debug::BLACK);
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::P)) gameState = OnGoing;
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::M)) gameState = MainMenu;
	renderer->Render();
	Debug::UpdateRenderables(dt);
}

void TutorialGame::ShowWin(float dt) {
	Debug::Print("You win!", Vector2(20, 30), Debug::RED);
	Debug::Print("Press Enter to continue", Vector2(20, 60), Debug::BLACK);

	if (gameMode == TimeLimited) {
		Debug::Print("Your score:", Vector2(20, 35), Debug::BLACK);
		Debug::Print(std::to_string(score), Vector2(35, 40), Debug::BLACK);
		int lowestScore;
		ScoreTable.size() > 4 ? lowestScore = ScoreTable[4].score : lowestScore = ScoreTable[ScoreTable.size() - 1].score;
		if (score > lowestScore) {
			Debug::Print("New Record!", Vector2(20, 45), Debug::RED);
			Debug::Print("Please input your name:________", Vector2(20, 50), Debug::RED);
			Debug::Print(nameString, Vector2(63, 50));
			char r = Window::GetKeyboard()->GetKey();
			if (r != '0') {
				if (r == 8 && !nameString.empty()) {
					nameString.pop_back();
				}
				else {
					nameString += r;
				}
			}
		}
		if (Window::GetKeyboard()->KeyPressed(KeyCodes::RETURN)) {
			if (score > lowestScore) {
				for (int i = 0; i < ScoreTable.size(); i++) {
					if (score > ScoreTable[i].score) {
						RankScore rk;
						rk.score = score;
						rk.playerName = nameString;
						ScoreTable.insert(ScoreTable.begin() + i, rk);
						break;
					}
				}
				std::ofstream file;
				file.open(Assets::SCOREDIR + "timeMode.txt", std::ios::out | std::ios::trunc);
				if (!ScoreTable.empty()) {
					for (int i = 0; i < ScoreTable.size() && i < 5; i++) {
						file << ScoreTable[i].playerName << std::endl;
						file << ScoreTable[i].score << std::endl;
					}
				}
			}
			gameState = MainMenu;
		}
	}
	else {
		Debug::Print("Cost Time:", Vector2(20, 35), Debug::BLACK);
		Debug::Print(std::to_string(totalTime), Vector2(35, 40), Debug::BLACK);
		float mostTimeCost;
		TimeTable.size() > 4 ? mostTimeCost = TimeTable[4].score : mostTimeCost = TimeTable[TimeTable.size() - 1].score;
		if (totalTime < mostTimeCost) {
			Debug::Print("New Record!", Vector2(20, 45), Debug::RED);
			Debug::Print("Please input your name:________", Vector2(20, 50), Debug::RED);
			Debug::Print(nameString, Vector2(63, 50));
			char r = Window::GetKeyboard()->GetKey();
			if (r != '0') {
				if (r == 8 && !nameString.empty()) {
					nameString.pop_back();
				}
				else {
					nameString += r;
				}
			}
		}
		if (Window::GetKeyboard()->KeyPressed(KeyCodes::RETURN)) {
			if (totalTime < mostTimeCost) {
				for (int i = 0; i < TimeTable.size(); i++) {
					if (score < TimeTable[i].score) {
						RankScore rk;
						rk.score = score;
						rk.playerName = nameString;
						TimeTable.insert(TimeTable.begin() + i, rk);
						break;
					}
				}
				std::ofstream file;
				file.open(Assets::SCOREDIR + "scoreMode.txt", std::ios::out | std::ios::trunc);
				if (!TimeTable.empty()) {
					for (int i = 0; i < TimeTable.size() && i < 5; i++) {
						file << TimeTable[i].playerName << std::endl;
						file << TimeTable[i].score << std::endl;
					}
				}
			}
			gameState = MainMenu;
		}
	}

	renderer->Render();
	Debug::UpdateRenderables(dt);
}

void TutorialGame::ShowLose(float dt) {
	Debug::Print("You lose!", Vector2(30, 40), Debug::BLUE);
	Debug::Print("Press Enter to continue", Vector2(30, 70), Debug::BLACK);
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::RETURN)) gameState = MainMenu;
	renderer->Render();
	Debug::UpdateRenderables(dt);
}

void TutorialGame::LoadRankingFile() {
	std::ifstream timeFile(Assets::SCOREDIR + "timeMode.txt");
	std::ifstream scoreFile(Assets::SCOREDIR + "scoreMode.txt");

	while (!scoreFile.eof()) {
		std::string name;
		scoreFile >> name;
		float s;
		scoreFile >> s;
		TimeTable.emplace_back(name, s);
	}

	while (!timeFile.eof())
	{
		std::string name;
		timeFile >> name;
		float s;
		timeFile >> s;
		ScoreTable.emplace_back(name, s);
	}

	scoreFile.close();
	timeFile.close();
}

void TutorialGame::DrawAnim(GameObject* g, MeshAnimation* anim) {
	//const vector <Matrix4 > invBindPose = playerMesh->GetInverseBindPose();
	const Matrix4* invBindPose = g->GetRenderObject()->GetMesh()->GetInverseBindPose().data();
	const Matrix4* frameData = anim->GetJointData(g->GetRenderObject()->currentFrame % anim->GetFrameCount());
	vector <Matrix4 > tempMatrices;
	for (unsigned int i = 0; i < g->GetRenderObject()->GetMesh()->GetJointCount(); ++i) {
		tempMatrices.emplace_back(frameData[i] * invBindPose[i]);
	}
	g->GetRenderObject()->SetFrameMatrices(tempMatrices);
}

void TutorialGame::UpdateAnim(GameObject* g, MeshAnimation* anim) {
	while (g->GetRenderObject()->frameTime < 0.0f) {
		g->GetRenderObject()->currentFrame = (g->GetRenderObject()->currentFrame + 1) % anim->GetFrameCount();
		g->GetRenderObject()->frameTime += 1.0f / anim->GetFrameTime();
	}
	DrawAnim(g, anim);
}

void TutorialGame::UpdateBossAnim(GameObject* boss, MeshAnimation* bossAnimation, float dt) {
	if (boss != nullptr) {
		if (gameLevel->GetBoss()->getIsRencentlyHurt()) {
			boss->GetRenderObject()->frameTime -= dt/1.25;
			UpdateAnim(boss, bossFlinchAnimation);
		}
		else {
			if (iceCubeBullet->GetIsHiding() && gameLevel->GetBoss()->getShooting() && gameLevel->GetBoss()->getHasIceCubeBullet()) {
				/*boss->GetRenderObject()->frameTime -= dt;
				UpdateAnim(boss, bossShootingAnimation);*/
				playShootingAnimation = true;
			}
			if (playShootingAnimation) {
				boss->GetRenderObject()->frameTime -= dt;
				UpdateAnim(boss, bossShootingAnimation);
				shootingTimer += dt;
				if (shootingTimer >= shootingDuration) {
					playShootingAnimation = false;
					shootingTimer = 0.0f; // reset
					iceCubeBulletFrames = 0.0f;

				}
			}
			else {
				if (!iceCubeBullet->GetIsHiding()) {
					if (iceCubeBulletFrames < 30) {
						boss->GetRenderObject()->frameTime -= dt;
						UpdateAnim(boss, bossShootingAnimation);
						iceCubeBulletFrames++;
					}
					else {
						boss->GetRenderObject()->frameTime -= dt / 1.4;
						UpdateAnim(boss, bossCheersAnimation);
					}
				}
				else {
					boss->GetRenderObject()->frameTime -= dt;
					UpdateAnim(boss, bossAnimation);
					iceCubeBulletFrames = 0.0f;

				}
			}
		}
	}
}

void TutorialGame::UpdatePlayerAnim(Player* player, MeshAnimation* playerIdleAnimation, MeshAnimation* playerWalkAnimation, float dt) {
	if (player != nullptr) {
		if (player->GetIsWalk()) {
			player->GetRenderObject()->frameTime -= dt;
			UpdateAnim(player, playerWalkAnimation);
		}
		else if (player->IsJumping()) {
			player->GetRenderObject()->frameTime -= dt;
			UpdateAnim(player, playerJumpAnimation);
			// reset jump frame time!
			if (player->GetRenderObject()->frameTime < 0.0f) {
				player->GetRenderObject()->frameTime = 0.0f;
			}
		}
		else {
			player->GetRenderObject()->frameTime -= dt;
			UpdateAnim(player, playerIdleAnimation);

		}
	}
}

void TutorialGame::UpdateGhostAnim(GameObject* ghost, MeshAnimation* ghostAnimation, float dt) {
	if (ghost != nullptr) {
		ghost->GetRenderObject()->frameTime -= dt;
		UpdateAnim(ghost, ghostAnimation);
	}
}

void TutorialGame::SwitchLevel() {
	if (!physics->GetCollisionDetectionList(portal).empty() && physics->GetCollisionDetectionList(portal)[0] == player) {
		switch (currentLevel)
		{
		case 1:
			GameLevel::RemoveLevel(world, gameLevel->GetConnection(), false, false);
			gameLevel->AddLevelToWorld(world, *gameLevel->GetLevel1());
			player->GetTransform().SetPosition(Vector3(0, 10, 0)).SetOrientation(Quaternion(0.0, 0.0, 0.0, 1.0));
			player->GetPhysicsObject()->SetLinearVelocity(Vector3());
			portal = gameLevel->GetLevel1()->portal;

			ghost = gameLevel->getGhost();
			ghostAnimation = gameLevel->getGhostAnimation();
			break;
		case 2:
			gameLevel->RemoveLevel(world, gameLevel->GetLevel1(), true, false);
			gameLevel->AddLevelToWorld(world, *gameLevel->GetConnection());
			player->GetTransform().SetPosition(Vector3(0, 10, 60)).SetOrientation(Quaternion(0.0, 0.0, 0.0, 1.0));
			player->GetPhysicsObject()->SetLinearVelocity(Vector3());
			portal = gameLevel->GetConnection()->portal;
			break;
		case 3:
			gameLevel->RemoveLevel(world, gameLevel->GetConnection(), false, false);
			gameLevel->AddLevelToWorld(world, *gameLevel->GetLevel2());
			player->GetTransform().SetPosition(Vector3(0, 10, 0)).SetOrientation(Quaternion(0.0, 0.0, 0.0, 1.0));
			player->GetPhysicsObject()->SetLinearVelocity(Vector3());
			//portal = gameLevel->GetLevel2()->portal;
			break;
		case 4:
			gameLevel->RemoveLevel(world, gameLevel->GetLevel2(), true, false);
			gameLevel->AddLevelToWorld(world, *gameLevel->GetConnection());
			player->GetTransform().SetPosition(Vector3(0, 10, 60)).SetOrientation(Quaternion(0.0, 0.0, 0.0, 1.0));
			player->GetPhysicsObject()->SetLinearVelocity(Vector3());
			portal = gameLevel->GetConnection()->portal;
			break;
		case 5:
			gameLevel->RemoveLevel(world, gameLevel->GetConnection(), false, false);
			gameLevel->AddLevelToWorld(world, *gameLevel->GetLevel3());
			player->GetTransform().SetPosition(Vector3(0, 10, 0)).SetOrientation(Quaternion(0.0, 0.0, 0.0, 1.0));
			player->GetPhysicsObject()->SetLinearVelocity(Vector3());
			//portal = gameLevel->GetLevel3()->portal;

			boss = gameLevel->GetBoss();
			bossAnimation = gameLevel->getBossAnimation();
			iceCubeBullet = gameLevel->getIceCubeBullet();
			break;
		case 6:
			gameLevel->RemoveLevel(world, gameLevel->GetLevel3(), true, false);
			gameLevel->AddLevelToWorld(world, *gameLevel->GetConnection());
			player->GetTransform().SetPosition(Vector3(0, 10, 0)).SetOrientation(Quaternion(0.0, 0.0, 0.0, 1.0));
			player->GetPhysicsObject()->SetLinearVelocity(Vector3());
			portal = gameLevel->GetConnection()->portal;
			break;
		case 7:
			gameLevel->RemoveLevel(world, gameLevel->GetConnection(), true, false);
			gameLevel->AddLevelToWorld(world, 0, true, false);
			gameLevel->AddLevelToWorld(world, 0, false, false);
			player->GetTransform().SetPosition(Vector3(0, 10, 0)).SetOrientation(Quaternion(0.0, 0.0, 0.0, 1.0));
			player->GetPhysicsObject()->SetLinearVelocity(Vector3());
			break;
		default:
			break;
		}
		currentLevel++;
	}
}

void TutorialGame::UpdateTrackingBall(Vector3 ballPosition, const Vector3& playerPosition, float speed, float dt) {

	Debug::DrawLine(ballPosition, playerPosition, Debug::BLACK);
	Vector3 direction = (playerPosition - ballPosition).Normalised();

	float distance = speed * dt;
	ballPosition += direction * distance;

	iceCubeBullet->GetTransform().SetPosition(ballPosition);
}
void TutorialGame::UpdateProjMatrixFov(float df) {
	float fov = world->GetMainCamera().GetFov();
	fov -= df;
	fov = std::max(15.0f, fov);
	fov = std::min(120.0f, fov);
	world->GetMainCamera().SetFov(fov);
}

void TutorialGame::IceCubeBulletLogic(float dt) {
	if (iceCubeBullet->GetIsHiding() && gameLevel->GetBoss()->getShooting() && gameLevel->GetBoss()->getHasIceCubeBullet() && !playShootingAnimation) {
		iceCubeBullet->GetTransform().SetPosition(gameLevel->GetBoss()->GetTransform().GetPosition() + Vector3(0, 20, 30));
		std::cout << iceCubeBullet->GetTransform().GetPosition() << std::endl;
		iceCubeBullet->SetIsHiding(false);
		gameLevel->GetBoss()->setHasIceCubeBullet(false);
		iceCubeBullet->SetExistenceTime(0.0f);
	}
	if (!iceCubeBullet->GetIsHiding()) {
		iceCubeBullet->UpdateExistenceTime(dt);
		Vector3 playerPosition = player->GetTransform().GetPosition();
		Vector3 ballPosition = iceCubeBullet->GetTransform().GetPosition();
		UpdateTrackingBall(ballPosition, playerPosition, 2, dt);
		if (iceCubeBullet->GetExistenceTime() >= 6.0f) {
			iceCubeBullet->GetTransform().SetPosition(Vector3(0, -20, 0));
			gameLevel->GetBoss()->setHasIceCubeBullet(true);
			iceCubeBullet->SetIsHiding(true);
		}
	}
}
void TutorialGame::ExecuteAttack(float dt) {
	Vector3 targetPosition;
	Vector3 bossPosition;
	targetPosition = player->GetTransform().GetPosition();
	bossPosition = gameLevel->GetBoss()->GetTransform().GetPosition();
	Vector3 direction = (targetPosition - bossPosition).Normalised();
	Ray ray = Ray(bossPosition, direction);
	RayCollision closestCollision;
	closestCollision.rayDistance = gameLevel->GetBoss()->attackRange;
	if (world->Raycast(ray, closestCollision, true)) {
		if (closestCollision.node == player) {
			player->updateHealth(-1);
			std::cout << "now player health is " << player->GetHealth() << std::endl;
			gameLevel->GetBoss()->setIsAttack(false);
		}
	}
}