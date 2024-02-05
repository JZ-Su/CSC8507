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

	InitialiseMesh();
	LoadRankingFile();
	gameState = MainMenu;
	mainMenuState = MainMenu_Start;
	gameMode = TimeLimited;
	jumpTimer = 0;
}

/*
hello
Each of the little demo scenarios used in the game uses the same 2 meshes,
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void TutorialGame::InitialiseAssets() {
	InitCamera();
	InitWorld();
}

void TutorialGame::InitialiseMesh() {
	cubeMesh = renderer->LoadMesh("cube.msh");
	sphereMesh = renderer->LoadMesh("sphere.msh");
	//charMesh	= renderer->LoadMesh("goat.msh");
	charMesh = renderer->LoadMesh("Keeper.msh");
	enemyMesh = renderer->LoadMesh("Keeper.msh");
	//bonusMesh	= renderer->LoadMesh("apple.msh");
	bonusMesh = renderer->LoadMesh("goat.msh");
	capsuleMesh = renderer->LoadMesh("capsule.msh");

	basicTex = renderer->LoadTexture("checkerboard.png");
	basicShader = renderer->LoadShader("scene.vert", "scene.frag");
}

TutorialGame::~TutorialGame() {
	delete cubeMesh;
	delete sphereMesh;
	delete charMesh;
	delete enemyMesh;
	delete bonusMesh;

	delete basicTex;
	delete basicShader;

	delete physics;
	delete renderer;
	delete world;
}

void TutorialGame::UpdateGame(float dt) {
	//vector<Vector3> testnodes;
	//NavigationPath outpath;
	//bool found = GRID->FindPath(player->GetRenderObject()->GetTransform()->GetPosition(), Vector3(-25, 0, -85), outpath);
	//Vector3 pos;
	//while (outpath.PopWaypoint(pos)) testnodes.push_back(pos);
	//for (int i = 1; i < testnodes.size(); i++) {
	//	Vector3 a = testnodes[i - 1];
	//	Vector3 b = testnodes[i];
	//	Debug::DrawLine(a, b, Debug::RED);
	//}
	updateTimer(dt);
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::P)) {
		gameState = Pause;
		return;
	}
	if ((gameMode == TimeLimited && totalTime >= 150) || (gameMode == ScoreRequired && score >= 20)) {
		gameState = Win;
		return;
	}

	totalTime += dt;

	if (!inSelectionMode) {
		world->GetMainCamera().UpdateCamera(dt);
	}
	if (lockedObject != nullptr) {
		//Vector3 objPos = lockedObject->GetTransform().GetPosition();
		//Vector3 camPos = objPos + lockedOffset;

		//Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0, 1, 0));

		//Matrix4 modelMat = temp.Inverse();

		//Quaternion q(modelMat);
		//Vector3 angles = q.ToEuler(); //nearly there now!

		//world->GetMainCamera().SetPosition(camPos);
		//world->GetMainCamera().SetPitch(angles.x);
		//world->GetMainCamera().SetYaw(angles.y);
		LockedObjectMovement(dt);
	}

	UpdateKeys();

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

	if (lockedObject != nullptr) {
		Ray ray = CollisionDetection::BuildRayFromScreenCenter(world->GetMainCamera());
		RayCollision blockCollision;
		if (world->Raycast(ray, blockCollision, true)) {
			if ((GameObject*)blockCollision.node != player) {
				blocker = (GameObject*)blockCollision.node;
				Vector4 color = blocker->GetRenderObject()->GetColour();
				blocker->GetRenderObject()->SetColour(Vector4(color.x, color.y, color.z, 0.5));
			}
			else {
				if (blocker != nullptr) {
					Vector4 color = blocker->GetRenderObject()->GetColour();
					blocker->GetRenderObject()->SetColour(Vector4(color.x, color.y, color.z, 1));
					blocker = nullptr;
				}
			}
		}
	}

	if (keeperList.size() != 0) {
		for (int i = 0; i < keeperList.size(); i++) {
			((StateGameObject*)keeperList[i][0])->Update(dt); //keeper
			Vector3 position = keeperList[i][0]->GetRenderObject()->GetTransform()->GetPosition();
			keeperList[i][1]->GetRenderObject()->GetTransform()->SetPosition(Vector3(position.x, 0.3, position.z));
			//keeperList[i][2]->GetRenderObject()->GetTransform()->SetPosition(Vector3(position.x, 0.5, position.z));

			Vector3 playerPos = player->GetRenderObject()->GetTransform()->GetPosition();
			float deltaX = std::abs(playerPos.x - position.x);
			float deltaZ = std::abs(playerPos.z - position.z);
			if (deltaX <= 2 && deltaZ <= 2) {
				gameState = Lose;
				return;
			}
		}
	}

	AddBonusToWorld(totalTime);

	SelectObject();
	MoveSelectedObject();

	world->UpdateWorld(dt);
	renderer->Update(dt);
	physics->Update(dt);

	BonusCollisionDetection();
	CountScore();
	Debug::Print("Score: " + std::to_string(score), Vector2(70, 80));
	Debug::Print("Totaltime: " + std::to_string(totalTime), Vector2(70, 85));
	Debug::Print("Press P to Pause!", Vector2(70, 90));

	renderer->Render();
	Debug::UpdateRenderables(dt);
	
}

void TutorialGame::UpdateKeys() {
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

	//if (lockedObject) {
		//LockedObjectMovement();
	//}
	//else {
	DebugObjectMovement();
	//}
}

void TutorialGame::LockedObjectMovement(float dt) {
	

	Matrix4 view = world->GetMainCamera().BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();

	Vector3 camRightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	////forward is more tricky -  camera forward is 'into' the screen...
	////so we can take a guess, and use the cross of straight up, and
	////the right axis, to hopefully get a vector that's good enough!

	//Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	//fwdAxis.y = 0.0f;
	//fwdAxis.Normalise();


	//if (Window::GetKeyboard()->KeyDown(KeyCodes::UP)) {
	//	selectionObject->GetPhysicsObject()->AddForce(fwdAxis);
	//}

	//if (Window::GetKeyboard()->KeyDown(KeyCodes::DOWN)) {
	//	selectionObject->GetPhysicsObject()->AddForce(-fwdAxis);
	//}

	//if (Window::GetKeyboard()->KeyDown(KeyCodes::NEXT)) {
	//	selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
	//}
	//paste the code from tutorial script here

	Vector3 pos = lockedObject->GetTransform().GetPosition();
	Quaternion ObjOrientation = lockedObject->GetTransform().GetOrientation();

	//fwd axis of camera
	Vector3 UpAxis = Vector3(0, 1, 0);
	Vector3 fwdAxis = Vector3::Cross(camRightAxis, UpAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	//fwd axis of player
	//Vector3 fwdAxis = ObjOrientation * Vector3(0, 0, 1)  ;
	Vector3 rightAxis = Vector3::Cross(UpAxis, fwdAxis);
	//Vector3 rightAxis = ObjOrientation * Vector3(1, 0, 0)  ;

	v -= (Window::GetMouse()->GetRelativePosition().y);
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
	if (world->Raycast(collisionRay, collisionRayData, true, lockedObject))
	{
		if (collisionRayData.rayDistance < 6)
			campos = targetpos - camdir * (collisionRayData.rayDistance - 1.0f);
	}
	int speed = 5;
	if (Window::GetKeyboard()->KeyDown(KeyCodes::W)) {
		lockedObject->GetPhysicsObject()->AddForce(-fwdAxis * speed);
		player->GetTransform().SetOrientation(Quaternion(0, fwdAxis.x, 0, 1.0f));
		//player->GetTransform().SetOrientation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
		
	}

	if (Window::GetKeyboard()->KeyDown(KeyCodes::S)) {
		lockedObject->GetPhysicsObject()->AddForce(fwdAxis * speed);
		//player->GetTransform().SetOrientation(Quaternion(0, fwdAxis.x, 0, 0.0f));
		//player->GetTransform().SetOrientation(Quaternion(0.0f, 1.0f, 0.0f, 0.0f));
	}
	if (Window::GetKeyboard()->KeyDown(KeyCodes::A)) {
		lockedObject->GetPhysicsObject()->AddForce(-rightAxis * speed);
		//player->GetTransform().SetOrientation(Quaternion(0, rightAxis.x, 0, 1.0f));
		//player->GetTransform().SetOrientation(Quaternion(0.0f, 1.0f, 0.0f, 1.0f));
	}

	if (Window::GetKeyboard()->KeyDown(KeyCodes::D)) {
		lockedObject->GetPhysicsObject()->AddForce(rightAxis * speed);
		//player->GetTransform().SetOrientation(Quaternion(0, -rightAxis.x, 0, 1.0f));
		//player->GetTransform().SetOrientation(Quaternion(0.0f, -1.0f, 0.0f, 1.0f));
	}
	if (Window::GetKeyboard()->KeyDown(KeyCodes::SPACE)) {
		if (jumpTimer <=0)
		{
			setTimer(2.0f);
			lockedObject->GetPhysicsObject()->AddForce(Vector3(0, 100*7, 0));
		}


	}
	/*
	if (Window::GetKeyboard()->KeyDown(KeyCodes::NUM3)) {
		lockedObject->GetPhysicsObject()->AddTorque(Vector3(0, 3, 0));

	}
	if (Window::GetKeyboard()->KeyDown(KeyCodes::NUM4)) {
		lockedObject->GetPhysicsObject()->AddTorque(Vector3(0, -3, 0));

	}*/
	/*if (Window::GetKeyboard()->KeyDown(KeyCodes::NEXT)) {
		player->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
	}*/
	Matrix4 viewMat = Matrix4::BuildViewMatrix(campos, targetpos, Vector3(0, 1, 0)).Inverse();
	Quaternion q(viewMat);
	float pitch = q.ToEuler().x + 10.0f;
	float yaw = q.ToEuler().y + 15.0f;

	world->GetMainCamera().SetPosition(campos);
	world->GetMainCamera().SetPitch(pitch);
	world->GetMainCamera().SetYaw(yaw);
	
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
	/*else {
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

		if (Window::GetKeyboard()->KeyDown(KeyCodes::UP)) {
			player->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
			player->GetTransform().SetOrientation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::DOWN)) {
			player->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
			player->GetTransform().SetOrientation(Quaternion(0.0f, 1.0f, 0.0f, 0.0f));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::LEFT)) {
			player->GetPhysicsObject()->AddForce(Vector3(-10, 0, 0));
			player->GetTransform().SetOrientation(Quaternion(0.0f, 1.0f, 0.0f, 1.0f));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::RIGHT)) {
			player->GetPhysicsObject()->AddForce(Vector3(10, 0, 0));
			player->GetTransform().SetOrientation(Quaternion(0.0f, -1.0f, 0.0f, 1.0f));
		}
	}*/
}

