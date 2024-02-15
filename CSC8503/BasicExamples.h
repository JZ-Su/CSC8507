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
		GameObject* CreateBoss(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* boss;

		GameObject* getBoss() {
			return boss;
		}

		MeshAnimation* getBossAnimation() {
			return bossAnimation;
		}

		MeshAnimation* getplayerIdleAnimation() {
			return playerIdleAnimation;
		}

		MeshAnimation* getplayerWalkAnimation() {
			return playerWalkAnimation;
		}

		Player* GetPlayer()
		{
			return player;
		}

		StateGameObject* CreateAItest(const Vector3& position, const Vector3& dimensions,  GameObject* player,float inverseMass = 10.0f);

		GameObject* CreateTestMesh(const Vector3& position, const Vector3& dimensions, float inverseMass);
		void LoadMaterialTextures(GameObject* character, Mesh* mesh,MeshMaterial* material, GameTechRenderer* renderer);

	protected:
		Mesh* cubeMesh = nullptr;
		Mesh* sphereMesh = nullptr;
		Mesh* charMesh = nullptr;
		Mesh* bossMesh = nullptr;
		Mesh* playerMesh = nullptr;
		Mesh* ghostMesh = nullptr;
		Mesh* goatMesh = nullptr;
		Mesh* capsuleMesh = nullptr;

	
		Texture* basicTexture = nullptr;
		Texture* MetalTexture[3] = { nullptr,nullptr,nullptr };
		Texture* floorTexture[6] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };
		Shader* basicShader = nullptr;
		Shader* roleShader = nullptr;
		Shader* floorShader = nullptr;
		Shader* ghostShader = nullptr;
		Shader* bossShader = nullptr;
		Shader* playerShader = nullptr;

		Player* player = nullptr;

		MeshMaterial* bossMat = nullptr;
		MeshMaterial* playerMat = nullptr;
		MeshMaterial* ghostMat = nullptr;

		MeshAnimation* bossAnimation = nullptr;
		MeshAnimation* playerIdleAnimation = nullptr;
		MeshAnimation* ghostAnimation = nullptr;
		MeshAnimation* playerWalkAnimation = nullptr;
		
		GameTechRenderer* render;  // new render variable

	};
}
