#include "BasicExamples.h"
#include "RenderObject.h"
#include "PhysicsObject.h"
#include "MeshMaterial.h"

#include <fstream>

using namespace NCL;
using namespace CSC8503;

BasicExamples::BasicExamples(GameTechRenderer* render) {

	this->render = render; 

	cubeMesh    = render->LoadMesh("cube.msh");
	sphereMesh  = render->LoadMesh("sphere.msh");
	charMesh    = render->LoadMesh("Keeper.msh");
	bossMesh	= render->LoadMesh("Role_T.msh");
	playerMesh  = render->LoadMesh("Male_Guard.msh");
	ghostMesh   = render->LoadMesh("Ghost.msh");
	goatMesh    = render->LoadMesh("goat.msh");
	capsuleMesh = render->LoadMesh("capsule.msh");

	basicTexture = render->LoadTexture("checkerboard.png");
	MetalTexture[0] = render->LoadTexture("Color/white.jpg");
	MetalTexture[1] = render->LoadTexture("Color/grey.jpg");
	MetalTexture[2] = render->LoadTexture("Color/black.jpg");
	floorTexture[0] = render->LoadTexture("Floor/floor_color.jpg");
	floorTexture[1] = render->LoadTexture("Floor/floor_normal.png");
	floorTexture[2] = MetalTexture[1];
	floorTexture[3] = render->LoadTexture("Floor/floor_roughness.jpg");
	floorTexture[4] = render->LoadTexture("Floor/floor_ao.jpg");
	floorTexture[5] = render->LoadTexture("Floor/floor_height.png");
	
	bossMat = new MeshMaterial("Role_T.mat");
	playerMat = new MeshMaterial("Male_Guard.mat");
	ghostMat = new MeshMaterial("Ghost.mat");

	basicShader = render->LoadShader("scene.vert", "scene.frag");
	floorShader = render->LoadShader("scene.vert", "scene_uv.frag");
	bossShader = render->LoadShader("SkinningVertex.vert", "TexturedFragment.frag");
	playerShader = render->LoadShader("SkinningVertex.vert", "TexturedFragment.frag");
	ghostShader = render->LoadShader("SkinningVertex.vert", "ghost.frag");

	bossAnimation = new MeshAnimation("Role_T.anm");
	playerIdleAnimation = new MeshAnimation("idle1.anm");
	playerWalkAnimation = new MeshAnimation("StepForwardTwoHand.anm");
	ghostAnimation = new MeshAnimation("Ghost.anm");
}

BasicExamples::~BasicExamples() {
	delete cubeMesh;
	delete sphereMesh;
	delete charMesh;
	delete goatMesh;
	delete capsuleMesh;
	delete basicTexture;
	for (int i = 0; i < 3; i++)
		delete MetalTexture[i];
	for (int i = 0; i < 6; i++)
		delete floorTexture[i];
	delete basicShader;
	delete floorShader;
	delete ghostShader;
}

// Todo: the indices is in wrong order
void BasicExamples::ExportToObj(const Mesh& mesh, const std::string& filename) {
	std::ofstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return;
	}

	for (const auto& vertex : mesh.GetPositionData()) {
		file << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
	}

	for (const auto& normal : mesh.GetNormalData()) {
		file << "vn " << normal.x << " " << normal.y << " " << normal.z << std::endl;
	}

	Vector3 nor;
	for (int i = 0; mesh.GetNormalForTri(i, nor); i++) {
		int j = i;
		file << "f " << mesh.GetIndexData()[j++] << "//" << nor.x << " "
			<< mesh.GetIndexData()[j++] << "//" << nor.y << " "
			<< mesh.GetIndexData()[j] << "//" << nor.z << std::endl;
	}

	file.close();
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
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, floorTexture[0], floorShader));
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

GameObject* BasicExamples::CreateGhost(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* ghost = new GameObject("ghost");

	AABBVolume* volume = new AABBVolume(dimensions);
	ghost->SetBoundingVolume((CollisionVolume*)volume);

	ghost->GetTransform().SetScale(dimensions * 2).SetPosition(position);
	ghost->SetRenderObject(new RenderObject(&ghost->GetTransform(), ghostMesh, nullptr, ghostShader));
	ghost->SetPhysicsObject(new PhysicsObject(&ghost->GetTransform(), ghost->GetBoundingVolume()));
	ghost->GetRenderObject()->isAnimation = true;
	LoadMaterialTextures(ghost, ghostMesh, ghostMat, render);
	ghost->GetPhysicsObject()->SetInverseMass(inverseMass);
	ghost->GetPhysicsObject()->InitCubeInertia();

	return ghost;
}