/*

A single function to add a large immoveable cube to the bottom of our world

*/
GameObject* TutorialGame::AddFloorToWorld(const Vector3& position) {
	GameObject* floor = new GameObject("Floor");

	Vector3 floorSize = Vector3(100, 2, 100);
	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(floor);

	return floor;
}

GameObject* TutorialGame::AddFloorToWorld(const Vector3& position, const Vector3& floorSize) {
	GameObject* floor = new GameObject("Floor");

	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(floor);

	return floor;
}

/*

Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple'
physics worlds. You'll probably need another function for the creation of OBB cubes too.

*/
GameObject* TutorialGame::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject("Sphere");

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}

GameObject* TutorialGame::AddSquareToWorld(const Vector3& position, float radius, const Vector4& color) {
	GameObject* square = new GameObject("Square");

	//AABBVolume* volume = new AABBVolume(Vector3());
	//square->SetBoundingVolume((CollisionVolume*)volume);

	Vector3 squareSize = Vector3(2 * radius, 1, 2 * radius);

	square->GetTransform()
		.SetScale(squareSize)
		.SetPosition(position);

	square->SetRenderObject(new RenderObject(&square->GetTransform(), cubeMesh, basicTex, basicShader));
	square->SetPhysicsObject(new PhysicsObject(&square->GetTransform(), square->GetBoundingVolume()));
	square->GetRenderObject()->SetColour(color);

	world->AddGameObject(square);

	return square;
}

