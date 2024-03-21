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

#include "PushdownState.h"

using namespace NCL;
using namespace CSC8503;

std::vector<std::string> TutorialGame::itemList;

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

	gameState = Start;
	mainMenuState = MainMenu_Start;
}
/*

Each of the little demo scenarios used in the game uses the same 2 meshes,
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/

void TutorialGame::InitialiseAssets() {
	InitCamera();
	InitAudio();
	InitWorld();
}

TutorialGame::~TutorialGame() {
	delete physics;
	delete renderer;
	delete world;
}

void TutorialGame::UpdateGame(float dt) {
	//Debug::DrawCollisionBox(player);
	//Debug::DrawCollisionBox(cameraCollision);

	if (player) {
		if (PlayerPreHealth > player->GetHealth()) {
			PlayerPreHealth -= 0.5f;
		}
		isWalking = player->GetIsWalk();
		if (PlayerPreHealth < player->GetHealth()) {
			PlayerPreHealth = player->GetHealth();
		}
	}
	if (BossPrehHealth != gameLevel->GetBoss()->getBossHealth()) {
		BossPrehHealth -= 0.5f;
	}
	
	Debug::DrawLine(Vector3(), Vector3(100, 0, 0), Debug::RED);
	Debug::DrawLine(Vector3(), Vector3(0, 100, 0), Debug::GREEN);
	Debug::DrawLine(Vector3(), Vector3(0, 0, 100), Debug::BLUE);
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::P)) {
		gameState = Pause;
		return;
	}
	if (!inSelectionMode) world->GetMainCamera().UpdateCamera(dt);
	if (lockedObject != nullptr) LockedObjectMovement(dt);
	if (lockedObject == nullptr) Debug::Print("Press F to lock camera", Vector2(5, 80));
	else Debug::Print("Press F to free camera", Vector2(5, 80));
	/*playerPosition = player->GetTransform().GetPosition();*/
	mainCameraPosition = world->GetMainCamera().GetPosition();
	if (isWalking) {
		FMOD_VECTOR position = { mainCameraPosition.x, mainCameraPosition.y, mainCameraPosition.z };
		if (!lastWalkingState) {
			soundManager.play3DSound("walking", position);
			soundManager.setSoundVolume("walking", 0.8f);
			soundManager.setSoundSpeed("walking", 0.8f);
		}
		else {
			soundManager.update3DSoundPosition("walking", position);
		}
		UpdateListenerPosition(dt, mainCameraPosition);
	}
	else if (lastWalkingState) {
		soundManager.stopSound("walking");
	}
	lastWalkingState = isWalking;
	if (currentLevel == 4) {
		Vector3 pos1 = gameLevel->GetGhostai()->GetTransform().GetPosition();
		FMOD_VECTOR pos11 = { pos1.x,pos1.y, pos1.z };
		Vector3 pos2 = gameLevel->GetGhostai2()->GetTransform().GetPosition();
		FMOD_VECTOR pos22 = { pos2.x,pos2.y, pos2.z };
		if (!soundManager.isSoundPlaying("ghost")) {
			soundManager.play3DSound("ghost", pos11);
			soundManager.setSoundVolume("ghost", 0.4f);
			soundManager.play3DSound("ghost", pos22);
			soundManager.setSoundVolume("ghost", 0.4f);
		}
		else {
			soundManager.update3DSoundPosition("ghost", pos11);
			soundManager.update3DSoundPosition("ghost", pos22);
		}
	}
	if (currentLevel == 5) {
		soundManager.stopSound("ghost");
	}
	soundManager.update();
	totalTime += dt;
	skilltime += dt;
	SelectObject();
	MoveSelectedObject();

	UpdateLevel(dt);
	UpdateKeys(dt);
	world->UpdateWorld(dt);
	renderer->Update(dt);
	physics->Update(dt);
	renderer->Render();
	Debug::UpdateRenderables(dt);
	GameTechRenderer::UpdateUI();

	if (!isDebug) SwitchLevel();
	if (PlayerPreHealth < player->GetHealth()) {
		PlayerPreHealth = player->GetHealth();
	}
	
	if (skilltime > 5) {
		skilltime = 5;
	}

	if (skilltime == 5) {
		useskill = true;
	}

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
		std::cout << "Boss's Health:" << gameLevel->GetBoss()->getBossHealth() << std::endl;
		gameLevel->GetBoss()->SetIsRencentlyHurt(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::H)) {
		gameLevel->GetBoss()->decreaseBossHealth(20.0f);
		std::cout << "Boss's Health:" << gameLevel->GetBoss()->getBossHealth() << std::endl;
		gameLevel->GetBoss()->SetIsRencentlyHurt(true);
	}
	if (lockedObject) {
		//LockedObjectMovement(dt);
	}
	else {
		DebugObjectMovement();
	}
}

