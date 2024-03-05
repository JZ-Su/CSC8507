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
	capsuleMesh = render->LoadMesh("capsule.msh");
	charMesh    = render->LoadMesh("Keeper.msh");
	//bossMesh	= render->LoadMesh("Role_T.msh");
	//playerMesh = render->LoadMesh("Male_Guard.msh");
	//bossMesh = render->LoadMesh("MaleA.msh");
	bossMesh = render->LoadMesh("Male_Guard.msh");
	playerMesh = render->LoadMesh("FemaleA.msh");
	ghostMesh = render->LoadMesh("Ghost.msh");
	goatMesh    = render->LoadMesh("goat.msh");
	shieldMesh = render->LoadMesh("Shield.msh");
	bookshelfMesh = render->LoadMesh("bookshelf.msh");
	tableMesh = render->LoadMesh("table.msh");
	columnMesh = render->LoadMesh("column.msh");
	stairMesh = render->LoadMesh("Stair.msh");
	handrailMesh = render->LoadMesh("handrail.msh");
	coinMesh = render->LoadMesh("coin.msh");

	basicTexture = render->LoadTexture("checkerboard.png");
	IceCubeTexture = render->LoadTexture("IceCube.jpg");
	shieldTexture = render->LoadTexture("Shield_AlbedoTransparency.png");
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
	layerTexture[0] = render->LoadTexture("Floor/floor1_color.jpg");
	layerTexture[1] = render->LoadTexture("Floor/floor1_normal.png");
	layerTexture[2] = DefualtTexture[2];
	layerTexture[3] = render->LoadTexture("Floor/floor1_roughness.jpg");
	layerTexture[4] = render->LoadTexture("Floor/floor1_ao.jpg");
	layerTexture[5] = render->LoadTexture("Floor/floor1_height.png");
	ceilingTexture[0] = render->LoadTexture("Ceiling/ceiling_color.jpg");
	ceilingTexture[1] = render->LoadTexture("Ceiling/ceiling_normal.png");
	ceilingTexture[2] = DefualtTexture[2];
	ceilingTexture[3] = render->LoadTexture("Ceiling/ceiling_roughness.jpg");
	ceilingTexture[4] = render->LoadTexture("Ceiling/ceiling_ao.jpg");
	ceilingTexture[5] = render->LoadTexture("Ceiling/ceiling_height.png");
	
	//bossMat = new MeshMaterial("Role_T.mat");
	//playerMat = new MeshMaterial("Male_Guard.mat");
	//bossMat = new MeshMaterial("MaleA.mat");
	bossMat = new MeshMaterial("Male_Guard.mat");
	playerMat = new MeshMaterial("FemaleA.mat");
	ghostMat = new MeshMaterial("Ghost.mat");
	shieldMat = new MeshMaterial("Shield.mat");
	bookshelfMat = new MeshMaterial("bookshelf.mat");
	tableMat = new MeshMaterial("table.mat");
	columnMat = new MeshMaterial("Column.mat");
	stairMat = new MeshMaterial("Stair.mat");
	handrailMat = new MeshMaterial("handrail.mat");
	coinMat = new MeshMaterial("coin.mat");

	basicShader = render->LoadShader("scene.vert", "scene.frag");
	floorShader = render->LoadShader("scene.vert", "scene_uv.frag");
	modelShader = render->LoadShader("model.vert", "model.frag");
	bossShader = render->LoadShader("SkinningVertex.vert", "TexturedFragment.frag");
	playerShader = render->LoadShader("SkinningVertex.vert", "player.frag");
	ghostShader = render->LoadShader("SkinningVertex.vert", "ghost.frag");

	//bossAnimation = new MeshAnimation("Role_T.anm");
	bossAnimation = new MeshAnimation("Taunt.anm");
	bossCheersAnimation = new MeshAnimation("Angry.anm");
	bossShootingAnimation = new MeshAnimation("Gunfire1.anm");
	bossFlinchAnimation = new MeshAnimation("Flinches.anm");
	bossChasingAnimation = new MeshAnimation("StepForwardTwoHand.anm");
	bossAttackingAnimation = new MeshAnimation("Stow.anm");
	//bossAnimation = new MeshAnimation("Male_Jump.anm");
	playerIdleAnimation = new MeshAnimation("Female_Stand.anm");
	//playerWalkAnimation = new MeshAnimation("StepForwardTwoHand.anm");
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

	delete playerMat;
	delete bossMat;
	delete ghostMat;
	delete bookshelfMat;
	delete tableMat;
	delete columnMat;
	delete stairMat;
	delete handrailMat;
	delete coinMat;

	delete basicTexture;
	for (int i = 0; i < 5; i++)
		delete DefualtTexture[i];
	for (int i = 0; i < 6; i++)
		delete floorTexture[i];
	for (int i = 0; i < 6; i++)
		delete ceilingTexture[i];

	delete basicShader;
	delete floorShader;
	delete modelShader;
	delete ghostShader;
	delete bossShader;
	delete playerShader;
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

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->SetTag("Ground");
	return cube;
}