GameObject* TutorialGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass) {
	GameObject* cube = new GameObject("Cube");

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius) {
	GameObject* capsule = new GameObject();

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	capsule->SetBoundingVolume((CollisionVolume*)volume);

	capsule->GetTransform().SetPosition(position).SetScale(Vector3(radius, halfHeight, radius));
	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, basicTex, basicShader));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));
	capsule->GetPhysicsObject()->SetInverseMass(10.0f);
	capsule->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(capsule);

	return capsule;
}

GameObject* TutorialGame::AddEnemyToWorld(const Vector3& position) {
	float meshSize = 2.0f;
	float inverseMass = 0.5f;

	GameObject* character = new GameObject("Enemy");

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(character);

	return character;
}

GameObject* TutorialGame::AddPlayerToWorld(const Vector3& position) {
	float meshSize = 3.0f;
	float inverseMass = 0.5f;

	GameObject* character = new GameObject("Player");

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.7f, 0.3f) * meshSize);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), charMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitCubeInertia();
	player = character;
	world->AddGameObject(player);
	lockedObject = player;
	return player;
}

GameObject* TutorialGame::AddBonusToWorld(const Vector3& position) {
	/*GameObject* apple = new GameObject();

	SphereVolume* volume = new SphereVolume(0.5f);
	apple->SetBoundingVolume((CollisionVolume*)volume);
	apple->GetTransform()
		.SetScale(Vector3(2, 2, 2))
		.SetPosition(position);

	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), bonusMesh, nullptr, basicShader));
	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

	apple->GetPhysicsObject()->SetInverseMass(1.0f);
	apple->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(apple);

	return apple;*/
	float meshSize = 2.0f;
	float inverseMass = 1.0f;

	GameObject* character = new GameObject("Bonus");
	SphereVolume* volume = new SphereVolume(2.0f);

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), bonusMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	character->GetRenderObject()->SetColour(Vector4(1, 0, 1, 1));

	//world->AddGameObject(character);
	world->AddGameObjectAtBegin(character);

	return character;
}