void TutorialGame::LockedObjectMovement(float dt) {
	player->forceToBeAdded = Vector3();
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


	//cameraCollision->GetTransform().SetPosition(campos + Vector3(0, 7, 3));

	//physics->Update(dt);

	/*Ray collisionRay = Ray(targetpos, -camdir);
	RayCollision collisionRayData;
	if (world->Raycast(collisionRay, collisionRayData, true, lockedObject))
	{
		if (collisionRayData.rayDistance < 6)
			campos = targetpos - camdir * (collisionRayData.rayDistance - 1.0f);
	}*/

	if (Window::GetKeyboard()->KeyDown(KeyCodes::W)) {
		player->SetIsWalk(true);
		//player->getIsAccelerated()?lockedObject->GetPhysicsObject()->AddForce(-fwdAxis*3): lockedObject->GetPhysicsObject()->AddForce(-fwdAxis*1.5);
		player->getIsAccelerated() ? player->forceToBeAdded+=(-fwdAxis * 3) : player->forceToBeAdded += (-fwdAxis * 1.5);
		lockedObject->GetTransform().SetOrientation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (Window::GetKeyboard()->KeyDown(KeyCodes::S)) {
		player->SetIsWalk(true);
		player->getIsAccelerated() ? player->forceToBeAdded += (fwdAxis * 3) : player->forceToBeAdded += (fwdAxis* 1.5);
		//player->getIsAccelerated() ? lockedObject->GetPhysicsObject()->AddForce(fwdAxis * 3) : lockedObject->GetPhysicsObject()->AddForce(fwdAxis* 1.5);
		lockedObject->GetTransform().SetOrientation(Quaternion(0.0f, 1.0f, 0.0f, 0.0f));
	}
	else if (Window::GetKeyboard()->KeyDown(KeyCodes::A)) {
		player->SetIsWalk(true);
		player->getIsAccelerated() ? player->forceToBeAdded += (-rightAxis * 3) : player->forceToBeAdded += (-rightAxis* 1.5);
		//player->getIsAccelerated() ? lockedObject->GetPhysicsObject()->AddForce(-rightAxis * 3) : lockedObject->GetPhysicsObject()->AddForce(-rightAxis* 1.5);
	}
	else if (Window::GetKeyboard()->KeyDown(KeyCodes::D)) {
		player->SetIsWalk(true);
		player->getIsAccelerated() ? player->forceToBeAdded += (rightAxis * 3) : player->forceToBeAdded += (rightAxis* 1.5);
		//player->getIsAccelerated() ? lockedObject->GetPhysicsObject()->AddForce(rightAxis * 3) : lockedObject->GetPhysicsObject()->AddForce(rightAxis* 1.5);
	}
	else if (Window::GetKeyboard()->KeyDown(KeyCodes::SPACE)) {
		if (player->GetCanJump())
		{
			player->isSpacePressed = true;
			/*Vector3 velocity = lockedObject->GetPhysicsObject()->GetLinearVelocity();
			lockedObject->GetPhysicsObject()->SetLinearVelocity(Vector3(velocity.x, 24, velocity.z));*/
			player->SetCanJump(false);
			player->setJumpTimer(1.1f);
			player->SetIsJumping(true);
		}
	}
	else {

		if (player->IsJumping()) {
			if (player->updateJumpTimer(dt)) {
				player->isSpacePressed = false;
				player->SetIsJumping(false);
			}
		}
		player->SetIsWalk(false);
		if (!player->getIsBeingHitBack() && !player->getIsMeleeAttacked()) {
			float velocityY = player->GetPhysicsObject()->GetLinearVelocity().y;
			player->GetPhysicsObject()->SetLinearVelocity(Vector3(0, velocityY, 0));
		}

	}

	if (Window::GetKeyboard()->KeyPressed(KeyCodes::NUM1)) {
		player->UseItem(0);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::NUM2)) {
		player->UseItem(1);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::NUM3)) {
		player->UseItem(2);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::NUM4)) {
		player->UseItem(3);
		//speedProp = gameLevel->CreateSpeedProp(Vector3(0, 5, 65), Vector3(16, 16, 16));

		//world->AddGameObject(speedProp);

		//propList.push_back(speedProp);

		//player->SetIsAccelerated(true);
	}

	if (useskill) {

		if (Window::GetKeyboard()->KeyDown(KeyCodes::Q)) {
			if (progress < 4) {
				progress += 0.03;
			}
			if (progress > 4) {
				progress = 4;
			}
			skilltime = 0;
		}
		else {
			progress = 0;
		}

		Matrix4 viewMat = Matrix4::BuildViewMatrix(campos, targetpos, Vector3(0, 1, 0)).Inverse();/*cameraCollision->GetTransform().GetPosition()*/
		Quaternion q(viewMat);
		float pitch = q.ToEuler().x;
		float yaw = q.ToEuler().y;

		Quaternion lookat = Quaternion::EulerAnglesToQuaternion(0, yaw, 0);
		lockedObject->GetTransform().SetOrientation(lookat);

	player->orientationNetPlayer = lookat;

	world->GetMainCamera().SetPosition(campos + Vector3(0, 5, 3));
	world->GetMainCamera().SetPitch(pitch);
	world->GetMainCamera().SetYaw(yaw);
	//renderer.UpdateProjMatrixFov(Window::GetMouse()->GetWheelMovement());
	UpdateProjMatrixFov(Window::GetMouse()->GetWheelMovement());

		world->GetMainCamera().SetPosition(campos + Vector3(0, 5, 3));/*cameraCollision->GetTransform().GetPosition()+Vector3(0,5,0)*/
		world->GetMainCamera().SetPitch(pitch);
		world->GetMainCamera().SetYaw(yaw);
		//renderer.UpdateProjMatrixFov(Window::GetMouse()->GetWheelMovement());
		UpdateProjMatrixFov(Window::GetMouse()->GetWheelMovement());

	}
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

		//if (Window::GetKeyboard()->KeyDown(KeyCodes::NUMPAD2)) {
		//	selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 0, -10));
		//}

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

		//if (Window::GetKeyboard()->KeyDown(KeyCodes::NUMPAD2)) {
		//	player->GetPhysicsObject()->AddTorque(Vector3(0, 0, -10));
		//}

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
	gameLevel = new GameLevel(renderer);
	gameLevel->AddLevelToWorld(world, gameLevel->GetGeneric());
	player = gameLevel->GetPlayer();
	playerlist = gameLevel->GetPlayerList();
	//localplayer = gameLevel->GetPlayer();
	playerWalkAnimation = gameLevel->getplayerWalkAnimation();
	playerIdleAnimation = gameLevel->getplayerIdleAnimation();
	playerJumpAnimation = gameLevel->getplayerJumpAnimation();
	//cameraCollision = gameLevel->getCamreaCollision();

	/*
		Please switch the debug mode here
	*/
	
	//isDebug = true;
	isDebug = false;
	int debugLevel =3;

	if (isDebug) {
		switch (debugLevel)
		{
		case 1:
			//Level 1
			currentLevel = 4;
			gameLevel->AddLevelToWorld(world, *gameLevel->GetLevel1());
			ghostai = gameLevel->GetGhostai();
			ghostai2 = gameLevel->GetGhostai2();
			ghostAnimation = gameLevel->getGhostAnimation();
			break;
		case 2:
			//Level 2
			currentLevel = 2;
			gameLevel->AddLevelToWorld(world, *gameLevel->GetLevel2());
			break;
		case 3:
			//Level 3
			currentLevel = 6;
			gameLevel->AddLevelToWorld(world, *gameLevel->GetLevel3());
			boss = gameLevel->GetBoss();
			shield = gameLevel->GetShield();
			bossAnimation = gameLevel->getBossAnimation();
			bossCheersAnimation = gameLevel->getBossCheersAnimation();
			bossShootingAnimation = gameLevel->getBossShootingAnimation();
			bossFlinchAnimation = gameLevel->getBossFlinchAnimation();
			bossAttackingAnimation = gameLevel->getBossAttackingAnimation();
			bossChasingAnimation = gameLevel->getBossChasingAnimation();
			bossAngryAnimation = gameLevel->getBossAngryAnimation();
			bossDeathAnimation = gameLevel->getBossDeathAnimation();
			iceCubeBullet = gameLevel->getIceCubeBullet();
			fireBallBullet = gameLevel->getFireBallBullet();
			PlayerPreHealth = player->GetHealth();
			BossPrehHealth = gameLevel->GetBoss()->getBossHealth();
			break;
		case 4:
			//Level 4 initial function
			currentLevel = 8;
			player->GetTransform().SetPosition(Vector3(-70, 10, -50));
			gameLevel->AddLevelToWorld(world, 0, true, false);
			gameLevel->AddLevelToWorld(world, 0, false, false);
			hasRotation = true;
			hasReverse = false;
			break;
		default:
			break;
		}
	}
	else {
		currentLevel = 1;
		gameLevel->AddLevelToWorld(world, *gameLevel->GetConnection());
		portal = gameLevel->GetConnection()->portal;
		lockedObject = player;
		PlayLevelBGM("level0");

	}

	score = 0;
	totalTime = 0.0f;
	skilltime = 0.0f;
}

/*
Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around.

*/
bool TutorialGame::SelectObject() {
	//if (Window::GetKeyboard()->KeyPressed(KeyCodes::Q)) {
	//	inSelectionMode = !inSelectionMode;
	//	if (inSelectionMode) {
	//		Window::GetWindow()->ShowOSPointer(true);
	//		Window::GetWindow()->LockMouseToWindow(false);
	//	}
	//	else {
	//		Window::GetWindow()->ShowOSPointer(false);
	//		Window::GetWindow()->LockMouseToWindow(true);
	//	}
	//}
	//if (inSelectionMode) {
	//	Debug::Print("Press Q to change to camera mode!", Vector2(5, 85));

	//	if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::Left)) {
	//		if (selectionObject) {	//set colour to deselected;
	//			selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
	//			selectionObject = nullptr;
	//		}

	//		Ray ray = CollisionDetection::BuildRayFromMouse(world->GetMainCamera());

	//		RayCollision closestCollision;
	//		if (world->Raycast(ray, closestCollision, true)) {
	//			selectionObject = (GameObject*)closestCollision.node;

	//			selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
	//			return true;
	//		}
	//		else {
	//			return false;
	//		}
	//	}
	//	//if (Window::GetKeyboard()->KeyPressed(NCL::KeyCodes::L)) {
	//	//	if (selectionObject) {
	//	//		if (lockedObject == selectionObject) {
	//	//			lockedObject = nullptr;
	//	//		}
	//	//		else {
	//	//			lockedObject = selectionObject;
	//	//		}
	//	//	}
	//	//}
	//}
	//else {
	//	Debug::Print("Press Q to change to select mode!", Vector2(5, 85));
	//}
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
	case NCL::CSC8503::MainMenu_Exit:
		Debug::Print("Exit", Vector2(30, 50), Debug::RED);
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

void TutorialGame::SelectGameMode(float dt) {
	Debug::Print("Mode Select: ", Vector2(30, 30), Debug::BLUE);
	if (gameMode == Multi) {
		Debug::Print("Multi Player", Vector2(30, 50), Debug::RED);
		Debug::Print("Single player", Vector2(30, 60), Debug::BLACK);
	}
	else if (gameMode == Single) {
		Debug::Print("Multi Player", Vector2(30, 50), Debug::BLACK);
		Debug::Print("Single player", Vector2(30, 60), Debug::RED);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::UP))   gameMode = Multi;
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::DOWN)) gameMode = Single;

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

