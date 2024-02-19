#include "BasicExamples.h"
#include "RenderObject.h"
#include "PhysicsObject.h"

using namespace NCL;

BasicExamples::BasicExamples(GameTechRenderer* render) {
	cubeMesh    = render->LoadMesh("cube.msh");
	sphereMesh  = render->LoadMesh("sphere.msh");
	charMesh    = render->LoadMesh("Keeper.msh");
	goatMesh    = render->LoadMesh("goat.msh");
	capsuleMesh = render->LoadMesh("capsule.msh");
	QuadMesh = render->LoadMesh("Quad.msh");

	basicTexture = render->LoadTexture("checkerboard.png");
	floorTexture[0] = render->LoadTexture("Floor/floor_color.jpg");
	floorTexture[1] = render->LoadTexture("Floor/floor_normal.png");
	floorTexture[3] = render->LoadTexture("Floor/floor_roughness.jpg");
	basicShader = render->LoadShader("scene.vert", "scene.frag");
	testShader = render->LoadShader("scene.vert", "scene_uv.frag");
	Shader = new OGLShader("debug.vert", "debug.frag");
}

BasicExamples::~BasicExamples() {
	cubeMesh = nullptr;
	sphereMesh = nullptr;
	charMesh = nullptr;
	goatMesh = nullptr;
	capsuleMesh = nullptr;
	basicTexture = nullptr;
	basicShader = nullptr;
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
	cube->GetRenderObject()->SetDefaultTexture(floorTexture[3], 3);

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

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

	AABBVolume* volume = new AABBVolume(Vector3(0.6, 1.4, 0.6) * dimensions);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform().SetScale(dimensions * 2).SetPosition(position);
	character->SetRenderObject(new RenderObject(&character->GetTransform(), charMesh, basicTexture, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
	character->GetRenderObject()->SetColour(Vector4(1, 0, 0.5, 1));
	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitCubeInertia();
	player = character;

	return player;
}

StateGameObject* BasicExamples::CreateAItest(const Vector3& position, const Vector3& dimensions, GameObject* player, float inverseMass) {
	//GameObject* cube = new GameObject("cube");
	AABBVolume* volume = new AABBVolume(dimensions); 
	StateGameObject* ghost = new StateGameObject(player);

	ghost->SetBoundingVolume((CollisionVolume*)volume);
	ghost->GetTransform().SetPosition(position).SetScale(dimensions * 2);
	ghost->SetRenderObject(new RenderObject(&ghost->GetTransform(), cubeMesh, basicTexture, basicShader));

	ghost->SetPhysicsObject(new PhysicsObject(&ghost->GetTransform(), ghost->GetBoundingVolume()));

	ghost->GetPhysicsObject()->SetInverseMass(inverseMass);
	ghost->GetPhysicsObject()->InitCubeInertia();
	ghost->SetCollisionResponse(false);

	return ghost;
}


GameObject* BasicExamples::CreateQuad(const Vector3& position, const Vector3& dimensions, float inverseMass) {

	GameObject* quad = new GameObject("quad");
	AABBVolume* volume = new AABBVolume(dimensions);
	
	quad->SetBoundingVolume((CollisionVolume*)volume);
	quad->GetTransform().SetPosition(position).SetScale(dimensions * 2);
	quad->SetRenderObject(new RenderObject(&quad->GetTransform(), QuadMesh, basicTexture, basicShader));
	quad->SetPhysicsObject(new PhysicsObject(&quad->GetTransform(), quad->GetBoundingVolume()));
	quad->GetPhysicsObject()->SetInverseMass(inverseMass);
	quad->GetPhysicsObject()->InitCubeInertia();
	quad->SetCollisionResponse(false);
	quad->GetRenderObject()->SetTag("UI");
	//quad->SetTag("UI");

	/*Matrix4 proj = Matrix4::Orthographic(0.0, 100.0f, 100, 0, -1.0f, 1.0f);
	int matSlot = glGetUniformLocation(Shader->GetProgramID(), "viewProjMatrix");
	glUniformMatrix4fv(matSlot, 1, false, (float*)proj.array);

	vertices = quad->GetRenderObject()->GetMesh()->GetVertices();

	for (Vector3& vertex : vertices) {
		vertex = proj * vertex;
	}
	quad->GetRenderObject()->GetMesh()->SetVertexPositions(vertices);

	glGenBuffers(1, &BlineVertVBO);
	glBindBuffer(GL_ARRAY_BUFFER, BlineVertVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(Vector3), vertices.data());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);*/
	
	/*Matrix4 viewMatrix = Matrix4::Orthographic(-1.0f,10000.0f,this->windowSize.x/2.0, -(this->windowSize.x) / 2.0, this->windowSize.y/2.0,-(this->windowSize.y)/2.0);
	Matrix4 projMatrix = gameWorld.GetMainCamera().BuildProjectionMatrix(hostWindow.GetScreenAspect());
	Matrix4 viewProj = projMatrix * viewMatrix;*/


	return quad;
}