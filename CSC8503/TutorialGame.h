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
#include "MeshAnimation.h"
#include "SoundManager.h"

namespace NCL {
	namespace CSC8503 {
		enum GameState {
			MainMenu,	ModeSelect,	Start,
			OnGoing,	Pause,		End,
			Exit
		};
		enum MainMenuState {
			MainMenu_Start,
			MainMenu_Exit
		};
		enum GameMode {
			Single, Multi
		};

		class TutorialGame		{
		public:
			TutorialGame();
			~TutorialGame();

			void UpdateGame(float dt);

			GameState GetState() { return gameState; }

			//void UpdateState();

			void ShowMainMenu(float dt);
			void SelectGameMode(float dt);
			void InitGame(float dt);
			void ShowPause(float dt);
			void ShowEnd(float dt);
			void ExecuteAttack(float dt);
			
			GameWorld* GetWorld() {
				return world;
			}

			void DropMassiveItems();
			void DropItems();
			
		private:
			static TutorialGame* instance;
			static TutorialGame* GetGame() {
				return instance;
			};

		protected:
			bool isDebug;

			void InitialiseAssets();

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

			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			Vector3 lockedOffset        = Vector3(0, 50, 50); //Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

			GameLevel* gameLevel = nullptr;

			int score;
			Player* player;

			std::vector<Player*> playerlist;

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
			float skilltime;
			float cd;
			bool useskill = false;

			GameState gameState;
			MainMenuState mainMenuState;
			GameMode gameMode;

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
			void launchGhostMissle(float& progress);

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
			bool getRed = false;
			int mapIndex = 0;

			int iceCubeBulletFrames = 0;
			int currentLevel;
			int randomBullet;
			GameObject* portal;
			float shootingTimer = 0.0f;
			float attackingTimer = 0.0f;
			float shieldPropTimer = 0.0f;
			const float shieldPropDuration = 1.0f;
			float bossAngryTimer = 0.0f;
			const float bossAngryDuration = 1.6f;
			float speedPropTimer = 0.0f;
			const float speedPropDuration = 6.0f;
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
			bool pressedQ = false;
			bool bossDeathLogic = false;
			bool bossDeathLogicDone = false;
			bool playerDeathLogic = false;
			string currentBGM = "";
			float progress=0;
			float level3Timer = 0;
			void UpdateLevel3UI();
			vector<GameObject*> vec;
			std::vector<std::pair<GameObject*, float>> ghostListWithTimestamps;
		};
	}
}