void TutorialGame::InitCamera() {
	world->GetMainCamera().SetNearPlane(0.1f);
	world->GetMainCamera().SetFarPlane(500.0f);
	//world->GetMainCamera().SetPitch(-15.0f);
	world->GetMainCamera().SetPitch(-35.0f);
	//world->GetMainCamera().SetYaw(315.0f);
	world->GetMainCamera().SetYaw(0.0f);
	//world->GetMainCamera().SetPosition(Vector3(-60, 40, 60));
	world->GetMainCamera().SetPosition(Vector3(-60, 40, 60));
	lockedObject = nullptr;
}

void TutorialGame::InitWorld() {
	world->ClearAndErase();
	physics->Clear();
	keeperList.clear();
	blocker = nullptr;

	InitGameExamples();
	InitPlayground();

	score = 0;
	totalTime = 0.0f;
	timeInterval = 5.0f;
	GRID = new NavigationGrid("TestGrid3.txt", Vector3(-95, 2, -95));
}

void TutorialGame::InitDefaultFloor() {
	floor = AddFloorToWorld(Vector3(0, -2, 0));
	goalArea = AddFloorToWorld(Vector3(0, -2, 110), Vector3(100, 2, 10));
	goalArea->GetRenderObject()->SetColour(Vector4(0, 1, 1, 1));
}

void TutorialGame::InitGameExamples() {
	AddCapsuleToWorld(Vector3(45, 30, 45), 5, 5);
	player = AddPlayerToWorld(Vector3(0, 2, 100));
	player->GetRenderObject()->SetColour(Vector4(0.3, 0.3, 1, 1));
	LockCameraToObject(player);
	linkObjects.push_back(player);

	keeperList.push_back(AddKeeperToWorld(Vector3(-35, 0, -85), Vector3(55, 0, -15), "Keeper"));
	keeperList.push_back(AddKeeperToWorld(Vector3(75, 0, -55), Vector3(-45, 0, 55), "Keeper"));
}

void TutorialGame::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddSphereToWorld(position, radius, 1.0f);
		}
	}
	AddFloorToWorld(Vector3(0, -2, 0));
}

void TutorialGame::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 1.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);

	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);

			if (rand() % 2) {
				AddCubeToWorld(position, cubeDims);
			}
			else {
				AddSphereToWorld(position, sphereRadius);
			}
		}
	}
}

