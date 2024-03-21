#include "BasicExamples.h"
#include "RenderObject.h"
#include "PhysicsObject.h"
#include "MeshMaterial.h"

#include <fstream>
#include "TutorialGame.h"

using namespace NCL;
using namespace CSC8503;

std::vector<GameObject*> BasicExamples::propList;

BasicExamples::BasicExamples(GameTechRenderer* render) {

	this->render = render; 

	cubeMesh    = render->LoadMesh("cube.msh");
	sphereMesh  = render->LoadMesh("sphere.msh");
	capsuleMesh = render->LoadMesh("capsule.msh");
	charMesh    = render->LoadMesh("Keeper.msh");
	bossMesh = render->LoadMesh("Male_Guard.msh");
	playerMesh = render->LoadMesh("FemaleA.msh");
	ghostMesh = render->LoadMesh("Ghost.msh");
	goatMesh    = render->LoadMesh("goat.msh");
	shieldMesh = render->LoadMesh("Shield.msh");
	rockMesh = render->LoadMesh("Rock.msh");
	speedPropMesh = render->LoadMesh("CoffeCup.msh");
	bookshelfMesh = render->LoadMesh("bookshelf.msh");
	redBottleMesh = render->LoadMesh("Bottle_Health.msh");
	tableMesh = render->LoadMesh("table.msh");
	columnMesh = render->LoadMesh("column.msh");
	stairMesh = render->LoadMesh("Stair.msh");
	handrailMesh = render->LoadMesh("handrail.msh");
	coinMesh = render->LoadMesh("coin.msh");
	wallLightMesh = render->LoadMesh("LightWall.msh");
	hangLightMesh = render->LoadMesh("LightHanging.msh");
	layerMesh = render->LoadMesh("layer.msh");

	basicTexture = render->LoadTexture("checkerboard.png");
	IceCubeTexture = render->LoadTexture("IceCube.jpg");
	shieldTexture = render->LoadTexture("Shield_AlbedoTransparency.png");
	FireBallTexture = render->LoadTexture("FireBall.jpg");
	speedPropTexture = render->LoadTexture("LowPolyFoodLiteTexture_01.png");
	redBottleTexture = render->LoadTexture("Bottle_red_diff.png");
	DefualtTexture[0] = render->LoadTexture("Defualt/white.jpg");
	DefualtTexture[1] = render->LoadTexture("Defualt/grey.jpg");
	DefualtTexture[2] = render->LoadTexture("Defualt/black.jpg");
	DefualtTexture[3] = render->LoadTexture("Defualt/halfgrey.jpg");
	DefualtTexture[4] = render->LoadTexture("Defualt/defualt_normal.png");
	floorTexture[0] = render->LoadTexture("Floor/floor_color.jpg");
	floorTexture[1] = render->LoadTexture("Floor/floor_normal.png");
	floorTexture[2] = DefualtTexture[1];
	floorTexture[3] = render->LoadTexture("Floor/floor_roughness.jpg");
	floorTexture[4] = render->LoadTexture("Floor/floor_ao.jpg");
	floorTexture[5] = render->LoadTexture("Floor/floor_height.png");
	ceilingTexture[0] = render->LoadTexture("Ceiling/ceiling_color.jpg");
	ceilingTexture[1] = render->LoadTexture("Ceiling/ceiling_normal.png");
	ceilingTexture[2] = DefualtTexture[2];
	ceilingTexture[3] = render->LoadTexture("Ceiling/ceiling_roughness.jpg");
	ceilingTexture[4] = render->LoadTexture("Ceiling/ceiling_ao.jpg");
	ceilingTexture[5] = render->LoadTexture("Ceiling/ceiling_height.png");
	wallTexture[0] = render->LoadTexture("Wall/tiles_0044_color_1k.jpg");
	wallTexture[1] = render->LoadTexture("Wall/tiles_0044_normal_opengl_1k.png");
	wallTexture[2] = DefualtTexture[2];
	wallTexture[3] = render->LoadTexture("Wall/tiles_0044_roughness_1k.jpg");
	wallTexture[4] = render->LoadTexture("Wall/tiles_0044_ao_1k.jpg");
	wallTexture[5] = render->LoadTexture("Wall/tiles_0044_height_1k.png");
	greenWallTexture[0] = render->LoadTexture("Wall/Texture_13_Diffuse.png");
	greenWallTexture[1] = render->LoadTexture("Wall/Texture_13_Normal.png");
	greenWallTexture[2] = DefualtTexture[2]; 
	greenWallTexture[3] = render->LoadTexture("Wall/Texture_131_roughness.png");
	greenWallTexture[4] = render->LoadTexture("Wall/Texture_13_ao.png");
	greenWallTexture[5] = DefualtTexture[5];
	signTexture[0] = render->LoadTexture("Wall/PicturesOnTheWall/start.png");
	signTexture[1] = render->LoadTexture("Wall/PicturesOnTheWall/continue.png");

	bossMat = new MeshMaterial("Male_Guard.mat");
	playerMat = new MeshMaterial("FemaleA.mat");
	ghostMat = new MeshMaterial("Ghost.mat");
	bookshelfMat = new MeshMaterial("bookshelf.mat");
	tableMat = new MeshMaterial("table.mat");
	columnMat = new MeshMaterial("Column.mat");
	stairMat = new MeshMaterial("Stair.mat");
	handrailMat = new MeshMaterial("handrail.mat");
	coinMat = new MeshMaterial("coin.mat");
	wallLightMat = new MeshMaterial("LightWall.mat");
	hangLightMat = new MeshMaterial("LightHanging.mat");
	layerMat = new MeshMaterial("Layer.mat");

	basicShader = render->LoadShader("scene.vert", "scene.frag");
	floorShader = render->LoadShader("scene.vert", "scene_uv.frag");
	modelShader = render->LoadShader("model.vert", "model.frag");
	bossShader = render->LoadShader("SkinningVertex.vert", "TexturedFragment.frag");
	playerShader = render->LoadShader("SkinningVertex.vert", "player.frag");
	ghostShader = render->LoadShader("SkinningVertex.vert", "ghost.frag");
	lampShader = render->LoadShader("model.vert", "lamp.frag");
	itemShader = render->LoadShader("scene.vert", "item.frag");

	bossAnimation = new MeshAnimation("Taunt.anm");
	bossCheersAnimation = new MeshAnimation("Angry.anm");
	bossShootingAnimation = new MeshAnimation("Gunfire1.anm");
	bossFlinchAnimation = new MeshAnimation("Flinches.anm");
	bossChasingAnimation = new MeshAnimation("StepForwardTwoHand.anm");
	bossAttackingAnimation = new MeshAnimation("Stow.anm");
	bossAngryAnimation = new MeshAnimation("Cheer1.anm");
	bossDeathAnimation = new MeshAnimation("Death1.anm");
	playerIdleAnimation = new MeshAnimation("Female_Stand.anm");
	playerWalkAnimation = new MeshAnimation("Female_Run.anm");
	playerJumpAnimation = new MeshAnimation("Female_Jump.anm");
	ghostAnimation = new MeshAnimation("Ghost.anm");
}