void TutorialGame::ShowEnd(float dt) {
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::RETURN)) {
		gameState = MainMenu;
		return;
	}

	totalTime += dt;
	float sceneAlpha, fontAlpha;
	totalTime < 3 ? sceneAlpha = totalTime / 3 : sceneAlpha = 1;
	GameTechRenderer::CreateGameUI({ Vector3(-1,1,-1),Vector3(-1,-1,-1),Vector3(1,-1,-1),Vector3(1,1,-1) }, "changingblood", "blank", sceneAlpha);

	if (totalTime > 3.5) {
		Debug::Print("Thanks For Playing!", Vector2(30, 30));
		Debug::Print("Press Enter to Mainmenu", Vector2(30, 40));
	}

	renderer->Render();
	Debug::UpdateRenderables(dt);
	GameTechRenderer::UpdateUI();
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

void TutorialGame::UpdateBossAnim(Boss* boss, MeshAnimation* bossAnimation, float dt) {
	if (boss != nullptr && !boss->getIsDead()) {
		if (gameLevel->GetBoss()->getIsRencentlyHurt()) {
			boss->GetRenderObject()->frameTime -= dt / 2.0;
			UpdateAnim(boss, bossFlinchAnimation);
		}
		else {
			bool iceCubeBulletShooting = iceCubeBullet->GetIsHiding() && fireBallBullet->GetIsHiding() && gameLevel->GetBoss()->getShooting() && gameLevel->GetBoss()->getHasIceCubeBullet();
			bool fireBallBulletShooting = fireBallBullet->GetIsHiding() && iceCubeBullet->GetIsHiding() && gameLevel->GetBoss()->getShooting() && gameLevel->GetBoss()->getHasFireBallBullet();
			if (iceCubeBulletShooting || fireBallBulletShooting) {
				/*boss->GetRenderObject()->frameTime -= dt;
				UpdateAnim(boss, bossShootingAnimation);*/
				playShootingAnimation = true;
			}
			if (playShootingAnimation) {
				if (gameLevel->GetBoss()->getIsShootingIceCube()) {
					boss->GetRenderObject()->frameTime -= dt / 1.6;
					UpdateAnim(boss, bossShootingAnimation);
					shootingTimer += dt;
					if (shootingTimer >= shootingDuration) {
						playShootingAnimation = false;
						shootingTimer = 0.0f; // reset
						iceCubeBulletFrames = 0.0f;
					}
				}
				else if (gameLevel->GetBoss()->getIsShootingFireBall()) {
					boss->GetRenderObject()->frameTime -= dt;
					UpdateAnim(boss, bossAngryAnimation);
					std::cout << "Boss is angry, be ready to counter the FIREBALL!" << std::endl;
					bossAngryTimer += dt;
					if (bossAngryTimer >= bossAngryDuration) {
						boss->GetRenderObject()->frameTime -= dt / 1.6;
						UpdateAnim(boss, bossShootingAnimation);
						shootingTimer += dt;
						if (shootingTimer >= shootingDuration) {
							playShootingAnimation = false;
							shootingTimer = 0.0f; // reset
							iceCubeBulletFrames = 0.0f;
						}
					}

				}

			}
			else {
				if ((!iceCubeBullet->GetIsHiding() || !fireBallBullet->GetIsHiding()) && !gameLevel->GetBoss()->getIsChasing()) {
					if (iceCubeBulletFrames < 20) {
						boss->GetRenderObject()->frameTime -= dt;
						UpdateAnim(boss, bossShootingAnimation);
						iceCubeBulletFrames++;
					}
					else {
						boss->GetRenderObject()->frameTime -= dt / 1.5;
						UpdateAnim(boss, bossCheersAnimation);
					}
				}
				else if (gameLevel->GetBoss()->getIsChasing()) {
					if (gameLevel->GetBoss()->getIsAttack()) {
						if (attackingTimer < attackingDuration) {
							boss->GetRenderObject()->frameTime -= dt;
							UpdateAnim(boss, bossAttackingAnimation);
							attackingTimer += dt;
							if (attackingTimer > 0.38f) {
								playerIsHit = true;
							}
						}
						else {
							gameLevel->GetBoss()->setIsAttack(false);
						}
					}
					else {
						boss->GetRenderObject()->frameTime -= dt * 1.5;
						UpdateAnim(boss, bossChasingAnimation);
						attackingTimer = 0;
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
	else {
		bossDeathTimer += dt;
		if (bossDeathTimer < bossDeathDuration) {
			boss->GetRenderObject()->frameTime -= dt;
			UpdateAnim(boss, bossDeathAnimation);
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
	if (!portal->isEnable) return;
	if (!physics->GetCollisionDetectionList(portal).empty() && physics->GetCollisionDetectionList(portal)[0] == player) {
		switch (currentLevel)
		{
		case 1:
			gameLevel->RemoveLevel(world, gameLevel->GetConnection(), false);
			gameLevel->AddLevelToWorld(world, *gameLevel->GetLevel2());
			player->GetTransform().SetPosition(Vector3(235, 10, 175)).SetOrientation(Quaternion(0.0, 0.0, 0.0, 1.0));
			player->GetPhysicsObject()->SetLinearVelocity(Vector3());
			portal = gameLevel->GetLevel2()->portal;
			PlayLevelBGM("level2");
			currentLevel++;
			break;
		case 2:
			gameLevel->RemoveLevel(world, gameLevel->GetLevel2(), true);
			gameLevel->AddLevelToWorld(world, *gameLevel->GetConnection());
			player->GetTransform().SetPosition(Vector3(0, 10, 60)).SetOrientation(Quaternion(0.0, 0.0, 0.0, 1.0));
			player->GetPhysicsObject()->SetLinearVelocity(Vector3());
			portal = gameLevel->GetConnection()->portal;
			PlayLevelBGM("level0");
			currentLevel++;
			break;
		case 3:
			GameLevel::RemoveLevel(world, gameLevel->GetConnection(), false);
			gameLevel->AddLevelToWorld(world, *gameLevel->GetLevel1());
			player->GetTransform().SetPosition(Vector3(0, 10, 0)).SetOrientation(Quaternion(0.0, 0.0, 0.0, 1.0));
			player->GetPhysicsObject()->SetLinearVelocity(Vector3());
			portal = gameLevel->GetLevel1()->portal;
			portal->isEnable = false;
			portal->GetRenderObject()->SetColour(Debug::RED);
			ghostai = gameLevel->GetGhostai();
			ghostai2 = gameLevel->GetGhostai2();
			ghostAnimation = gameLevel->getGhostAnimation();
			PlayLevelBGM("level1");
			currentLevel++;
			break;
		case 4:
			gameLevel->RemoveLevel(world, gameLevel->GetLevel1(), true);
			gameLevel->AddLevelToWorld(world, *gameLevel->GetConnection());
			player->GetTransform().SetPosition(Vector3(0, 10, 60)).SetOrientation(Quaternion(0.0, 0.0, 0.0, 1.0));
			player->GetPhysicsObject()->SetLinearVelocity(Vector3());
			portal = gameLevel->GetConnection()->portal;
			PlayLevelBGM("level0");
			currentLevel++;
			break;
		case 5:
			gameLevel->RemoveLevel(world, gameLevel->GetConnection(), false);
			gameLevel->AddLevelToWorld(world, *gameLevel->GetLevel3());
			player->GetTransform().SetPosition(Vector3(0, 4, 135)).SetOrientation(Quaternion(0.0, 0.0, 0.0, 1.0));
			player->GetPhysicsObject()->SetLinearVelocity(Vector3());
			portal = gameLevel->GetLevel3()->portal;
			portal->isEnable = false;
			portal->GetRenderObject()->SetColour(Debug::RED);
			boss = gameLevel->GetBoss();
			shield = gameLevel->GetShield();
			bossAnimation = gameLevel->getBossAnimation();
			iceCubeBullet = gameLevel->getIceCubeBullet();
			bossCheersAnimation = gameLevel->getBossCheersAnimation();
			bossShootingAnimation = gameLevel->getBossShootingAnimation();
			bossFlinchAnimation = gameLevel->getBossFlinchAnimation();
			bossAttackingAnimation = gameLevel->getBossAttackingAnimation();
			bossChasingAnimation = gameLevel->getBossChasingAnimation();
			fireBallBullet = gameLevel->getFireBallBullet();
			bossAngryAnimation = gameLevel->getBossAngryAnimation();
			bossDeathAnimation = gameLevel->getBossDeathAnimation();
			PlayerPreHealth = player->GetHealth();
			BossPrehHealth = gameLevel->GetBoss()->getBossHealth();
			PlayLevelBGM("level3");
			currentLevel++;
			break;
		case 6:
			gameLevel->RemoveLevel(world, gameLevel->GetLevel3(), true);
			for (const auto& element : BasicExamples::propList) {
				world->RemoveGameObject(element);
			}
			gameLevel->AddLevelToWorld(world, *gameLevel->GetConnection());
			player->GetTransform().SetPosition(Vector3(0, 10, 0)).SetOrientation(Quaternion(0.0, 0.0, 0.0, 1.0));
			player->GetPhysicsObject()->SetLinearVelocity(Vector3());
			portal = gameLevel->GetConnection()->portal;
			PlayLevelBGM("level0");
			currentLevel++;
			break;
		case 7:
			gameLevel->RemoveLevel(world, gameLevel->GetConnection(), true);
			gameLevel->AddLevelToWorld(world, 0, true, false);
			gameLevel->AddLevelToWorld(world, 0, false, false);
			player->GetTransform().SetPosition(Vector3(-70, 10, -50)).SetOrientation(Quaternion(0.0, 0.0, 0.0, 1.0));
			player->GetPhysicsObject()->SetLinearVelocity(Vector3());
			PlayLevelBGM("level4");
			if (exit != nullptr) {
				delete exit;
				exit = nullptr;
			}
			currentLevel++;
			break;
		default:
			break;
		}
	}
}

void TutorialGame::UpdateLevel(float dt) {
	// update player
	player->UpdatePlayer(dt);
	UpdatePlayerAnim(player, playerIdleAnimation, playerWalkAnimation, dt);

	Vector3 playerPosition;
	playerPosition = player->GetTransform().GetPosition() + Vector3(0,1,0);
	Vector3 direction = Vector3(0, -1, 0);
	Ray ray = Ray(playerPosition, direction);
	RayCollision closestCollision;
	//closestCollision.rayDistance = gameLevel->GetBoss()->attackRange;
	if (world->Raycast(ray, closestCollision, true)) {
		GameObject* blocker = (GameObject*)closestCollision.node;
		if (blocker->GetName() == "floor"|| blocker->GetName() == "aabb") {
			float distance = playerPosition.y - blocker->GetTransform().GetPosition().y - blocker->GetTransform().GetScale().y / 2;
			if (distance < 2) {
				player->GetPhysicsObject()->AddForce(-physics->GetGravity() * 0.2);
			}
		}
	}
	// Level 1 stuff
	if (currentLevel == 4) {
		ghostai->Update(dt);
		ghostai2->Update(dt);
		UpdateGhostAnim(ghostai, ghostAnimation, dt);
		UpdateGhostAnim(ghostai2, ghostAnimation, dt);
		if (portal && !portal->isEnable) {
			portal->isEnable = gameLevel->CheckCoinList();
		}
		else {
			if (!isDebug) portal->GetRenderObject()->SetColour(Debug::GREEN);
		}

	}
	// Level 2
	else if (currentLevel == 2) {
		for (const auto& element : gameLevel->GetL2Doors()) {
			element->Update(dt);
			if (element->GetState() != "keepState" && element->GetTimer() - dt == 0) {
				Vector3 pos = element->GetTransform().GetPosition();
				FMOD_VECTOR poss = { pos.x,pos.y, pos.z };
				soundManager.play3DSound("door",poss);
				soundManager.setSoundVolume("door", 2.0f);
			}
		}
	}
	// Level 3
	else if (currentLevel == 6) {
		gameLevel->GetBoss()->Update(dt);
		UpdateBossAnim(gameLevel->GetBoss(), bossAnimation, dt);
		propSpawnTimer += dt;
		if (boss->getIsDead()) {
			portal->isEnable = true;
			portal->GetRenderObject()->SetColour(Debug::GREEN);
			portal->GetTransform().SetPosition(Vector3(0, 3, 100));
		}
		if (propSpawnTimer > propSpawnCooldown) {
			GenerateRandomPropPositionInBounds(Vector3(-80, 1, -80), Vector3(80, 1, 80));
			propSpawnTimer = 0.0f;
		}
		if (boss->GetStunTimer() >= 0.5f && std::fmod(boss->GetStunTimer(), 0.5f) < dt&& boss->GetStunTimer()!=0.0f) {
			DropItems();
		}
		if (gameLevel->GetBoss()->getIsDroppingMassiveItems()) {
			DropMassiveItems();
			DropMassiveItems();
			gameLevel->GetBoss()->setIsDroppingMassiveItems(false);
		}
		if (player->getIsRollingRock()) {
			Quaternion playerQuaternion = player->GetTransform().GetOrientation();
			Vector3 defaultForward = Vector3(0, 0, -1);
			Vector3 currentDirection = playerQuaternion * defaultForward;
			rollingRock = gameLevel->CreateRollingRock(player->GetTransform().GetPosition() + currentDirection.Normalised() * 10 + Vector3(0, 5, 0), 4);

			world->AddGameObject(rollingRock);
			if (rollingRock) {
				RollStone(rollingRock, currentDirection, 72000);
				player->SetIsRollingRock(false);
			}
		}
		if (player->getIsAccelerated()) {
			speedPropTimer += dt;
			if (speedPropTimer > speedPropDuration) {
				player->SetIsAccelerated(false);
				speedPropTimer = 0.0f;
			}
		}
		if (fireBallBullet->GetIsHiding() && iceCubeBullet->GetIsHiding()) {
			boss->updateBulletTimer(dt);
		}
		if (gameLevel->GetBoss()->getShooting()) {
			if (boss->getNextBullet() == 0) {
				gameLevel->GetBoss()->setIsShootingIceCube(true);
				IceCubeBulletLogic(dt);
			}
			else {
				gameLevel->GetBoss()->setIsShootingFireBall(true);
				FireBallBulletLogic(dt);
			}

		}
		if (!iceCubeBullet->GetIsHiding()&& !gameLevel->GetBoss()->getShooting()) {
			iceCubeBullet->GetPhysicsObject()->AddForce(Vector3(0, 15.0f, 0));
			iceCubeBullet->UpdateExistenceTime(dt);
			Vector3 playerPosition = player->GetTransform().GetPosition() + Vector3(0, 5, 0);
			Vector3 ballPosition = iceCubeBullet->GetTransform().GetPosition();
			//UpdateTrackingBall(ballPosition, playerPosition, 2, dt);
			//if (iceCubeBullet->GetExistenceTime() >= 6.0f) {
			if (!iceCubeBullet->GetIsBlockBack()) {
				UpdateTrackingBall(ballPosition, playerPosition, 15, dt);
				std::cout << "tracking" << std::endl;
			}
			if (iceCubeBullet->GetExistenceTime() >= 10.0f) {

				gameLevel->GetBoss()->setHasIceCubeBullet(true);
				iceCubeBullet->SetIsHiding(true, Vector3(20, -98, 0));
				iceCubeBullet->GetPhysicsObject()->SetLinearVelocity(Vector3());
				iceCubeBullet->SetIsBolckBack(false);
			}
		}
		if (fireBallBullet->GetIsHiding() && iceCubeBullet->GetIsHiding()) {
			boss->updateBulletTimer(dt);
		}
		if (gameLevel->GetBoss()->getIsAttack()) {
			ExecuteAttack(dt);
		}
		if (player->getIsProtected()) {
			gameLevel->GetShield()->SetIsNotHiding();
		}
		if (!gameLevel->GetShield()->GetIsHiding()) {
			Debug::DrawCollisionBox(shield);
			UpdateShieldPosition(dt);
			shieldPropTimer += dt;
			if (shieldPropTimer > shieldPropDuration) {
				gameLevel->GetShield()->SetIsHiding(true, Vector3(0, -65, 30));
				player->SetIsProtected(false);
				shieldPropTimer = 0.0f;
			}
		}
		if (player->getIsRencentlyHurt() && player->getIsBeingHitBack()) {
			Vector3 playerPosition = player->GetTransform().GetPosition() + Vector3(0, 5, 0);
			Vector3 bossPosition = boss->GetTransform().GetPosition();
			Vector3 hurtDirection = (playerPosition - bossPosition).Normalised();
			player->GetPhysicsObject()->AddForce(hurtDirection * 500);
			player->SetIsRencentlyHurt(false);
		}
		else if (player->getIsRencentlyHurt() && player->getIsMeleeAttacked()) {
			Vector3 playerPosition = player->GetTransform().GetPosition();
			Vector3 bossPosition = boss->GetTransform().GetPosition();
			Vector3 hurtDirection = (playerPosition - bossPosition).Normalised();
			player->GetPhysicsObject()->AddForce(hurtDirection * 2000);
			player->SetIsRencentlyHurt(false);
		}
		if (player->getIsBeingHitBack()) {
			hitBackTimer += dt;
			if (hitBackTimer > hitBackDuration) {
				player->SetIsBeingHitBack(false);
				hitBackTimer = 0.0f;
			}
		}
		else if (player->getIsMeleeAttacked()) {
			meleeAttackedTimer += dt;
			if (meleeAttackedTimer > meleeAttackedDuration) {
				player->SetIsMeleeAttacked(false);
				meleeAttackedTimer = 0.0f;
			}
		}
		if (fireBallBullet->GetIsBlockBack()) {
			Vector3 playerPosition = player->GetTransform().GetPosition() + Vector3(0, 5, 0);
			Vector3 bossPosition = boss->GetTransform().GetPosition() + Vector3(0, 12, 0);
			Vector3 backDirection = (playerPosition - bossPosition).Normalised();
			fireBallBullet->GetPhysicsObject()->AddForce(-backDirection * 4000);
			fireBallBullet->SetIsBolckBack(false);
		}
		if (iceCubeBullet->GetIsBlockBack()) {
			Vector3 playerPosition = player->GetTransform().GetPosition() + Vector3(0, 5, 0);
			Vector3 bossPosition = boss->GetTransform().GetPosition() + Vector3(0, 12, 0);
			Vector3 iceCubePosition = iceCubeBullet->GetTransform().GetPosition();
			Vector3 backDirection = (playerPosition - iceCubePosition).Normalised();
			float distanceToBoss = (bossPosition - iceCubePosition).Length();
			backDirection.y *= 0.2;
			backDirection.x *= 0.6;
			backDirection.z *= 0.6;
			iceCubeBullet->GetPhysicsObject()->AddForce(-backDirection * 600);

			if (distanceToBoss < 20) {
				if (isCloseToPlayer) {
					iceCubeBullet->GetPhysicsObject()->SetLinearVelocity(Vector3());
					isCloseToPlayer = false;
				}
				UpdateTrackingBall(iceCubePosition, bossPosition, 900, dt);
			}
		}
		UpdateLevel3UI();
	}
	// Level 4
	else if (currentLevel == 8) {
		gameLevel->GetL4Door()->Update(dt);
		if (gameLevel->GetL4Door()->GetState() != "keepState" && gameLevel->GetL4Door()->GetTimer() - dt == 0) {
			Vector3 pos = gameLevel->GetL4Door()->GetTransform().GetPosition();
			FMOD_VECTOR poss = { pos.x,pos.y, pos.z };
			soundManager.play3DSound("door",poss);
			soundManager.setSoundVolume("door", 2.0f);
		}
		GameObject* beginDet = gameLevel->GetBeginArea();
		GameObject* trueEndDet = gameLevel->GetTrueEndArea();
		GameObject* falseEndDet = gameLevel->GetFalseEndArea();
		trueEndDet->GetRenderObject()->SetColour(Debug::RED);
		falseEndDet->GetRenderObject()->SetColour(Debug::GREEN);
		beginDet->GetRenderObject()->SetColour(Debug::BLUE);

		vector<GameObject*> beginCD = physics->GetCollisionDetectionList(beginDet);
		if (std::count(beginCD.begin(), beginCD.end(), player) && beginDet->isEnable) {
			beginDet->isEnable = false;
			trueEndDet->isEnable = true;
			falseEndDet->isEnable = true;
		}

		vector<GameObject*> trueEndCD = physics->GetCollisionDetectionList(trueEndDet);
		if (std::count(trueEndCD.begin(), trueEndCD.end(), player) && trueEndDet->isEnable) {
			if (!hasRotation) {
				GameLevel::RemoveLevel(world, gameLevel->GetLevel4()[0], false);
				GameLevel::RemoveLevel(world, gameLevel->GetLevel4()[1], false);
			}
			else {
				GameLevel::RemoveLevel(world, gameLevel->GetLevel4r()[0], false);
				GameLevel::RemoveLevel(world, gameLevel->GetLevel4r()[1], false);
			}
			trueEndDet->isEnable = false;
			falseEndDet->isEnable = false;
			gameLevel->GetL4Door()->Deactivation();
			if (mapIndex >= 2) {
				hasReverse = !hasReverse;
			}
			score++;
			mapIndex = static_cast<int>(RandomValue(-3, 5));
			gameLevel->AddLevelToWorld(world, mapIndex, hasRotation, hasReverse);
			hasRotation = !hasRotation;
		}

		vector<GameObject*> falseEndCD = physics->GetCollisionDetectionList(falseEndDet);
		if (std::count(falseEndCD.begin(), falseEndCD.end(), player) && falseEndDet->isEnable) {
			if (!hasRotation) {
				GameLevel::RemoveLevel(world, gameLevel->GetLevel4()[0], false);
				GameLevel::RemoveLevel(world, gameLevel->GetLevel4()[1], false);
			}
			else {
				GameLevel::RemoveLevel(world, gameLevel->GetLevel4r()[0], false);
				GameLevel::RemoveLevel(world, gameLevel->GetLevel4r()[1], false);
			}
			trueEndDet->isEnable = false;
			falseEndDet->isEnable = false;
			gameLevel->GetL4Door()->Deactivation();
			if (mapIndex < 2) {
				hasReverse = !hasReverse;
			}
			score = 0;
			mapIndex = 0;
			gameLevel->AddLevelToWorld(world, mapIndex, hasRotation, hasReverse);
			hasRotation = !hasRotation;
		}

		if (score == 5 && exit == nullptr) {
			if (!hasReverse && !hasRotation) {
				exit = gameLevel->CreateCube(Vector3(-30, 10, -70), Vector3(15, 15, 15), 0.0f);
			}
			else if (!hasReverse && hasRotation) {
				exit = gameLevel->CreateCube(Vector3(30, 10, 70), Vector3(15, 15, 15), 0.0f);
			}
			else if (hasReverse && !hasRotation) {
				exit = gameLevel->CreateCube(Vector3(70, 10, 30), Vector3(15, 15, 15), 0.0f);
			}
			else if (hasReverse && hasRotation) {
				exit = gameLevel->CreateCube(Vector3(-70, 10, -30), Vector3(15, 15, 15), 0.0f);
			}
			exit->GetRenderObject()->SetColour(Debug::WHITE * 2);
			exit->GetRenderObject()->SetDefaultTexture(nullptr, 0);
			exit->SetTag("Exit");
			world->AddGameObject(exit);
		}

		if (exit != nullptr) {
			vector<GameObject*> ExitCD = physics->GetCollisionDetectionList(exit);
			if (std::count(ExitCD.begin(), ExitCD.end(), player)) {
				totalTime = 0;
				gameState = End;
				soundManager.stopSound("level4");
				soundManager.playSound("end");
				soundManager.setSoundVolume("end", 2.0f);
			}
		}
	}
}

void TutorialGame::UpdateTrackingBall(Vector3 ballPosition, const Vector3& playerPosition, float speed, float dt) {

	Debug::DrawLine(ballPosition, playerPosition, Debug::BLACK);
	Vector3 direction = (playerPosition - ballPosition).Normalised();
	Quaternion targetOrientation = Quaternion::AxisAngleToQuaterion(Vector3(0, -1, 0), Maths::RadiansToDegrees(atan2(direction.x, -direction.z)));
	iceCubeBullet->GetTransform().SetOrientation(targetOrientation);
	iceCubeBullet->GetPhysicsObject()->AddForce(direction * speed);
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
		Quaternion orientationQuaternion = boss->GetTransform().GetOrientation();
		Vector3 forwardVector = orientationQuaternion * Vector3(0, 0, -1);
		Vector3 bulletOffset = forwardVector * 20;
		Vector3 bulletStartPosition = gameLevel->GetBoss()->GetTransform().GetPosition() + bulletOffset + Vector3(0, 10, 0);
		iceCubeBullet->GetTransform().SetPosition(bulletStartPosition);
		iceCubeBullet->SetIsNotHiding();
		gameLevel->GetBoss()->setHasIceCubeBullet(false);
		iceCubeBullet->SetExistenceTime(0.0f);
		iceCubeBullet->GetPhysicsObject()->AddForce(forwardVector * 800);
		isCloseToPlayer = true;
		std::cout << "Icecube bullet is coming!" << std::endl;
	}
	if (!iceCubeBullet->GetIsHiding()) {
		iceCubeBullet->GetPhysicsObject()->AddForce(Vector3(0, 10.0f, 0));
		iceCubeBullet->UpdateExistenceTime(dt);
		Vector3 playerPosition = player->GetTransform().GetPosition() + Vector3(0, 5, 0);
		Vector3 ballPosition = iceCubeBullet->GetTransform().GetPosition();
		//UpdateTrackingBall(ballPosition, playerPosition, 2, dt);
		//if (iceCubeBullet->GetExistenceTime() >= 6.0f) {
		if (!iceCubeBullet->GetIsBlockBack()) {
			UpdateTrackingBall(ballPosition, playerPosition, 15, dt);
			std::cout << "tracking" << std::endl;
		}
		if (iceCubeBullet->GetExistenceTime() >= 10.0f) {

			gameLevel->GetBoss()->setHasIceCubeBullet(true);
			iceCubeBullet->SetIsHiding(true, Vector3(20, -98, 0));
			iceCubeBullet->GetPhysicsObject()->SetLinearVelocity(Vector3());
			iceCubeBullet->SetIsBolckBack(false);
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
			if (playerIsHit) {
				player->updateHealth(-4);
				player->SetIsRencentlyHurt(true);
				//player->SetIsBeingHitBack(true);
				player->SetIsMeleeAttacked(true);
				playerIsHit = false;
			}
		}
	}
}

void TutorialGame::FireBallBulletLogic(float dt) {
	if (fireBallBullet->GetIsHiding() && gameLevel->GetBoss()->getShooting() && gameLevel->GetBoss()->getHasFireBallBullet() && !playShootingAnimation) {
		Quaternion orientationQuaternion = boss->GetTransform().GetOrientation();
		Vector3 forwardVector = orientationQuaternion * Vector3(0, 0, -1);
		Vector3 bulletOffset = forwardVector * 20;
		Vector3 bulletStartPosition = gameLevel->GetBoss()->GetTransform().GetPosition() + bulletOffset + Vector3(0, 20, 0);
		fireBallBullet->GetTransform().SetPosition(bulletStartPosition);
		fireBallBullet->SetIsNotHiding();
		gameLevel->GetBoss()->setHasFireBallBullet(false);
		fireBallBullet->SetExistenceTime(0.0f);
		bossAngryTimer = 0.0f;
		Vector3 playerPosition = player->GetTransform().GetPosition() + Vector3(0, 5, 0);
		Vector3 ballPosition = fireBallBullet->GetTransform().GetPosition();
		// Update position of fireball
		Vector3 fireBallDirection = (playerPosition - ballPosition).Normalised();
		fireBallBullet->GetPhysicsObject()->AddForce(fireBallDirection * 9300);
		std::cout << "Fireball bullet is coming!" << std::endl;
	}
	if (!fireBallBullet->GetIsHiding()) {
		fireBallBullet->GetPhysicsObject()->AddForce(Vector3(0, 2.0f, 0));
		// Update existence time
		fireBallBullet->UpdateExistenceTime(dt);

		// Check if fireball exists for too long
		if (fireBallBullet->GetExistenceTime() >= 4) {
			gameLevel->GetBoss()->setHasFireBallBullet(true);
			fireBallBullet->SetIsHiding(true, Vector3(0, -98, 0));
		}
	}
}


void TutorialGame::AddSound() {
	soundManager.loadSound("walking", "../externals/media/walk.mp3", true, false);
	/*soundManager.loadSound("ghost", "../externals/media/ghost.mp3", false, false);*/
	soundManager.loadSound("ghost", "../externals/media/ghost.mp3", true, false);
	soundManager.loadSound("door", "../externals/media/door.mp3", false, false);
	soundManager.loadSound("end", "../externals/media/bgm/end.mp3", true, true);
	std::map<std::string, std::string> bgmPaths = {
	   {"level1", "../externals/media/bgm/ophelia.mp3"},
	   {"level2", "../externals/media/bgm/TownTheme.mp3"},
	   {"level3", "../externals/media/bgm/BattleInTheWinter.mp3"},
	   /*{"level4", "../externals/media/bgm/jntm.mp3"},*/
		{"level4", "../externals/media/bgm/David.mp3"},
	   {"level0","../externals/media/bgm/safezone.mp3"},
	};
	soundManager.loadAllBGM(bgmPaths);
}
void TutorialGame::UpdateListenerPosition(float dt, const Vector3& playerPosition) {
	//Vector3 playerVelocity = (playerPosition - previousMainCameraPosition) / dt;
	FMOD_VECTOR position = { playerPosition.x, playerPosition.y, playerPosition.z };
	/*FMOD_VECTOR velocity = { playerVelocity.x, playerVelocity.y, playerVelocity.z };*/
	FMOD_VECTOR velocity = { 0,0,0 };
	FMOD_VECTOR forward = { std::sin(world->GetMainCamera().GetYaw()) * std::cos(world->GetMainCamera().GetPitch()),
		std::sin(world->GetMainCamera().GetPitch()), std::cos(world->GetMainCamera().GetYaw()) * std::cos(world->GetMainCamera().GetPitch()) };
	FMOD_VECTOR up = { 0, 1, 0 };

	soundManager.setListenerPosition(position, velocity, forward, up);
	previousMainCameraPosition = playerPosition;
}
void TutorialGame::InitAudio() {
	if (!soundManager.init()) {
		std::cout << "Failed to initialize SoundManager" << std::endl;
	}
	AddSound();
	/*previousPlayerPosition = player->GetTransform().GetPosition();*/
	previousMainCameraPosition = world->GetMainCamera().GetPosition();
}

void TutorialGame::PlayLevelBGM(const std::string& levelName) {
	if (!currentBGM.empty()) {
		soundManager.stopSound(currentBGM);
	}
	currentBGM = levelName;
	soundManager.playSound(currentBGM);
	if (currentBGM == "level3") {
		soundManager.setSoundVolume(currentBGM, 1.0f);
	}
	else {
		soundManager.setSoundVolume(currentBGM, 0.4f);
	}
	
}

void TutorialGame::RollStone(GameObject* stone, const Vector3& forceDirection, float forceMagnitude) {
	PhysicsObject* stonePhysics = stone->GetPhysicsObject();

	if (stonePhysics) {
		Vector3 stoneCenter = stone->GetTransform().GetPosition() + Vector3(0, 2, 0);

		Vector3 force = forceDirection * forceMagnitude;

		stonePhysics->AddForceAtPosition(force, stoneCenter);
	}
}

void TutorialGame::UpdateShieldPosition(float dt) {
	Vector3 playerPosition = player->GetTransform().GetPosition() + Vector3(0, 5, 0);
	Vector3 shieldPosition = shield->GetTransform().GetPosition();

	Vector3 direction = (playerPosition - shieldPosition).Normalised();

	Quaternion targetOrientation = Quaternion::AxisAngleToQuaterion(Vector3(0, -1, 0), Maths::RadiansToDegrees(atan2(direction.x, -direction.z)));

	shield->GetTransform().SetOrientation(targetOrientation);

	float radius = 3.0f;
	float speed = 2.5f;
	static float angle = 0.0f;
	angle += speed * dt;

	float x = playerPosition.x + radius * cos(angle);
	float y = playerPosition.y;
	float z = playerPosition.z + radius * sin(angle);

	shield->GetTransform().SetPosition(Vector3(x, y, z));
}

Vector3 TutorialGame::GenerateRandomPropPositionInBounds(const Vector3& minBound, const Vector3& maxBound) {

	srand(static_cast<unsigned int>(time(nullptr)));

	int randomNumber = rand() % 4;

	float x = std::rand() % 181 - 90; 
	float y = 3;  
	float z = std::rand() % 181 - 90;  
	GameObject* speedProp = nullptr;
	GameObject* shieldProp = nullptr;
	GameObject* rollingRockProp = nullptr;
	GameObject* redBottleProp = nullptr; 
	switch (randomNumber) {
	case 0:
		speedProp = gameLevel->CreateSpeedProp(Vector3(x, y, z), Vector3(8, 8, 8));
		world->AddGameObject(speedProp);
		break;
	case 1:
		shieldProp = gameLevel->CreateShieldProp(Vector3(x, y, z), Vector3(1, 1, 1));
		world->AddGameObject(shieldProp);
		break;
	case 2:
		rollingRockProp = gameLevel->CreateRollingRockProp(Vector3(x, y, z), 0.5);
	    world->AddGameObject(rollingRockProp);
		break;
	case 3:
		redBottleProp = gameLevel->CreateRedBottleProp(Vector3(x, y, z), Vector3(2, 2, 2));
		world->AddGameObject(redBottleProp);
		break;
	}
	
	return Vector3(x, y, z);
}

void TutorialGame::UpdateLevel3UI() {
	health = (100.0f - (player->GetHealth())) * 0.008f;
	healthLength = (100 - PlayerPreHealth) * 0.008f;

	bosshealth = (100.0f - (gameLevel->GetBoss()->getBossHealth())) * 0.01f;

	BossHealthLendth = (100 - BossPrehHealth) * 0.01; 

	GameTechRenderer::CreateGameUI({ Vector3(-0.4, -0.75f, -1.0f), Vector3(-0.4, -0.8f, -1.0f), Vector3(0.4f, -0.8f, -1.0f), Vector3(0.4f, -0.75f, -1.0f) }, "background", "health");

	GameTechRenderer::CreateGameUI({ Vector3(0.4f - health, -0.8f, -1.0f), Vector3(0.4f - health, -0.75f, -1.0f) ,
		Vector3(0.4 - healthLength , -0.75f, -1.0f), Vector3(0.4 - healthLength, -0.8f, -1.0f) }, "changingblood", "health");

	GameTechRenderer::CreateGameUI({ Vector3(-0.4, -0.75f, -1.0f), Vector3(-0.4, -0.8f, -1.0f), Vector3(0.4f - health, -0.8f, -1.0f), Vector3(0.4f - health, -0.75f, -1.0f) }, "blood", "health");

	GameTechRenderer::CreateGameUI({ Vector3(-0.4, -0.75f, -1.0f), Vector3(-0.4, -0.8f, -1.0f), Vector3(0.4f, -0.8f, -1.0f), Vector3(0.4f, -0.75f, -1.0f) }, "frame", "health");

	GameTechRenderer::CreateGameUI({ Vector3(-0.4, -0.8f, -1.0f), Vector3(-0.4, -1.0f, -1.0f), Vector3(-0.2, -1.0f, -1.0f), Vector3(-0.2f, -0.8f, -1.0f) }, "inventory1", "item");
	GameTechRenderer::CreateGameUI({ Vector3(-0.2, -0.8f, -1.0f), Vector3(-0.2, -1.0f, -1.0f), Vector3(0.0f, -1.0f, -1.0f), Vector3(0.0f, -0.8f, -1.0f) }, "inventory2", "item");
	GameTechRenderer::CreateGameUI({ Vector3(0.0, -0.8f, -1.0f), Vector3(0.0f, -1.0f, -1.0f), Vector3(0.2f, -1.0f, -1.0f), Vector3(0.2f, -0.8f, -1.0f) }, "inventory3", "item");
	GameTechRenderer::CreateGameUI({ Vector3(0.2, -0.8f, -1.0f), Vector3(0.2, -1.0f, -1.0f), Vector3(0.4f, -1.0f, -1.0f), Vector3(0.4f, -0.8f, -1.0f) }, "inventory4", "item");

	itemList = Player::GetItemList();
	
	float x = Window::GetWindow()->GetScreenSize().x;
	float y = Window::GetWindow()->GetScreenSize().y;
	float b =y/x   ;

	float distance = 0.2;
	cd = skilltime / 6.0f;

	for (int i = 0; i < itemList.size(); i++) {
		GameTechRenderer::CreateGameUI({ Vector3(-0.36f + (i * distance), -0.83f, -1.0f), Vector3(-0.36f + (i * distance), -0.96f, -1.0f),
		Vector3(-0.36f + (i * distance)+(0.2 * b), -0.96f, -1.0f), Vector3(-0.36f + (i * distance)+(0.2*b), -0.83f, -1.0f)}, itemList.at(i), "item");
	}

	GameTechRenderer::CreateGameUI({ Vector3(-0.5, 0.95f, -1.0f), Vector3(-0.5, 0.9f, -1.0f), Vector3(0.5f, 0.9f, -1.0f),
	Vector3(0.5f, 0.95f, -1.0f) }, "background", "health");

	GameTechRenderer::CreateGameUI({ Vector3(0.5f - bosshealth, 0.9f, -1.0f),Vector3(0.5f - bosshealth, 0.95f, -1.0f),
		Vector3(0.5f - BossHealthLendth, 0.95f, -1.0f),Vector3(0.5f - BossHealthLendth,0.9f, -1.0f) }, "changingblood", "health");

	GameTechRenderer::CreateGameUI({ Vector3(-0.5, 0.95f, -1.0f), Vector3(-0.5, 0.9f, -1.0f), Vector3(0.5f - bosshealth, 0.9f, -1.0f),
	Vector3(0.5f - bosshealth, 0.95f, -1.0f) }, "blood", "health");

	GameTechRenderer::CreateGameUI({ Vector3(-0.5, 0.95f, -1.0f), Vector3(-0.5, 0.9f, -1.0f), Vector3(0.5f, 0.9f, -1.0f),
		Vector3(0.5f, 0.95f, -1.0f) }, "bossframe", "health");
	GameTechRenderer::CreateGameUI({ Vector3(0.6, -0.5f, -1.0f),  Vector3(0.6, -0.5f - (0.3), -1.0f),  Vector3(0.6 + (0.3 * b), -0.5f - (0.3), -1.0f),  Vector3(0.6 + (0.3 * b), -0.5f, -1.0f) }, "skill", "skill");

	GameTechRenderer::CreateGameUI({ Vector3(0.625, -0.55f, -1.0f),  Vector3(0.625, -0.55f - (0.2), -1.0f),  Vector3(0.625 + (0.2 * b), -0.55f - (0.2), -1.0f),  Vector3(0.625 + (0.2 * b), -0.55f, -1.0f) }, "redbottle", "skill",cd+0.155);

	if (progress <= 1) {
		GameTechRenderer::CreateGameUI({ Vector3(0.64, -0.45f, -1.0f),  Vector3(0.64, -0.48, -1.0f),  Vector3(0.655f, -0.48f, -1.0f),  Vector3(0.655f, -0.45f, -1.0f) }, "power", "power", progress);
	}
	if (progress <= 2 && progress > 1) {
		GameTechRenderer::CreateGameUI({ Vector3(0.64, -0.45f, -1.0f),  Vector3(0.64, -0.48, -1.0f),  Vector3(0.655f, -0.48f, -1.0f),  Vector3(0.655f, -0.45f, -1.0f) }, "power", "power", 1);
		GameTechRenderer::CreateGameUI({ Vector3(0.6625, -0.45f, -1.0f),  Vector3(0.6625, -0.48f, -1.0f),  Vector3(0.6775f, -0.48f, -1.0f),  Vector3(0.6775f, -0.45f, -1.0f) }, "power", "power", progress - 1);
	}
	if (progress <= 3 && progress > 2) {
		GameTechRenderer::CreateGameUI({ Vector3(0.64, -0.45f, -1.0f),  Vector3(0.64, -0.48, -1.0f),  Vector3(0.655f, -0.48f, -1.0f),  Vector3(0.655f, -0.45f, -1.0f) }, "power", "power", 1);
		GameTechRenderer::CreateGameUI({ Vector3(0.6625, -0.45f, -1.0f),  Vector3(0.6625, -0.48f, -1.0f),  Vector3(0.6775f, -0.48f, -1.0f),  Vector3(0.6775f, -0.45f, -1.0f) }, "power", "power", 1);
		GameTechRenderer::CreateGameUI({ Vector3(0.685, -0.45f, -1.0f),  Vector3(0.685, -0.48f, -1.0f),  Vector3(0.70f, -0.48f, -1.0f),  Vector3(0.70f, -0.45f, -1.0f) }, "power", "power", progress - 2);
	}
	if (progress <= 4 && progress > 3) {
		GameTechRenderer::CreateGameUI({ Vector3(0.64, -0.45f, -1.0f),  Vector3(0.64, -0.48, -1.0f),  Vector3(0.655f, -0.48f, -1.0f),  Vector3(0.655f, -0.45f, -1.0f) }, "power", "power", 1);
		GameTechRenderer::CreateGameUI({ Vector3(0.6625, -0.45f, -1.0f),  Vector3(0.6625, -0.48f, -1.0f),  Vector3(0.6775f, -0.48f, -1.0f),  Vector3(0.6775f, -0.45f, -1.0f) }, "power", "power", 1);
		GameTechRenderer::CreateGameUI({ Vector3(0.685, -0.45f, -1.0f),  Vector3(0.685, -0.48f, -1.0f),  Vector3(0.70f, -0.48f, -1.0f),  Vector3(0.70f, -0.45f, -1.0f) }, "power", "power", 1);
		GameTechRenderer::CreateGameUI({ Vector3(0.705, -0.45f, -1.0f),  Vector3(0.705, -0.48f, -1.0f),  Vector3(0.720f, -0.48f, -1.0f),  Vector3(0.720f, -0.45f, -1.0f) }, "power", "power", progress - 3);
	}

}

void TutorialGame::DropMassiveItems() {
		GameObject* dropSpeedProp = nullptr;
		GameObject* dropShieldProp = nullptr;
		GameObject* dropRollingRockProp = nullptr;
		GameObject* dropRedBottleProp = nullptr;
		Vector3 bossPos = boss->GetTransform().GetPosition() + (Vector3(0, 15, 0));
		dropSpeedProp = gameLevel->CreateSpeedProp(bossPos, Vector3(8, 8, 8),2.0f);
		world->AddGameObject(dropSpeedProp);

		dropShieldProp = gameLevel->CreateShieldProp(bossPos, Vector3(1, 1, 1),2.0f);
		world->AddGameObject(dropShieldProp);

		dropRollingRockProp = gameLevel->CreateRollingRockProp(bossPos, 0.5, 2.0f);
		world->AddGameObject(dropRollingRockProp);

		dropRedBottleProp = gameLevel->CreateRedBottleProp(bossPos, Vector3(2, 2, 2), 2.0f);
		world->AddGameObject(dropRedBottleProp);

		Vector3 randomDirection1 = Vector3((rand() % 200 - 100) / 100.0f, 0, (rand() % 200 - 100) / 100.0f);
		Vector3 randomDirection2 = Vector3((rand() % 200 - 100) / 100.0f, 0, (rand() % 200 - 100) / 100.0f);
		Vector3 randomDirection3 = Vector3((rand() % 200 - 100) / 100.0f, 0, (rand() % 200 - 100) / 100.0f);
		Vector3 randomDirection4 = Vector3((rand() % 200 - 100) / 100.0f, 0, (rand() % 200 - 100) / 100.0f);
		dropSpeedProp->GetPhysicsObject()->AddForce(randomDirection1.Normalised()*6000);
		dropShieldProp->GetPhysicsObject()->AddForce(randomDirection1.Normalised() * 6000);
		dropRollingRockProp->GetPhysicsObject()->AddForce(randomDirection1.Normalised() * 6000);
		dropRedBottleProp->GetPhysicsObject()->AddForce(randomDirection1.Normalised() * 6000);
}

void TutorialGame::DropItems() {
	GameObject* dropSpeedProp = nullptr;
	GameObject* dropShieldProp = nullptr;
	GameObject* dropRollingRockProp = nullptr;
	GameObject* dropRedBottleProp = nullptr;
	Vector3 bossPos = boss->GetTransform().GetPosition() + (Vector3(0, 15, 0));


	int randomNumber = rand() % 4;
	Vector3 randomDirection = Vector3((rand() % 200 - 100) / 100.0f, 0, (rand() % 200 - 100) / 100.0f);
	switch (randomNumber) {
	case 0:
		dropSpeedProp = gameLevel->CreateSpeedProp(bossPos, Vector3(8, 8, 8),2.0f);
		world->AddGameObject(dropSpeedProp);
		dropSpeedProp->GetPhysicsObject()->AddForce(randomDirection.Normalised() * 8000);
		break;
	case 1:
		dropShieldProp = gameLevel->CreateShieldProp(bossPos, Vector3(1, 1, 1), 2.0f);
		world->AddGameObject(dropShieldProp);
		dropShieldProp->GetPhysicsObject()->AddForce(randomDirection.Normalised() * 8000);
		break;
	case 2:
		dropRollingRockProp = gameLevel->CreateRollingRockProp(bossPos, 0.5, 2.0f);
		world->AddGameObject(dropRollingRockProp);
		dropRollingRockProp->GetPhysicsObject()->AddForce(randomDirection.Normalised() * 8000);
		break;
	case 3:
		dropRedBottleProp = gameLevel->CreateRedBottleProp(bossPos, Vector3(2, 2, 2), 2.0f);
		world->AddGameObject(dropRedBottleProp);
		dropRedBottleProp->GetPhysicsObject()->AddForce(randomDirection.Normalised() * 8000);
		break;
	}

}