Boss* BasicExamples::CreateBoss(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	Boss* character = new Boss("boss");
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
	player = new Player("player");

	AABBVolume* volume = new AABBVolume(Vector3(0.6, 1, 0.6) * dimensions);
	player->SetVolumeSize(Vector3(0.6, 1, 0.6) * dimensions);
	player->SetBoundingVolume((CollisionVolume*)volume);

	player->GetTransform().SetScale(dimensions * 2).SetPosition(position);
	player->SetRenderObject(new RenderObject(&player->GetTransform(), playerMesh, nullptr, playerShader));
	player->SetPhysicsObject(new PhysicsObject(&player->GetTransform(), player->GetBoundingVolume()));
	player->GetRenderObject()->isAnimation = true;
	LoadMaterialTextures(player, playerMesh, playerMat, render);

	player->GetPhysicsObject()->SetInverseMass(inverseMass);
	player->GetPhysicsObject()->InitCubeInertia();
	player->GetPhysicsObject()->SetApplyAngImp(false);

	return player;
}

void BasicExamples::LoadMaterialTextures(GameObject* character, Mesh* mesh, MeshMaterial* material, GameTechRenderer* renderer) {
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		const MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);

		// load diffuse
		const std::string* filenameDiffuse = nullptr;
		matEntry->GetEntry("Diffuse", &filenameDiffuse);
		std::string pathDiffuse = *filenameDiffuse;
		std::cout << "Diffuse Texture: " << pathDiffuse << std::endl;
		if (!pathDiffuse.empty()) {
			character->GetRenderObject()->matDiffuseTextures.emplace_back(renderer->LoadTexture(pathDiffuse));
		}

		// load normal
		const std::string* filenameNormal = nullptr;
		if (matEntry->GetEntry("Bump", &filenameNormal)) {
			std::string pathNormal = *filenameNormal;
			std::cout << "Normal Texture: " << pathNormal << std::endl;
			if (!pathNormal.empty()) {
				character->GetRenderObject()->matNormalTextures.emplace_back(renderer->LoadTexture(pathNormal));
			}
		}

		// load metal
		const std::string* filenameMetal = nullptr;
		if (matEntry->GetEntry("Metal", &filenameMetal)) {
			std::string pathMetal = *filenameMetal;
			std::cout << "Metal Texture: " << pathMetal << std::endl;
			if (!pathMetal.empty()) {
				character->GetRenderObject()->matMetalTextures.emplace_back(renderer->LoadTexture(pathMetal));
			}
		}

		// Load roughness
		const std::string* filenameRoughness = nullptr;
		if (matEntry->GetEntry("Roughness", &filenameRoughness)) {
			std::string pathRoughness = *filenameRoughness;
			std::cout << "Roughness Texture: " << pathRoughness << std::endl;
			if (!pathRoughness.empty()) {
				character->GetRenderObject()->matRoughnessTextures.emplace_back(renderer->LoadTexture(pathRoughness));
			}
		}

		// Load ambient occlusion (Ao)
		const std::string* filenameAo = nullptr;
		if (matEntry->GetEntry("Ao", &filenameAo)) {
			std::string pathAo = *filenameAo;
			std::cout << "Ao Texture: " << pathAo << std::endl;
			if (!pathAo.empty()) {
				character->GetRenderObject()->matAoTextures.emplace_back(renderer->LoadTexture(pathAo));
			}
		}

		// Load height
		const std::string* filenameHeight = nullptr;
		if (matEntry->GetEntry("Height", &filenameHeight)) {
			std::string pathHeight = *filenameHeight;
			std::cout << "Height Texture: " << pathHeight << std::endl;
			if (!pathHeight.empty()) {
				character->GetRenderObject()->matHeightTextures.emplace_back(renderer->LoadTexture(pathHeight));
			}
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

Door* BasicExamples::CreateDoor(const Vector3& position, const Vector3& dimensions, float inverseMass, float rotation) {
	Door* d = new Door(player);
	OBBVolume* volume = new OBBVolume(dimensions);

	d->SetBoundingVolume((CollisionVolume*)volume);
	d->GetTransform().SetPosition(position).SetScale(dimensions * 2).SetOrientation(Quaternion(Matrix4::Rotation(rotation, Vector3(0, 1, 0))));
	d->SetRenderObject(new RenderObject(&d->GetTransform(), cubeMesh, nullptr, basicShader));
	d->SetPhysicsObject(new PhysicsObject(&d->GetTransform(), d->GetBoundingVolume()));

	d->GetPhysicsObject()->SetInverseMass(inverseMass);
	d->GetPhysicsObject()->InitCubeInertia();
	d->SetDefaultPos(position);
	
	return d;
}