BasicExamples::~BasicExamples() {
	delete cubeMesh;
	delete sphereMesh;
	delete charMesh;
	delete goatMesh;
	delete playerMesh;
	delete bossMesh;
	delete ghostMesh;
	delete bookshelfMesh;
	delete tableMesh;
	delete columnMesh;
	delete stairMesh;
	delete handrailMesh;
	delete coinMesh;
	delete wallLightMesh;
	delete hangLightMesh;
	delete layerMesh;

	delete playerMat;
	delete bossMat;
	delete ghostMat;
	delete bookshelfMat;
	delete tableMat;
	delete columnMat;
	delete stairMat;
	delete handrailMat;
	delete coinMat;
	delete wallLightMat;
	delete hangLightMat;
	delete layerMat;

	delete basicTexture;
	for (int i = 0; i < 5; i++)
		delete DefualtTexture[i];
	for (int i = 0; i < 6; i++)
		delete floorTexture[i];
	for (int i = 0; i < 6; i++)
		delete ceilingTexture[i];
	for (int i = 0; i < 6; i++)
		delete wallTexture[i];	
	for (int i = 0; i < 6; i++)
		delete greenWallTexture[i];

	delete basicShader;
	delete floorShader;
	delete modelShader;
	delete ghostShader;
	delete bossShader;
	delete playerShader;
	delete lampShader;
}

GameObject* BasicExamples::CreateCube(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* cube = new GameObject("cube");

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2);
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTexture, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->GetRenderObject()->SetColour(Vector4(0.5, 0.5, 0.5, 0.01));
	return cube;
}

GameObject* BasicExamples::CreateBigWall(const Vector3& position, const Vector3& dimensions, float inverseMass, float scale) {
	GameObject* cube = new GameObject("cube");

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, wallTexture[0], floorShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetRenderObject()->SetDefaultTexture(wallTexture[1], 1);
	cube->GetRenderObject()->SetDefaultTexture(wallTexture[2], 2);
	cube->GetRenderObject()->SetDefaultTexture(wallTexture[3], 3);
	cube->GetRenderObject()->SetDefaultTexture(wallTexture[4], 4);
	cube->GetRenderObject()->SetDefaultTexture(wallTexture[5], 5);
	cube->GetRenderObject()->isWall = true;
	cube->GetRenderObject()->scale = scale;

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->SetTag("wall");
	return cube;
}

GameObject* BasicExamples::CreateGreenWall(const Vector3& position, const Vector3& dimensions, float inverseMass, float scale) {
	GameObject* cube = new GameObject("cube");

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, greenWallTexture[0], floorShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetRenderObject()->SetDefaultTexture(greenWallTexture[1], 1);
	cube->GetRenderObject()->SetDefaultTexture(greenWallTexture[2], 2);
	cube->GetRenderObject()->SetDefaultTexture(greenWallTexture[3], 3);
	cube->GetRenderObject()->SetDefaultTexture(greenWallTexture[4], 4);
	cube->GetRenderObject()->SetDefaultTexture(greenWallTexture[5], 5);
	cube->GetRenderObject()->isWall = true;
	cube->GetRenderObject()->scale = scale;


	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->SetTag("wall");
	return cube;
}