void TutorialGame::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims) {
	for (int x = 1; x < numCols + 1; ++x) {
		for (int z = 1; z < numRows + 1; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddCubeToWorld(position, cubeDims, 1.0f);
		}
	}
}

void TutorialGame::BridgeConstraintTest() {
	Vector3 cubeSize = Vector3(5, 5, 5);

	float invCubeMass = 5; //how heavy the middle pieces are
	int numLinks = 10;
	float maxDistance = 20; //constraint distance
	float cubeDistance = 20; //distance between links

	Vector3 startPos = Vector3(10, 10, 10);
	GameObject* start = AddCubeToWorld(startPos + Vector3(0, 0, 0), cubeSize, 10);
	GameObject* end = AddCubeToWorld(startPos + Vector3((numLinks + 2) * cubeDistance, 0, 0), cubeSize, 10);
	GameObject* previous = start;

	for (int i = 0; i < numLinks; i++) {
		GameObject* block = AddCubeToWorld(startPos + Vector3((i + 1) * cubeDistance, 0, 0), cubeSize, invCubeMass);
		PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);
		world->AddConstraint(constraint);
		previous = block;
	}
	PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
	world->AddConstraint(constraint);
}

std::vector<GameObject*> TutorialGame::AddKeeperToWorld(const Vector3& startPos, const Vector3& endPos, const std::string& objectName) {
	std::vector<GameObject*> objectList;
	StateGameObject* keeper = new StateGameObject(startPos, endPos, player, objectName);

	float meshSize = 3.0f;
	float inverseMass = 0.5f;
	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.7f, 0.3f) * meshSize);
	keeper->SetBoundingVolume((CollisionVolume*)volume);
	keeper->GetTransform().SetScale(Vector3(meshSize, meshSize, meshSize)).SetPosition(startPos);

	keeper->SetRenderObject(new RenderObject(&keeper->GetTransform(), enemyMesh, nullptr, basicShader));
	keeper->SetPhysicsObject(new PhysicsObject(&keeper->GetTransform(), keeper->GetBoundingVolume()));

	keeper->GetPhysicsObject()->SetInverseMass(1.0f);
	keeper->GetPhysicsObject()->InitCubeInertia();

	keeper->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));

	world->AddGameObject(keeper);

	objectList.push_back(keeper);

	GameObject* a = AddSquareToWorld(startPos + Vector3(0, 0.3, 0), 15, Vector4(1, 0.5, 0, 0.5));
	objectList.push_back(a);
	//GameObject* b = AddSquareToWorld(startPos + Vector3(0, 0.5, 0), 5, Vector4(1, 0, 0, 0.5));
	//objectList.push_back(b);

	return objectList;
	//return keeper;
}

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
		if (Window::GetKeyboard()->KeyPressed(NCL::KeyCodes::L)) {
			if (selectionObject) {
				if (lockedObject == selectionObject) {
					lockedObject = nullptr;
				}
				else {
					lockedObject = selectionObject;
				}
			}
		}
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


