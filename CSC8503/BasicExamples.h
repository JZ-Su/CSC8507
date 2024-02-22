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
#include "Boss.h"

#include "Door.h"

namespace NCL {
	class BasicExamples
	{
	public:
		BasicExamples(GameTechRenderer* render);
		BasicExamples();
		~BasicExamples();
		GameObject* CreateCube(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateFloor(const Vector3& position, const Vector3& dimensions, float inverseMass = 0.0f);
		GameObject* CreateCeiling(const Vector3& position, const Vector3& dimensions, float inverseMass = 0.0f);
		GameObject* CreateAABB(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateCubeOBB(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f, const Vector3& tilt=Vector3(1,0,0), int angle = 0.0f);
		GameObject* CreateSphere(const Vector3& position, float radius, float inverseMass = 10.0f);
		GameObject* CreateChar(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateGoat(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateCapsule(const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f);
		
		GameObject* CreateBookshelf(const Vector3& position, float inverseMass = 10.0f, const Vector3& tilt = Vector3(1, 0, 0), int angle = 0.0f);
		GameObject* CreateStairs(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f, const Vector3& tilt = Vector3(1, 0, 0), int angle = 0.0f);
		GameObject* CreateTable(const Vector3& position, float inverseMass = 10.0f, const Vector3& tilt = Vector3(1, 0, 0), int angle = 0.0f);
		GameObject* CreateColumn(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		
		GameObject* CreateGhost(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreatePlayer(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		/*Boss* CreateBoss(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);*/
		GameObjectWithBehavior* CreateBoss(const Vector3& position, const Vector3& dimensions, Player* player,float inverseMass = 10.0f);

		GameObject* ghost;

		Door* CreateDoor(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		Door* GetDoor() const { return door; }
	/*	GameObjectWithBehavior* GetBoss() {
			return boss;
		}*/
		/*Boss* GetBoss() {
			return boss;
		}*/

		MeshAnimation* getBossAnimation() {
			return bossAnimation;
		}

		GameObject* getGhost() {
			return ghost;
		}

		MeshAnimation* getGhostAnimation() {
			return ghostAnimation;
		}

		MeshAnimation* getplayerIdleAnimation() {
			return playerIdleAnimation;
		}

		MeshAnimation* getplayerWalkAnimation() {
			return playerWalkAnimation;
		}

		MeshAnimation* getplayerJumpAnimation() {
			return playerJumpAnimation;
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
		Mesh* goatMesh = nullptr;
		Mesh* capsuleMesh = nullptr;
		Mesh* bossMesh = nullptr;
		Mesh* playerMesh = nullptr;
		Mesh* ghostMesh = nullptr;
		Mesh* bookshelfMesh = nullptr;
		Mesh* tableMesh = nullptr;
		Mesh* columnMesh = nullptr;
		Mesh* stairMesh = nullptr;
		Mesh* handrailMesh = nullptr;
	
		Texture* basicTexture = nullptr;
		Texture* DefualtTexture[5] = { nullptr,nullptr,nullptr,nullptr,nullptr };
		Texture* floorTexture[6] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };
		Texture* ceilingTexture[6] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };
		Shader* basicShader = nullptr;
		Shader* floorShader = nullptr;
		Shader* modelShader = nullptr;
		Shader* ghostShader = nullptr;
		Shader* bossShader = nullptr;
		Shader* playerShader = nullptr;

		Player* player = nullptr;
		/*Boss* boss = nullptr;*/
		
		MeshMaterial* bossMat = nullptr;
		MeshMaterial* playerMat = nullptr;
		MeshMaterial* ghostMat = nullptr;
		MeshMaterial* bookshelfMat = nullptr;
		MeshMaterial* tableMat = nullptr;
		MeshMaterial* columnMat = nullptr;
		MeshMaterial* stairMat = nullptr;
		MeshMaterial* handrailMat = nullptr;

		MeshAnimation* bossAnimation = nullptr;
		MeshAnimation* playerIdleAnimation = nullptr;
		MeshAnimation* ghostAnimation = nullptr;
		MeshAnimation* playerWalkAnimation = nullptr;
		MeshAnimation* playerJumpAnimation = nullptr;
		
		GameTechRenderer* render;  // new render variable

		Door* door;
		
	};
}