GameObject* BasicExamples::CreateAABB(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* cube = new GameObject("aabb");

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2);
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTexture, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->GetRenderObject()->SetColour(Vector4(0.5, 0.5, 0.5, 0.0));
	cube->Deactivate();
	return cube;
}

GameObject* BasicExamples::CreateFloor(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* cube = new GameObject("floor");

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2);
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, floorTexture[0], floorShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetRenderObject()->SetDefaultTexture(floorTexture[1], 1);
	cube->GetRenderObject()->SetDefaultTexture(floorTexture[2], 2);
	cube->GetRenderObject()->SetDefaultTexture(floorTexture[3], 3);
	cube->GetRenderObject()->SetDefaultTexture(floorTexture[4], 4);
	cube->GetRenderObject()->SetDefaultTexture(floorTexture[5], 5);
	cube->GetRenderObject()->scale = 3.0;
	cube->GetPhysicsObject()->Setelasticity(0);
	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->GetPhysicsObject()->Setelasticity(0);
	cube->SetTag("Ground");
	return cube;
}

GameObject* BasicExamples::CreateLayer(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* layer = new GameObject("layer");

	layer->GetTransform().SetPosition(position).SetScale(dimensions * 1.67);
	layer->SetRenderObject(new RenderObject(&layer->GetTransform(), layerMesh, nullptr, modelShader));
	layer->SetPhysicsObject(new PhysicsObject(&layer->GetTransform(), layer->GetBoundingVolume()));

	layer->GetRenderObject()->isAnimation = true;
	LoadMaterialTextures(layer, layerMesh, layerMat, render);

	layer->GetPhysicsObject()->SetInverseMass(inverseMass);
	layer->GetPhysicsObject()->InitCubeInertia();
	//cube->SetTag("Ground");
	return layer;
}

GameObject* BasicExamples::CreateCeiling(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* cube = new GameObject("ceiling");

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2).SetOrientation(Matrix4::Rotation(45, Vector3(0,1,0)));
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, ceilingTexture[0], floorShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetRenderObject()->SetDefaultTexture(ceilingTexture[1], 1);
	cube->GetRenderObject()->SetDefaultTexture(ceilingTexture[2], 2);
	cube->GetRenderObject()->SetDefaultTexture(ceilingTexture[3], 3);
	cube->GetRenderObject()->SetDefaultTexture(ceilingTexture[4], 4);
	cube->GetRenderObject()->SetDefaultTexture(ceilingTexture[5], 5);
	cube->GetRenderObject()->scale = 3.0;
	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->SetTag("Ground");
	return cube;
}

GameObject* BasicExamples::CreateCubeOBB(const Vector3& position, const Vector3& dimensions, float inverseMass, const Vector3& tilt, int angle) {
	GameObject* cube = new GameObject("obb");

	OBBVolume* volume = new OBBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2).SetOrientation(Matrix4::Rotation(angle, tilt));
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTexture, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	cube->GetRenderObject()->SetColour(Vector4(0, 1, 0.5, 0.5));
	cube->Deactivate();
	return cube;
}

GameObject* BasicExamples::CreateSphere(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject("sphere");

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform().SetScale(sphereSize).SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTexture, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	return sphere;
}

