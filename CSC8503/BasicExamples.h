#pragma once
#include "GameTechRenderer.h"
#ifdef USEVULKAN
#include "GameTechVulkanRenderer.h"
#endif
#include "PhysicsSystem.h"

#include "StateGameObject.h"

#include "NavigationGrid.h"

#include "Player.h"

#include "MeshMaterial.h"

#include "MeshAnimation.h"

namespace NCL {
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
		GameObject* CreateRole(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* role;

		GameObject* getRole() {
			return role;
		}

		MeshAnimation* getRoleAnimation() {
			return roleAnimation;
		}

		Player* GetPlayer()
		{
			return player;
		}

		StateGameObject* CreateAItest(const Vector3& position, const Vector3& dimensions,  GameObject* player,float inverseMass = 10.0f);

		void LoadMaterialTextures(GameObject* character, Mesh* mesh,MeshMaterial* material, GameTechRenderer* renderer);

	protected:
		Mesh* cubeMesh = nullptr;
		Mesh* sphereMesh = nullptr;
		Mesh* charMesh = nullptr;
		Mesh* roleMesh = nullptr;
		Mesh* goatMesh = nullptr;
		Mesh* capsuleMesh = nullptr;

	
		Texture* basicTexture = nullptr;
		Texture* MetalTexture[3] = { nullptr,nullptr,nullptr };
		Texture* floorTexture[6] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };
		Shader* basicShader = nullptr;
		Shader* roleShader = nullptr;
		Shader* testShader = nullptr;
		Player* player = nullptr;

		MeshMaterial* roleMat = nullptr;

		MeshAnimation* roleAnimation = nullptr;
		GameTechRenderer* render;  // new render variable

	};
}
