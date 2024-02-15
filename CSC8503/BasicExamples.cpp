#include "BasicExamples.h"
#include "RenderObject.h"
#include "PhysicsObject.h"
#include "MeshMaterial.h"

using namespace NCL;
using namespace CSC8503;

BasicExamples::BasicExamples(GameTechRenderer* render) {

	this->render = render; 

	cubeMesh    = render->LoadMesh("cube.msh");
	sphereMesh  = render->LoadMesh("sphere.msh");
	charMesh    = render->LoadMesh("Keeper.msh");
	bossMesh	= render->LoadMesh("Role_T.msh");
	playerMesh = render->LoadMesh("Male_Guard.msh");
	//ghostMesh = render->LoadMesh("Ghost_animation.msh");
	goatMesh    = render->LoadMesh("goat.msh");
	capsuleMesh = render->LoadMesh("capsule.msh");

	basicTexture = render->LoadTexture("checkerboard.png");
	MetalTexture[0] = render->LoadTexture("white.jpg");
	MetalTexture[1] = render->LoadTexture("grey.jpg");
	MetalTexture[2] = render->LoadTexture("black.jpg");
	floorTexture[0] = render->LoadTexture("Floor/floor_color.jpg");
	floorTexture[1] = render->LoadTexture("Floor/floor_normal.png");
	floorTexture[2] = MetalTexture[1];
	floorTexture[3] = render->LoadTexture("Floor/floor_roughness.jpg");
	floorTexture[4] = render->LoadTexture("Floor/floor_ao.jpg");
	floorTexture[5] = render->LoadTexture("Floor/floor_height.png");
	bossMat = new MeshMaterial("Role_T.mat");
	playerMat = new MeshMaterial("Male_Guard.mat");
	//ghostMat = new MeshMaterial("Ghost_animation");

	basicShader = render->LoadShader("scene.vert", "scene.frag");
	bossShader = render->LoadShader("SkinningVertex.vert", "TexturedFragment.frag");
	playerShader = render->LoadShader("SkinningVertex.vert", "TexturedFragment.frag");
	testShader = render->LoadShader("scene.vert", "scene_uv.frag");
	//ghostShader = render->LoadShader("ghostVertex.vert", "TexturedFragment.frag");

	bossAnimation = new MeshAnimation("Role_T.anm");
	playerIdleAnimation = new MeshAnimation("idle1.anm");
	playerWalkAnimation = new MeshAnimation("StepForwardTwoHand.anm");
	//ghostAnimation = new MeshAnimation("ghost_ani.anm");
}

BasicExamples::~BasicExamples() {
	delete cubeMesh;
	delete sphereMesh;
	delete charMesh;
	delete goatMesh;
	delete capsuleMesh;
	delete basicTexture;
	delete basicShader;
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

	return cube;
}

GameObject* BasicExamples::CreateFloor(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* cube = new GameObject("cube");

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2);
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, floorTexture[0], testShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetRenderObject()->SetDefaultTexture(floorTexture[1], 1);
	cube->GetRenderObject()->SetDefaultTexture(floorTexture[2], 2);
	cube->GetRenderObject()->SetDefaultTexture(floorTexture[3], 3);
	cube->GetRenderObject()->SetDefaultTexture(floorTexture[4], 4);
	cube->GetRenderObject()->SetDefaultTexture(floorTexture[5], 5);

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->SetTag("Ground");
	return cube;
}

GameObject* BasicExamples::CreateCubeOBB(const Vector3& position, const Vector3& dimensions, float inverseMass, const Vector3& tilt, int angle) {
	GameObject* cube = new GameObject("cube");

	OBBVolume* volume = new OBBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2).SetOrientation(Matrix4::Rotation(angle, tilt));
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTexture, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	cube->GetRenderObject()->SetColour(Vector4(0, 1, 0.5, 1));

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

GameObject* BasicExamples::CreateBoss(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* character = new GameObject("boss");

	AABBVolume* volume = new AABBVolume(dimensions);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform().SetScale(dimensions * 2).SetPosition(position);
	character->SetRenderObject(new RenderObject(&character->GetTransform(), bossMesh, nullptr, bossShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
	character->GetRenderObject()->isAnimation = true;
	LoadMaterialTextures(character, bossMesh, bossMat, render);
	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitCubeInertia();

	return character;
}

GameObject* BasicExamples::CreateTestMesh(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* character = new GameObject("testmesh");

	AABBVolume* volume = new AABBVolume(dimensions);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform().SetScale(dimensions * 2).SetPosition(position);
	character->SetRenderObject(new RenderObject(&character->GetTransform(), ghostMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitCubeInertia();

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

	return capsule;
}

GameObject* BasicExamples::CreatePlayer(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	Player* character = new Player("player");

	AABBVolume* volume = new AABBVolume(Vector3(0.6, 0.1, 0.6) * dimensions);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform().SetScale(dimensions * 2).SetPosition(position);
	character->SetRenderObject(new RenderObject(&character->GetTransform(), playerMesh, nullptr, playerShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
	character->GetRenderObject()->isAnimation = true;
	LoadMaterialTextures(character, playerMesh, playerMat, render);

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitCubeInertia();
	player = character;
	return player;
}

void BasicExamples::LoadMaterialTextures(GameObject* character, Mesh* mesh, MeshMaterial* material, GameTechRenderer* renderer) {

	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		const MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);
		const std::string* filenameDiffuse = nullptr;
		matEntry->GetEntry("Diffuse", &filenameDiffuse);
		std::string pathDiffuse = *filenameDiffuse;
		std::cout << "Diffuse Texture: " << pathDiffuse << std::endl;

		if (!pathDiffuse.empty()) {
			character->GetRenderObject()->matTextures.emplace_back(renderer->LoadTexture(pathDiffuse));
		}

	}
}
StateGameObject* BasicExamples::CreateAItest(const Vector3& position, const Vector3& dimensions, GameObject* player, float inverseMass) {
	//GameObject* cube = new GameObject("cube");
	AABBVolume* volume = new AABBVolume(dimensions); 
	StateGameObject* ghost = new StateGameObject(player);

	ghost->SetBoundingVolume((CollisionVolume*)volume);
	ghost->GetTransform().SetPosition(position).SetScale(dimensions * 2);
	ghost->SetRenderObject(new RenderObject(&ghost->GetTransform(), cubeMesh, nullptr, basicShader));
	//LoadMaterialTextures(ghost, ghostMesh, ghostMat, render);
	ghost->SetPhysicsObject(new PhysicsObject(&ghost->GetTransform(), ghost->GetBoundingVolume()));

	ghost->GetPhysicsObject()->SetInverseMass(inverseMass);
	ghost->GetPhysicsObject()->InitCubeInertia();
	ghost->SetCollisionResponse(false);

	return ghost;
}