GameObject* BasicExamples::CreateChar(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* character = new GameObject("character");

	AABBVolume* volume = new AABBVolume(dimensions);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform().SetScale(dimensions * 2).SetPosition(position);
	character->SetRenderObject(new RenderObject(&character->GetTransform(), charMesh, basicTexture, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitCubeInertia();

	return character;
}

GameObject* BasicExamples::CreateGoat(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* goat = new GameObject("goat");

	AABBVolume* volume = new AABBVolume(dimensions);
	goat->SetBoundingVolume((CollisionVolume*)volume);

	goat->GetTransform().SetScale(dimensions * 2).SetPosition(position);
	goat->SetRenderObject(new RenderObject(&goat->GetTransform(), goatMesh, basicTexture, basicShader));
	goat->SetPhysicsObject(new PhysicsObject(&goat->GetTransform(), goat->GetBoundingVolume()));

	goat->GetPhysicsObject()->SetInverseMass(inverseMass);
	goat->GetPhysicsObject()->InitCubeInertia();

	return goat;
}

GameObject* BasicExamples::CreateColumn(const Vector3& position, float inverseMass) {
	GameObject* column = new GameObject("column");

	column->GetTransform().SetScale(Vector3(0.5, 0.358, 0.5) * 2).SetPosition(position);
	column->SetRenderObject(new RenderObject(&column->GetTransform(), columnMesh, nullptr, modelShader));
	column->SetPhysicsObject(new PhysicsObject(&column->GetTransform(), column->GetBoundingVolume()));

	column->GetRenderObject()->isAnimation = true;
	LoadMaterialTextures(column, columnMesh, columnMat, render);
	column->GetPhysicsObject()->SetInverseMass(inverseMass);
	column->GetPhysicsObject()->InitCubeInertia();

	return column;
}

GameObject* BasicExamples::CreateStairs(const Vector3& position, const Vector3& dimensions, float inverseMass, const Vector3& tilt, int angle) {
	GameObject* stair = new GameObject("stair");

	stair->GetTransform().SetPosition(position).SetScale(dimensions * 2).SetOrientation(Matrix4::Rotation(angle, tilt));
	stair->SetRenderObject(new RenderObject(&stair->GetTransform(), stairMesh, nullptr, modelShader));
	stair->SetPhysicsObject(new PhysicsObject(&stair->GetTransform(), stair->GetBoundingVolume()));
	stair->GetRenderObject()->isAnimation = true;
	LoadMaterialTextures(stair, stairMesh, stairMat, render);
	stair->GetPhysicsObject()->SetInverseMass(inverseMass);
	stair->GetPhysicsObject()->InitCubeInertia();

	return stair;
}

GameObject* BasicExamples::CreateBookshelf(const Vector3& position, float inverseMass, const Vector3& tilt, int angle) {
	GameObject* bookshelf = new GameObject("bookshelf");

	Vector3 collisionDimensions = Vector3(4, 5, 0.8);
	OBBVolume* volume = new OBBVolume(collisionDimensions);
	bookshelf->GetTransform().SetCollisionDimensions(collisionDimensions).SetCollisionOffset(Vector3(0, 5, 0.8));
	bookshelf->SetBoundingVolume((CollisionVolume*)volume);

	bookshelf->GetTransform().SetPosition(position).SetScale(Vector3(1.6, 1.6, 1.6));
	bookshelf->GetTransform().SetOrientation(Matrix4::Rotation(angle, tilt)).SetRotationCenter(Vector3(0, 0, 0.8));

	bookshelf->SetRenderObject(new RenderObject(&bookshelf->GetTransform(), bookshelfMesh, nullptr, modelShader));
	bookshelf->SetPhysicsObject(new PhysicsObject(&bookshelf->GetTransform(), bookshelf->GetBoundingVolume()));
	bookshelf->GetRenderObject()->isAnimation = true;
	LoadMaterialTextures(bookshelf, bookshelfMesh, bookshelfMat, render);
	bookshelf->GetPhysicsObject()->SetInverseMass(inverseMass);
	bookshelf->GetPhysicsObject()->InitCubeInertia();

	return bookshelf;
}

GameObject* BasicExamples::CreateTable(const Vector3& position, float inverseMass, const Vector3& tilt, int angle) {
	GameObject* table = new GameObject("table");

	table->GetTransform().SetPosition(position).SetScale(Vector3(1.6, 1.6, 1.6)).SetOrientation(Matrix4::Rotation(angle, tilt));
	table->SetRenderObject(new RenderObject(&table->GetTransform(), tableMesh, nullptr, modelShader));
	table->SetPhysicsObject(new PhysicsObject(&table->GetTransform(), table->GetBoundingVolume()));
	table->GetRenderObject()->isAnimation = true;
	LoadMaterialTextures(table, tableMesh, tableMat, render);
	table->GetPhysicsObject()->SetInverseMass(inverseMass);
	table->GetPhysicsObject()->InitCubeInertia();

	return table;
}

GameObject* BasicExamples::CreateGhost(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* ghost = new GameObject("ghost");
	AABBVolume* volume = new AABBVolume(dimensions);
	ghost->SetBoundingVolume((CollisionVolume*)volume);
	
	ghost->GetTransform().SetScale(dimensions * 2).SetPosition(position);
	ghost->SetRenderObject(new RenderObject(&ghost->GetTransform(), ghostMesh, nullptr, ghostShader));
	ghost->SetPhysicsObject(new PhysicsObject(&ghost->GetTransform(), ghost->GetBoundingVolume()));
	ghost->GetRenderObject()->isAnimation = true;
	ghost->GetRenderObject()->isAnimated = true;
	LoadMaterialTextures(ghost, ghostMesh, ghostMat, render);
	ghost->GetPhysicsObject()->SetInverseMass(inverseMass);
	ghost->GetPhysicsObject()->InitCubeInertia();
	ghost->GetRenderObject()->SetColour(Vector4(1, 1, 1, 0.5));
	ghost->SetTag("ghost");
	return ghost;
}

Boss* BasicExamples::CreateBoss(const Vector3& position, const Vector3& dimensions, Player* player, float inverseMass) {
	Boss* character = new Boss(player);
	//character->BossBehaviourTree(player);
	Vector3 collisionDimensions = Vector3(1, 2.0, 1) * dimensions;
	AABBVolume* volume = new AABBVolume(collisionDimensions);
	character->GetTransform().SetCollisionDimensions(collisionDimensions);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform().SetScale(dimensions * 2).SetPosition(position).SetOrientation(Matrix4::Rotation(180, Vector3(0, 1, 0)));
	character->GetTransform().SetCollisionOffset(Vector3(0, dimensions.y*2, 0));

	character->SetRenderObject(new RenderObject(&character->GetTransform(), bossMesh, nullptr, bossShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
	character->GetRenderObject()->isAnimation = true;
	character->GetRenderObject()->isAnimated = true;

	LoadMaterialTextures(character, bossMesh, bossMat, render);
	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitCubeInertia();
	character->SetTag("boss");
	return character;
}

GameObject* BasicExamples::CreateCapsule(const Vector3& position, float halfHeight, float radius, float inverseMass) {
	GameObject* capsule = new GameObject("capsule");

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	capsule->SetBoundingVolume((CollisionVolume*)volume);

	capsule->GetTransform().SetPosition(position).SetScale(Vector3(radius, halfHeight, radius));
	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, basicTexture, basicShader));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	capsule->GetPhysicsObject()->SetInverseMass(inverseMass);
	capsule->GetPhysicsObject()->InitSphereInertia();
	capsule->Deactivate();
	return capsule;
}

GameObject* BasicExamples::CreateLight(const Vector3& position, const Vector4& color, float radius, bool islight, bool isshadow) {
	GameObject* light = new GameObject("sphere");

	light->GetTransform().SetScale(Vector3(radius, radius, radius)).SetPosition(position);
	light->SetRenderObject(new RenderObject(&light->GetTransform(), sphereMesh, nullptr, basicShader));
	light->GetRenderObject()->isLight = true;
	light->GetRenderObject()->onLight = islight;
	light->GetRenderObject()->isShadow = isshadow;
	light->GetRenderObject()->SetColour(color);

	light->SetPhysicsObject(new PhysicsObject(&light->GetTransform(), light->GetBoundingVolume()));
	light->GetPhysicsObject()->SetInverseMass(0.0f);
	light->GetPhysicsObject()->InitCubeInertia();

	return light;
}

GameObject* BasicExamples::CreateLight(const Transform* transform, const Vector4& color, float radius, bool islight, bool isshadow) {
	GameObject* light = new GameObject("sphere");

	light->GetTransform().SetScale(Vector3(radius, radius, radius)).SetPosition(transform->GetPosition());
	light->SetRenderObject(new RenderObject(&light->GetTransform(), sphereMesh, nullptr, basicShader));
	light->GetRenderObject()->isLight = true;
	light->GetRenderObject()->onLight = islight;
	light->GetRenderObject()->isShadow = isshadow;
	light->GetRenderObject()->SetColour(color);

	light->SetPhysicsObject(new PhysicsObject(&light->GetTransform(), light->GetBoundingVolume()));
	light->GetPhysicsObject()->SetInverseMass(0.0f);
	light->GetPhysicsObject()->InitCubeInertia();

	return light;
}

GameObject* BasicExamples::CreateSignStart(const Vector3& position, const Vector3& dimensions, float rotation, float scale, bool enable) {
	GameObject* cube = new GameObject("cube");

	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2).SetOrientation(Matrix4::Rotation(rotation, Vector3(0, 1, 0)));
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, signTexture[0], floorShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetRenderObject()->SetDefaultTexture(DefualtTexture[1], 1);
	cube->GetRenderObject()->SetDefaultTexture(DefualtTexture[2], 2);
	cube->GetRenderObject()->SetDefaultTexture(DefualtTexture[3], 3);
	cube->GetRenderObject()->SetDefaultTexture(DefualtTexture[4], 4);
	cube->GetRenderObject()->SetDefaultTexture(DefualtTexture[5], 5);
	cube->GetRenderObject()->hasShadow = false;
	cube->GetRenderObject()->scale = scale;
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetPhysicsObject()->SetInverseMass(0.0f);
	cube->GetPhysicsObject()->InitCubeInertia();
	
	return cube;
}

GameObject* BasicExamples::CreateSignContinue(const Vector3& position, const Vector3& dimensions, float rotation, float scale, bool enable) {
	GameObject* cube = new GameObject("cube");

	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2).SetOrientation(Matrix4::Rotation(rotation, Vector3(0, 1, 0)));
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, signTexture[1], floorShader));
	cube->GetRenderObject()->SetDefaultTexture(DefualtTexture[1], 1);
	cube->GetRenderObject()->SetDefaultTexture(DefualtTexture[2], 2);
	cube->GetRenderObject()->SetDefaultTexture(DefualtTexture[3], 3);
	cube->GetRenderObject()->SetDefaultTexture(DefualtTexture[4], 4);
	cube->GetRenderObject()->SetDefaultTexture(DefualtTexture[5], 5);
	cube->GetRenderObject()->hasShadow = false;
	cube->GetRenderObject()->scale = scale;
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetPhysicsObject()->SetInverseMass(0.0f);
	cube->GetPhysicsObject()->InitCubeInertia();
	
	return cube;
}