GameObject* BasicExamples::CreateLayer(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* cube = new GameObject("layer");

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2);
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, layerTexture[0], basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetRenderObject()->SetDefaultTexture(layerTexture[1], 1);
	cube->GetRenderObject()->SetDefaultTexture(layerTexture[2], 2);
	cube->GetRenderObject()->SetDefaultTexture(layerTexture[3], 3);
	cube->GetRenderObject()->SetDefaultTexture(layerTexture[4], 4);
	cube->GetRenderObject()->SetDefaultTexture(layerTexture[5], 5);

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	//cube->SetTag("Ground");
	return cube;
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

	cube->GetRenderObject()->SetColour(Vector4(0, 1, 0.5, 0.0));
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
	LoadMaterialTextures(ghost, ghostMesh, ghostMat, render);
	ghost->GetPhysicsObject()->SetInverseMass(inverseMass);
	ghost->GetPhysicsObject()->InitCubeInertia();
	ghost->GetRenderObject()->SetColour(Vector4(1, 1, 1, 0.5));

	return ghost;
}

Boss* BasicExamples::CreateBoss(const Vector3& position, const Vector3& dimensions, Player* player, float inverseMass) {
	Boss* character = new Boss(player);
	//character->BossBehaviourTree(player);
	AABBVolume* volume = new AABBVolume(dimensions);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform().SetScale(dimensions * 2).SetPosition(position).SetOrientation(Matrix4::Rotation(180, Vector3(0, 1, 0)));
	character->GetTransform().SetCollisionOffset(Vector3(0, dimensions.y, 0));

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
		if (matEntry->GetEntry("Metallic", &filenameMetal)) {
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
		if (matEntry->GetEntry("AO", &filenameAo)) {
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
	return coin;
}

Door* BasicExamples::CreateDoor(const Vector3& position, const Vector3& dimensions, float inverseMass, float rotation) {
	Door* d = new Door(player, position, rotation);
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
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform().SetScale(sphereSize).SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, nullptr, basicShader));
	sphere->GetRenderObject()->SetColour(Vector4(1.0, 0.4, 0.0, 1.0));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();
	sphere->SetIsHiding(true);
	sphere->SetExistenceTime(0.0f);
	return sphere;
}

GameObject* BasicExamples::CreateIceCubeBullet(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* cube = new GameObject("icecubebullet");

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2);
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, IceCubeTexture, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->SetIsHiding(true);
	cube->SetExistenceTime(0.0f);

	return cube;
}

GameObject* BasicExamples::CreateShield(const Vector3& position, const Vector3& dimensions, float inverseMass) {
	GameObject* shield = new GameObject("shield");

	AABBVolume* volume = new AABBVolume(Vector3(1.2, 1.2, 0.2) * dimensions);
	shield->GetTransform().SetCollisionDimensions(Vector3(1.2, 1.2, 0.2) * dimensions);
	shield->SetBoundingVolume((CollisionVolume*)volume);

	shield->GetTransform().SetScale(dimensions * 2).SetPosition(position);
	shield->SetRenderObject(new RenderObject(&shield->GetTransform(), shieldMesh, shieldTexture, modelShader));
	shield->SetPhysicsObject(new PhysicsObject(&shield->GetTransform(), shield->GetBoundingVolume()));
	//shield->GetRenderObject()->isAnimation = true;
	//LoadMaterialTextures(shield, shieldMesh, shieldMat, render);

	shield->GetPhysicsObject()->SetInverseMass(inverseMass);
	shield->GetPhysicsObject()->InitCubeInertia();
	return shield;
}
