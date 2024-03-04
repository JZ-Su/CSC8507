#pragma once
#include "GameTechRenderer.h"
#ifdef USEVULKAN
#include "GameTechVulkanRenderer.h"
#endif
#include "PhysicsSystem.h"

#include "StateGameObject.h"

#include "NavigationGrid.h"

#include "Player.h"

namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	class BasicExamples
	{
	public:
		BasicExamples(GameTechRenderer* render);
		BasicExamples();
		~BasicExamples();
		GameObject* CreateCube(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateFloor(const Vector3& position, const Vector3& dimensions, float inverseMass = 0.0f);
		GameObject* CreateCubeOBB(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f, const Vector3& tilt=Vector3(1,0,0), int angle = 0.0f);
		GameObject* CreateSphere(const Vector3& position, float radius, float inverseMass = 10.0f);
		GameObject* CreateChar(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateGoat(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateCapsule(const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f);
		
		GameObject* CreatePlayer(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		Player* GetPlayer() { return player; }

		StateGameObject* CreateAItest(const Vector3& position, const Vector3& dimensions,  GameObject* player,float inverseMass = 10.0f);
		
		GameObject* CreateTestMesh(const Vector3& position, const Vector3& dimensions, float inverseMass);
		GameObject* CreateItem(const Vector3& position, const Vector3& dimensions, float inverseMass);


		void LoadMaterialTextures(GameObject* character, Mesh* mesh,MeshMaterial* material, GameTechRenderer* renderer);

		void ExportToObj(const Mesh& mesh, const std::string& filename);

	protected:
		Mesh* cubeMesh = nullptr;
		Mesh* sphereMesh = nullptr;
		Mesh* charMesh = nullptr;
		Mesh* goatMesh = nullptr;
		Mesh* capsuleMesh = nullptr;
		Mesh* QuadMesh = nullptr;

		Texture* basicTexture = nullptr;
		Texture* floorTexture[6] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };
		Shader* basicShader = nullptr;
		Shader* testShader = nullptr;
		Player* player;
		OGLShader* Shader;
		GLuint BlineVertVBO;

		vector<Vector3>  vertices;
	};
}