Player* BasicExamples::CreatePlayer(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	Player* player = new Player("player");

	Vector3 collisionDimensions = Vector3(0.6, 1.0, 0.6) * dimensions;
	AABBVolume* volume = new AABBVolume(collisionDimensions);
	player->GetTransform().SetCollisionDimensions(collisionDimensions);
	player->SetBoundingVolume((CollisionVolume*)volume);

	player->GetTransform().SetScale(dimensions * 2).SetPosition(position).SetCollisionOffset(Vector3(0, dimensions.y, 0));
	player->SetRenderObject(new RenderObject(&player->GetTransform(), playerMesh, nullptr, playerShader));
	player->SetPhysicsObject(new PhysicsObject(&player->GetTransform(), player->GetBoundingVolume()));
	player->GetRenderObject()->isAnimation = true;
	player->GetRenderObject()->isAnimated = true;

	LoadMaterialTextures(player, playerMesh, playerMat, render);

	player->GetPhysicsObject()->SetInverseMass(inverseMass);
	player->GetPhysicsObject()->InitPlayerInertia();
	player->GetPhysicsObject()->Setelasticity(0);
	player->GetPhysicsObject()->SetApplyAngImp(false);
	player->SetTag("player");
	return player;
}

void BasicExamples::LoadMaterialTextures(GameObject* character, Mesh* mesh, MeshMaterial* material, GameTechRenderer* renderer) {
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		const MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);

		// load diffuse
		const std::string* filenameDiffuse = nullptr;
		matEntry->GetEntry("Diffuse", &filenameDiffuse);
		std::string pathDiffuse = *filenameDiffuse;
		//std::cout << "Diffuse Texture: " << pathDiffuse << std::endl;
		if (!pathDiffuse.empty()) {
			character->GetRenderObject()->matDiffuseTextures.emplace_back(renderer->LoadTexture(pathDiffuse));
		}

		// load normal
		const std::string* filenameNormal = nullptr;
		if (matEntry->GetEntry("Bump", &filenameNormal)) {
			std::string pathNormal = *filenameNormal;
			//std::cout << "Normal Texture: " << pathNormal << std::endl;
			if (!pathNormal.empty()) {
				character->GetRenderObject()->matNormalTextures.emplace_back(renderer->LoadTexture(pathNormal));
			}
		}

		// load metal
		const std::string* filenameMetal = nullptr;
		if (matEntry->GetEntry("Metallic", &filenameMetal)) {
			std::string pathMetal = *filenameMetal;
			//std::cout << "Metal Texture: " << pathMetal << std::endl;
			if (!pathMetal.empty()) {
				character->GetRenderObject()->matMetalTextures.emplace_back(renderer->LoadTexture(pathMetal));
			}
		}

		// Load roughness
		const std::string* filenameRoughness = nullptr;
		if (matEntry->GetEntry("Roughness", &filenameRoughness)) {
			std::string pathRoughness = *filenameRoughness;
			//std::cout << "Roughness Texture: " << pathRoughness << std::endl;
			if (!pathRoughness.empty()) {
				character->GetRenderObject()->matRoughnessTextures.emplace_back(renderer->LoadTexture(pathRoughness));
			}
		}

		// Load ambient occlusion (Ao)
		const std::string* filenameAo = nullptr;
		if (matEntry->GetEntry("AO", &filenameAo)) {
			std::string pathAo = *filenameAo;
			//std::cout << "Ao Texture: " << pathAo << std::endl;
			if (!pathAo.empty()) {
				character->GetRenderObject()->matAoTextures.emplace_back(renderer->LoadTexture(pathAo));
			}
		}

		// Load height
		const std::string* filenameHeight = nullptr;
		if (matEntry->GetEntry("Height", &filenameHeight)) {
			std::string pathHeight = *filenameHeight;
			//std::cout << "Height Texture: " << pathHeight << std::endl;
			if (!pathHeight.empty()) {
				character->GetRenderObject()->matHeightTextures.emplace_back(renderer->LoadTexture(pathHeight));
			}
		}
	}
}