void TutorialGame::InitPlayground() {
	//Floor
	InitDefaultFloor();
	//Left
	AddCubeToWorld(Vector3(-95, 5, 10), Vector3(5, 5, 110), 0);
	//Right
	AddCubeToWorld(Vector3(95, 5, 10), Vector3(5, 5, 110), 0);
	//Near
	AddCubeToWorld(Vector3(0, 5, 115), Vector3(90, 5, 5), 0);
	//Far
	AddCubeToWorld(Vector3(0, 5, -95), Vector3(90, 5, 5), 0);

	AddCubeToWorld(Vector3(65, 5, 75), Vector3(25, 5, 5), 0);
	AddCubeToWorld(Vector3(-65, 5, 75), Vector3(25, 5, 5), 0);
	AddCubeToWorld(Vector3(0, 5, 75), Vector3(20, 5, 5), 0);

	AddCubeToWorld(Vector3(-70, 5, 45), Vector3(10, 5, 5), 0);
	AddCubeToWorld(Vector3(-65, 5, 20), Vector3(5, 5, 20), 0);
	AddCubeToWorld(Vector3(-60, 5, -5), Vector3(20, 5, 5), 0);

	AddCubeToWorld(Vector3(-40, 5, 35), Vector3(10, 5, 5), 0);

	AddCubeToWorld(Vector3(-25, 5, 15), Vector3(25, 5, 5), 0);
	AddCubeToWorld(Vector3(-15, 5, 35), Vector3(5, 5, 15), 0);

	AddCubeToWorld(Vector3(20, 5, 30), Vector3(10, 5, 20), 0);

	AddCubeToWorld(Vector3(65, 5, 40), Vector3(5, 5, 10), 0);
	AddCubeToWorld(Vector3(65, 5, 25), Vector3(15, 5, 5), 0);

	AddCubeToWorld(Vector3(-75, 5, -45), Vector3(5, 5, 15), 0);

	AddCubeToWorld(Vector3(-45, 5, -40), Vector3(5, 5, 20), 0);
	AddCubeToWorld(Vector3(-45, 5, -65), Vector3(15, 5, 5), 0);

	AddCubeToWorld(Vector3(-15, 5, -20), Vector3(5, 5, 20), 0);
	AddCubeToWorld(Vector3(15, 5, -5), Vector3(25, 5, 5), 0);

	AddCubeToWorld(Vector3(-15, 5, -80), Vector3(5, 5, 10), 0);

	AddCubeToWorld(Vector3(15, 5, -45), Vector3(5, 5, 25), 0);

	AddSphereToWorld(Vector3(40, 2, -40), 10.0f, 0);

	AddCubeToWorld(Vector3(50, 5, -65), Vector3(10, 5, 5), 0);
	AddCubeToWorld(Vector3(65, 5, -40), Vector3(5, 5, 40), 0);
	AddCubeToWorld(Vector3(75, 5, -35), Vector3(5, 5, 15), 0);
}

void TutorialGame::AddBonusToWorld(float totalTime) {
	if (totalTime >= timeInterval) {
		timeInterval += 5.0f;

		int x = 0, z = 0;
		Vector3 position;
		while (true) {
			x = rand() % 18 * 10 - 85;
			z = rand() % 16 * 10 - 85;
			position = Vector3(x, 2, z);
			if (!(*GRID).IsWall(position)) {
				break;
			}
		}
		AddBonusToWorld(position);
	}
}

void TutorialGame::BonusCollisionDetection() {
	std::vector<GameObject*> collisionList = physics->GetCollisionDetectionList(player);
	for (int i = 0; i < collisionList.size(); i++) {
		if (collisionList[i]->GetName() == "Bonus" && collisionList[i]->isConnected == false) {
			// Build Constraint
			collisionList[i]->isConnected = true;
			collisionList[i]->GetRenderObject()->SetColour(Vector4(0.3, 0.3, 1, 1));
			PositionConstraint* constraint = new PositionConstraint(linkObjects[linkObjects.size() - 1], collisionList[i], 5);
			world->AddConstraint(constraint);
			linkObjects.push_back(collisionList[i]);
		}
		if (collisionList[i] == goalArea) {
			world->ClearConstraint();
			for (int i = linkObjects.size() - 1; i > 0; i--) {
				if (linkObjects[i] == player) continue;
				linkObjects[i]->isConnected = false;
				linkObjects[i]->GetRenderObject()->SetColour(Vector4(1, 0, 1, 1));
			}
			linkObjects.clear();
			linkObjects.push_back(player);
		}
	}
}

void TutorialGame::CountScore() {
	std::vector<GameObject*> goalAreaCollisionList = physics->GetCollisionDetectionList(goalArea);
	std::vector<GameObject*> floorCollisionList = physics->GetCollisionDetectionList(floor);
	for (auto& i : goalAreaCollisionList) {
		if (i->GetName() == "Bonus" && i->isScore == false) {
			i->isScore = true;
			score++;
		}
	}
	for (auto& i : floorCollisionList) {
		if (i->GetName() == "Bonus" && i->isScore == true) {
			i->isScore = false;
			score--;
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
	keeperList.clear();
	linkObjects.clear();

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
	linkObjects.push_back(player);
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
	//Debug::Print("Your score:", Vector2(25, 50), Debug::BLACK);
	//Debug::Print(std::to_string(score), Vector2(45, 60), Debug::BLACK);
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

float TutorialGame::updateTimer(float dt)
{
	if (jumpTimer > 0)
	{
		jumpTimer -= dt;
	}

	return jumpTimer;
}