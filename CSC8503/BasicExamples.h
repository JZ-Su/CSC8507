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
#include "Coin.h"
#include "Door.h"
#include "GhostAI.h"

namespace NCL {
	class BasicExamples
	{
	public:
		BasicExamples(GameTechRenderer* render);
		BasicExamples() {};
		~BasicExamples();
		GameObject* CreateCube(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateSignStart(const Vector3& position, const Vector3& dimensions, float rotation = 0.0f, float scale = 1.0f, bool enable = true); 
		GameObject* CreateSignContinue(const Vector3& position, const Vector3& dimensions, float rotation = 0.0f, float scale = 1.0f, bool enable = true);
		GameObject* CreateBigWall(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f, float scale = 0.05f);
		GameObject* CreateGreenWall(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f, float scale = 0.1f);

		GameObject* CreateFloor(const Vector3& position, const Vector3& dimensions, float inverseMass = 0.0f);
		GameObject* CreateCeiling(const Vector3& position, const Vector3& dimensions, float inverseMass = 0.0f);
		GameObject* CreateAABB(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateCubeOBB(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f, const Vector3& tilt=Vector3(1,0,0), int angle = 0.0f);
		GameObject* CreateSphere(const Vector3& position, float radius, float inverseMass = 10.0f);
		GameObject* CreateChar(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateGoat(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateCapsule(const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f);
		GameObject* CreateCoin(const Vector3& position, const Vector3& dimensions, float inverseMass = 0.0f, float rotation = 0.0f);
		
		//GameObject* CreateIceCubeBullet(const Vector3& position, float radius, float inverseMass);
		GameObject* CreateIceCubeBullet(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);

		GameObject* CreateFireBallBullet(const Vector3& position, float radius, float inverseMass);

		GameObject* CreateBookshelf(const Vector3& position, float inverseMass = 10.0f, const Vector3& tilt = Vector3(1, 0, 0), int angle = 0.0f);
		GameObject* CreateStairs(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f, const Vector3& tilt = Vector3(1, 0, 0), int angle = 0.0f);
		GameObject* CreateTable(const Vector3& position, float inverseMass = 10.0f, const Vector3& tilt = Vector3(1, 0, 0), int angle = 0.0f);
		GameObject* CreateColumn(const Vector3& position, float inverseMass = 10.0f);
		GameObject* CreateLayer(const Vector3& position, const Vector3& dimensions, float inverseMass = 0.0f);
		GameObject* CreateHandrail(const Vector3& position, float inverseMass = 0.0f, const Vector3& tilt = Vector3(1, 0, 0), int angle = 0.0f);

		GameObject* CreateLight(const Vector3& position, const Vector4& color, float radius, bool islight, bool isshadow);
		
		GameObject* CreateGhost(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateRollingRock(const Vector3& position, float radius, float inverseMass = 0.5f);
		GameObject* CreateShield(const Vector3& position, const Vector3& dimensions, float inverseMass);
		GameObject* CreateShieldProp(const Vector3& position, const Vector3& dimensions, float inverseMass = 0.0f);
		GameObject* CreateSpeedProp(const Vector3& position, const Vector3& dimensions, float inverseMass = 0.0f);
		GameObject* CreateWallLight(const Vector3& position, float inverseMass = 0.0f, const Vector3& tilt = Vector3(1, 0, 0), int angle = 0.0f);
		GameObject* CreateHangLight(const Vector3& position, const Vector3& dimensions, float inverseMass = 0.0f);
		Player*     CreatePlayer(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		Boss*       CreateBoss(const Vector3& position, const Vector3& dimensions, Player* player, float inverseMass = 10.0f);
		Door*       CreateDoor(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f, float rotation = 0.0f, float resDis = 15.0f);
		
		GameObject* CreatRedbottle(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f, float rotation = 0.0f);

		MeshAnimation* getBossAnimation() const { return bossAnimation; }
		MeshAnimation* getBossCheersAnimation() const { return bossCheersAnimation; }
		MeshAnimation* getBossAngryAnimation() const { return bossAngryAnimation; }
		MeshAnimation* getBossDeathAnimation() const { return bossDeathAnimation; }
		MeshAnimation* getBossShootingAnimation() const { return bossShootingAnimation; }
		MeshAnimation* getBossFlinchAnimation() const { return bossFlinchAnimation; }
		MeshAnimation* getGhostAnimation() const { return ghostAnimation; }
		MeshAnimation* getplayerIdleAnimation() const { return playerIdleAnimation; }
		MeshAnimation* getplayerWalkAnimation() const { return playerWalkAnimation; }
		MeshAnimation* getplayerJumpAnimation() const { return playerJumpAnimation; }
		MeshAnimation* getBossChasingAnimation() const { return bossChasingAnimation; }
		MeshAnimation* getBossAttackingAnimation() const { return bossAttackingAnimation; }

		GameObject* getIceCubeBullet() const { return iceCubeBullet; }
		GameObject* getFireBallBullet() const { return fireBallBullet; }

		GhostAI* CreateGhostAI(const Vector3& position, const Vector3& dimensions, GameObject* player, float inverseMass, const Vector3& star, const Vector3& end);

		void LoadMaterialTextures(GameObject* character, Mesh* mesh,MeshMaterial* material, GameTechRenderer* renderer);

		Boss*		GetBoss() const { return boss; }
		Player*		GetPlayer() const { return player; }
		std::vector<Player*> GetPlayerList() const{ return playerList; }
		GameObject*	GetGhost() const { return ghostmodle; }
		GameObject* GetShield() const{ return shield; }

		GhostAI* GetGhostai() const { return ghost; }
		GhostAI* GetGhostai2() const { return ghost2; }

	protected:
		Mesh* cubeMesh = nullptr;
		Mesh* sphereMesh = nullptr;
		Mesh* charMesh = nullptr;
		Mesh* goatMesh = nullptr;
		Mesh* capsuleMesh = nullptr;
		Mesh* bossMesh = nullptr;
		Mesh* playerMesh = nullptr;
		Mesh* ghostMesh = nullptr;
		Mesh* rockMesh = nullptr;
		Mesh* shieldMesh = nullptr;
		Mesh* speedPropMesh = nullptr;
		Mesh* bookshelfMesh = nullptr;
		Mesh* tableMesh = nullptr;
		Mesh* columnMesh = nullptr;
		Mesh* stairMesh = nullptr;
		Mesh* handrailMesh = nullptr;
		Mesh* coinMesh = nullptr;
		Mesh* wallLightMesh = nullptr;
		Mesh* hangLightMesh = nullptr;
		Mesh* layerMesh = nullptr;

		GameObject* CreateLight(const Transform* transform, const Vector4& color, float radius, bool islight, bool isshadow);


		Texture* basicTexture = nullptr;
		Texture* IceCubeTexture = nullptr;
		Texture* shieldTexture = nullptr;
		Texture* speedPropTexture = nullptr;
		Texture* FireBallTexture = nullptr;
		Texture* DefualtTexture[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
		Texture* floorTexture[6]   = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		Texture* ceilingTexture[6]   = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		Texture* wallTexture[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		Texture* greenWallTexture[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		Texture* signTexture[2];
		Shader* basicShader = nullptr;
		Shader* floorShader = nullptr;
		Shader* modelShader = nullptr;
		Shader* ghostShader = nullptr;
		Shader* bossShader = nullptr;
		Shader* playerShader = nullptr;
		Shader* lampShader = nullptr;

		MeshMaterial* bossMat = nullptr;
		MeshMaterial* playerMat = nullptr;
		MeshMaterial* ghostMat = nullptr;
		MeshMaterial* shieldMat = nullptr;
		MeshMaterial* bookshelfMat = nullptr;
		MeshMaterial* tableMat = nullptr;
		MeshMaterial* columnMat = nullptr;
		MeshMaterial* stairMat = nullptr;
		MeshMaterial* handrailMat = nullptr;
		MeshMaterial* coinMat = nullptr;
		MeshMaterial* wallLightMat = nullptr;
		MeshMaterial* hangLightMat = nullptr;
		MeshMaterial* layerMat = nullptr;

		MeshAnimation* bossAnimation = nullptr;
		MeshAnimation* bossCheersAnimation = nullptr;
		MeshAnimation* bossAngryAnimation = nullptr;
		MeshAnimation* bossDeathAnimation = nullptr;
		MeshAnimation* bossShootingAnimation = nullptr;
		MeshAnimation* bossFlinchAnimation = nullptr;
		MeshAnimation* bossChasingAnimation = nullptr;
		MeshAnimation* bossAttackingAnimation = nullptr;
		MeshAnimation* playerIdleAnimation = nullptr;
		MeshAnimation* ghostAnimation = nullptr;
		MeshAnimation* playerWalkAnimation = nullptr;
		MeshAnimation* playerJumpAnimation = nullptr;
		
		GameTechRenderer* render;  // new render variable
		
		Player* player = nullptr;
		std::vector<Player*> playerList;
		Boss* boss = nullptr;
		GameObject* ghostmodle;
		GameObject* shield;
		GameObject* iceCubeBullet;
		GameObject* fireBallBullet;

		GhostAI* ghost;
		GhostAI* ghost2;

	};
}