GhostAI* BasicExamples::CreateGhostAI(const Vector3& position, const Vector3& dimensions, GameObject* player, float inverseMass, const Vector3& star, const Vector3& end) {
	GhostAI* ghost = new GhostAI(star,end,player);
	AABBVolume* volume = new AABBVolume(dimensions);
	ghost->SetBoundingVolume((CollisionVolume*)volume);
	ghost->GetTransform().SetScale(dimensions * 2).SetPosition(position);
	ghost->SetRenderObject(new RenderObject(&ghost->GetTransform(), ghostMesh, nullptr, ghostShader));
	ghost->SetPhysicsObject(new PhysicsObject(&ghost->GetTransform(), ghost->GetBoundingVolume()));
	ghost->GetRenderObject()->isAnimation = true;
	ghost->GetRenderObject()->isAnimated = true;
	LoadMaterialTextures(ghost, ghostMesh, ghostMat, render);
	ghost->GetPhysicsObject()->SetInverseMass(inverseMass);
	ghost->GetPhysicsObject()->InitCubeInertia();
	ghost->GetRenderObject()->SetColour(Vector4(1, 1, 1, 0.5));
	ghost->SetTag("ghost");
	return ghost;
}

GameObject* BasicExamples::CreateCoin(const Vector3& position, const Vector3& dimensions, float inverseMass, float rotation) {
	GameObject* coin = new Coin("coin");
	AABBVolume* volume = new AABBVolume(dimensions * Vector3(5, 5, 1));
	coin->GetTransform().SetCollisionDimensions(dimensions * Vector3(5, 5, 1));
	coin->SetBoundingVolume((CollisionVolume*)volume);
	coin->GetTransform().SetPosition(position).SetScale(dimensions).SetOrientation(Quaternion(Matrix4::Rotation(rotation, Vector3(0, 1, 0))));
	coin->SetRenderObject(new RenderObject(&coin->GetTransform(), coinMesh, nullptr, basicShader));
	coin->SetPhysicsObject(new PhysicsObject(&coin->GetTransform(), coin->GetBoundingVolume()));

	coin->GetPhysicsObject()->SetInverseMass(inverseMass);
	coin->GetPhysicsObject()->InitCubeInertia();
	coin->GetRenderObject()->SetColour(Debug::YELLOW);
	coin->SetTag("coin");
	return coin;
}

