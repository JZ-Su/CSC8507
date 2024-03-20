#include "../NCLCoreClasses/KeyboardMouseController.h"

#pragma once
#include "GameTechRenderer.h"
#ifdef USEVULKAN
#include "GameTechVulkanRenderer.h"
#endif
#include "PhysicsSystem.h"
#include "StateGameObject.h"
#include "NavigationGrid.h"
#include "GameLevel.h"
#include "BasicExamples.h"
#include "MeshAnimation.h"
#include "SoundManager.h"

namespace NCL {
	namespace CSC8503 {
		enum GameState {
			MainMenu,   Ranking, Start,
			ModeSelect, OnGoing, Pause,
			Win,        Lose,    Exit,
		};
		enum MainMenuState {
			MainMenu_Start,
			MainMenu_Ranking,
			MainMenu_Exit,
		};
		enum GameMode {
			TimeLimited,
			ScoreRequired,
		};
		enum RankingPage {
			TimeLimitedModePage,
			ScoreRequiredModePage,
		};
		struct RankScore {
			std::string playerName;
			float score;
		};

		class TutorialGame		{
		public:
			TutorialGame();
			~TutorialGame();

			virtual void UpdateGame(float dt);

			GameState GetState() { return gameState; }

			//void UpdateState();

			void ShowMainMenu(float dt);
			void ShowRanking(float dt);
			void SelectGameMode(float dt);
			void InitGame();
			void ShowPause(float dt);
			void ShowWin(float dt);
			void ShowLose(float dt);
			void ExecuteAttack(float dt);
			
			GameWorld* GetWorld() {
				return world;
			}
		protected:
			bool isDebug;

			virtual void InitialiseAssets();

			void InitCamera();
			void InitAudio();
			void UpdateKeys(float dt);

			void InitWorld();

			/*
			These are some of the world/object creation functions I created when testing the functionality
			in the module. Feel free to mess around with them to see different objects being created in different
			test scenarios (constraints, collision types, and so on). 
			*/

			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement(float dt);
			void UpdateProjMatrixFov(float df);
			//void BridgeConstraintTest();

#ifdef USEVULKAN
			GameTechVulkanRenderer*	renderer;
#else
			GameTechRenderer* renderer;
#endif
			PhysicsSystem*		physics;
			GameWorld*			world;

			KeyboardMouseController controller;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;

			Mesh*	capsuleMesh = nullptr;
			Mesh*	cubeMesh	= nullptr;
			Mesh*	sphereMesh	= nullptr;

			Texture*	basicTex	= nullptr;
			Shader*		basicShader = nullptr;

			//Coursework Meshes
			Mesh*	charMesh	= nullptr;
			Mesh*	enemyMesh	= nullptr;
			Mesh*	bonusMesh	= nullptr;

			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			Vector3 lockedOffset        = Vector3(0, 50, 50); //Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

			GameObject* objClosest = nullptr;

			GameLevel* gameLevel = nullptr;

			int score;
			Player* player;

			Boss* boss;
			GameObject* ghost;
			GameObject* shield;
			GameObject* iceCubeBullet;
			GameObject* fireBallBullet;
			MeshAnimation* bossAnimation;
			MeshAnimation* bossCheersAnimation;
			MeshAnimation* bossShootingAnimation;
			MeshAnimation* bossFlinchAnimation;
			MeshAnimation* ghostAnimation;
			MeshAnimation* playerIdleAnimation;
			MeshAnimation* playerWalkAnimation;
			MeshAnimation* playerJumpAnimation;
			MeshAnimation* bossAttackingAnimation;
			MeshAnimation* bossChasingAnimation;
			MeshAnimation* bossAngryAnimation;
			MeshAnimation* bossDeathAnimation;
			StateGameObject* ghostai;
			StateGameObject* ghostai2;

			float health;
			float totalTime;
			float bosshealth;
			float PlayerPreHealth;		
			float BossPrehHealth;
			float currenthealth;
			float healthLength;
			float BossHealthLendth;

			GameState gameState;
			MainMenuState mainMenuState;
			GameMode gameMode;

			RankingPage rankingPage;
			std::string nameString;
			std::vector<RankScore> TimeTable, ScoreTable;
			void LoadRankingFile();

			void DrawAnim(GameObject* g, MeshAnimation* anim);
			void UpdateAnim(GameObject* g, MeshAnimation* anim);
			void UpdateBossAnim(Boss* boss, MeshAnimation* bossAnimation, float dt);
			void UpdatePlayerAnim(Player* player, MeshAnimation* playerIdleAnimation, MeshAnimation* playerWalkAnimation, float dt);
			void UpdateGhostAnim(GameObject* ghost, MeshAnimation* ghostAnimation, float dt);
			void UpdateTrackingBall(Vector3 ballPosition,const Vector3& playerPosition, float speed, float dt);
			Vector3 GenerateRandomPropPositionInBounds(const Vector3& minBound, const Vector3& maxBound);
			void IceCubeBulletLogic(float dt);
			void FireBallBulletLogic(float dt);

			void UpdateShieldPosition(float dt);
			void RollStone(GameObject* stone, const Vector3& forceDirection, float forceMagnitude);
		

			void AddSound();
			void UpdateListenerPosition(float dt, const Vector3& playerPosition);
			float h = 0, v = 0;

			void UpdateLevel(float dt);
			void SwitchLevel();
			void PlayLevelBGM(const std::string& levelName);
			int coinCount;

			GameObject* exit = nullptr;
			GameObject* rollingRock = nullptr;
			GameObject* shieldProp = nullptr;
			GameObject* speedProp = nullptr;
			bool hasRotation = true;
			bool playerIsHit = false;
			bool isCloseToPlayer = false;
			bool hasReverse = false;
			int mapIndex = 0;

			int iceCubeBulletFrames = 0;
			int currentLevel;
			int randomBullet;
			GameObject* portal;
			float shootingTimer = 0.0f;
			float attackingTimer = 0.0f;
			float shieldPropTimer = 0.0f;
			const float shieldPropDuration = 18.6f;
			float bossAngryTimer = 0.0f;
			const float bossAngryDuration = 1.6f;
			float speedPropTimer = 0.0f;
			const float speedPropDuration = 10.0f;
			const float attackingDuration = 0.4f;
			const float shootingDuration = 0.65f;
			float bossDeathTimer = 0.0f;
			const float bossDeathDuration = 0.39f;
			bool playShootingAnimation = false;
			float hitBackTimer = 0.0f;
			const float hitBackDuration = 1.0f;
			float meleeAttackedTimer = 0.0f;
			const float meleeAttackedDuration = 1.0f;
			float propSpawnTimer = 0.0f;
			float propSpawnCooldown = 3.5f;
			static std::vector<std::string> itemList;

			SoundManager soundManager;
			Vector3 previousMainCameraPosition;
			bool lastWalkingState = false;
			Vector3 mainCameraPosition;
			bool isWalking = false;
			string currentBGM="";

			void UpdateLevel3UI();
		};
	}
}