Door* BasicExamples::CreateDoor(const Vector3& position, const Vector3& dimensions, float inverseMass, float rotation, float resDis) {
	Door* d = new Door(player, position, rotation, resDis);
	OBBVolume* volume = new OBBVolume(dimensions);

	d->SetBoundingVolume((CollisionVolume*)volume);
	d->GetTransform().SetPosition(position).SetScale(dimensions * 2).SetOrientation(Quaternion(Matrix4::Rotation(rotation, Vector3(0, 1, 0))));
	d->SetRenderObject(new RenderObject(&d->GetTransform(), cubeMesh, nullptr, basicShader));
	d->SetPhysicsObject(new PhysicsObject(&d->GetTransform(), d->GetBoundingVolume()));

	d->GetPhysicsObject()->SetInverseMass(inverseMass);
	d->GetPhysicsObject()->InitCubeInertia();
	
	return d;
}

GameObject* BasicExamples::CreateFireBallBullet(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject("fireballbullet");
	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius + 2);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform().SetScale(sphereSize).SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, FireBallTexture, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();
	sphere->SetIsHiding(true, position);
	sphere->SetExistenceTime(0.0f);
	sphere->SetTag("fireballbullet");
	return sphere;
}

GameObject* BasicExamples::CreateIceCubeBullet(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* cube = new GameObject("icecubebullet");

	OBBVolume* volume = new OBBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2);
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, IceCubeTexture, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->SetIsHiding(true, position);
	cube->SetExistenceTime(0.0f);
	cube->SetTag("icecubebullet");

	return cube;
}

GameObject* BasicExamples::CreateRollingRock(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject("rollingrock");
	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius + 1);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform().SetScale(sphereSize).SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), rockMesh, nullptr, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));
	sphere->GetRenderObject()->SetColour(Vector4(0.3f, 0.3f, 0.3f, 1.0f));
	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();
	sphere->SetTag("rollingrock");
	propList.push_back(sphere);
	return sphere;
}

GameObject* BasicExamples::CreateShield(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* shield = new GameObject("shield");

	//AABBVolume* volume = new AABBVolume(Vector3(1.2, 1.2, 0.2) * dimensions);
	//shield->GetTransform().SetCollisionDimensions(Vector3(1.2, 1.2, 0.2) * dimensions);
	//shield->SetBoundingVolume((CollisionVolume*)volume);

	shield->GetTransform().SetScale(dimensions * 2).SetPosition(position);
	shield->SetRenderObject(new RenderObject(&shield->GetTransform(), shieldMesh, shieldTexture, modelShader));
	shield->SetPhysicsObject(new PhysicsObject(&shield->GetTransform(), shield->GetBoundingVolume()));
	//shield->GetRenderObject()->isAnimation = true;
	//LoadMaterialTextures(shield, shieldMesh, shieldMat, render);

	shield->GetPhysicsObject()->SetInverseMass(inverseMass);
	shield->GetPhysicsObject()->InitCubeInertia();
	shield->SetIsHiding(true, Vector3(0, -100, 0));
	shield->SetTag("shield");
	return shield;
}

GameObject* BasicExamples::CreateWallLight(const Vector3& position, float inverseMass, const Vector3& tilt, int angle){
	GameObject* lightmodel = new GameObject("lightmodel");
	AABBVolume* volume = new AABBVolume(Vector3(4, 4, 4));
	lightmodel->SetBoundingVolume((CollisionVolume*)volume);
	lightmodel->GetTransform().SetPosition(position).SetScale(Vector3(4, 4, 4) * 2).SetOrientation(Matrix4::Rotation(angle, tilt));
	lightmodel->SetRenderObject(new RenderObject(&lightmodel->GetTransform(), wallLightMesh, nullptr, lampShader));
	lightmodel->SetPhysicsObject(new PhysicsObject(&lightmodel->GetTransform(), lightmodel->GetBoundingVolume()));

	lightmodel->GetRenderObject()->isAnimation = true;
	LoadMaterialTextures(lightmodel, wallLightMesh, wallLightMat, render);
	lightmodel->GetPhysicsObject()->SetInverseMass(inverseMass);
	lightmodel->GetPhysicsObject()->InitCubeInertia();

	return lightmodel;
}

GameObject* BasicExamples::CreateHangLight(const Vector3& position, const Vector3& dimensions, float inverseMass){
	GameObject* lightmodel = new GameObject("lightmodel");
	AABBVolume* volume = new AABBVolume(dimensions);
	lightmodel->SetBoundingVolume((CollisionVolume*)volume);
	lightmodel->GetTransform().SetPosition(position).SetScale(dimensions * 2);// .SetOrientation(Quaternion(Matrix4::Rotation(rotation, Vector3(0, 1, 0))));
	lightmodel->SetRenderObject(new RenderObject(&lightmodel->GetTransform(), hangLightMesh, nullptr, lampShader));
	lightmodel->SetPhysicsObject(new PhysicsObject(&lightmodel->GetTransform(), lightmodel->GetBoundingVolume()));

	lightmodel->GetRenderObject()->isAnimation = true;
	lightmodel->GetRenderObject()->hasShadow = false;
	LoadMaterialTextures(lightmodel, hangLightMesh, hangLightMat, render);
	lightmodel->GetPhysicsObject()->SetInverseMass(inverseMass);
	lightmodel->GetPhysicsObject()->InitCubeInertia();

	return lightmodel;
}

GameObject* BasicExamples::CreateHandrail(const Vector3& position, float inverseMass, const Vector3& tilt, int angle) {
	GameObject* handrail = new GameObject("handrail");
	OBBVolume* volume = new OBBVolume(Vector3(7, 4, 1));
	handrail->GetTransform().SetCollisionDimensions(Vector3(6, 4, 1));
	handrail->SetBoundingVolume((CollisionVolume*)volume);

	handrail->GetTransform().SetPosition(position).SetScale(Vector3(2, 2, 2) * 2).SetOrientation(Matrix4::Rotation(angle, tilt));
	handrail->SetRenderObject(new RenderObject(&handrail->GetTransform(), handrailMesh, nullptr, modelShader));
	handrail->SetPhysicsObject(new PhysicsObject(&handrail->GetTransform(), handrail->GetBoundingVolume()));

	handrail->GetRenderObject()->isAnimation = true;
	LoadMaterialTextures(handrail, handrailMesh, handrailMat, render);
	handrail->GetPhysicsObject()->SetInverseMass(inverseMass);
	handrail->GetPhysicsObject()->InitCubeInertia();

	return handrail;
}

GameObject* BasicExamples::CreateRedBottleProp(const Vector3& position, const Vector3& dimensions, float inverseMass ) {
	GameObject* redbottle = new GameObject("redbottle");

	AABBVolume* volume = new AABBVolume(dimensions);
	redbottle->SetBoundingVolume((CollisionVolume*)volume);
	redbottle->GetTransform().SetPosition(position).SetScale(dimensions * 2).SetOrientation(Matrix4::Rotation(260, Vector3(1, 0, 0)));
	redbottle->SetRenderObject(new RenderObject(&redbottle->GetTransform(), redBottleMesh, redBottleTexture, itemShader));
	redbottle->SetPhysicsObject(new PhysicsObject(&redbottle->GetTransform(), redbottle->GetBoundingVolume()));

	redbottle->GetPhysicsObject()->SetInverseMass(inverseMass);
	redbottle->GetPhysicsObject()->InitCubeInertia();
	redbottle->SetTag("item");
	propList.push_back(redbottle);
	return redbottle;
}

GameObject* BasicExamples::CreateSpeedProp(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* coffeecup = new GameObject("speedprop");

	AABBVolume* volume = new AABBVolume(Vector3(0.15, 0.15, 0.15) * dimensions);
	coffeecup->GetTransform().SetCollisionDimensions(Vector3(0.15, 0.15, 0.15) * dimensions);
	coffeecup->SetBoundingVolume((CollisionVolume*)volume);

	coffeecup->GetTransform().SetScale(dimensions * 2).SetPosition(position);
	coffeecup->SetRenderObject(new RenderObject(&coffeecup->GetTransform(), speedPropMesh, speedPropTexture, itemShader));
	coffeecup->SetPhysicsObject(new PhysicsObject(&coffeecup->GetTransform(), coffeecup->GetBoundingVolume()));
	//shield->GetRenderObject()->isAnimation = true;
	//LoadMaterialTextures(shield, shieldMesh, shieldMat, render);

	coffeecup->GetPhysicsObject()->SetInverseMass(inverseMass);
	coffeecup->GetPhysicsObject()->InitCubeInertia();
	coffeecup->SetTag("item");
	propList.push_back(coffeecup);
	return coffeecup;
}

GameObject* BasicExamples::CreateShieldProp(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* shield = new GameObject("shieldprop");

	AABBVolume* volume = new AABBVolume(Vector3(1.2, 1.2, 0.2) * dimensions);
	shield->GetTransform().SetCollisionDimensions(Vector3(1.2, 1.2, 0.2) * dimensions);
	shield->SetBoundingVolume((CollisionVolume*)volume);

	shield->GetTransform().SetScale(dimensions * 2).SetPosition(position);
	shield->SetRenderObject(new RenderObject(&shield->GetTransform(), shieldMesh, shieldTexture, itemShader));
	shield->SetPhysicsObject(new PhysicsObject(&shield->GetTransform(), shield->GetBoundingVolume()));
	//shield->GetRenderObject()->isAnimation = true;
	//LoadMaterialTextures(shield, shieldMesh, shieldMat, render);

	shield->GetPhysicsObject()->SetInverseMass(inverseMass);
	shield->GetPhysicsObject()->InitCubeInertia();
	shield->SetTag("item");
	propList.push_back(shield);
	return shield;
}

GameObject* BasicExamples::CreateRollingRockProp(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject("rollingrockprop");
	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius + 1);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform().SetScale(sphereSize).SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), rockMesh, nullptr, itemShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));
	sphere->GetRenderObject()->SetColour(Vector4(0.3f, 0.3f, 0.3f, 1.0f));
	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();
	sphere->SetTag("item");
	propList.push_back(sphere);
	return sphere;
}
