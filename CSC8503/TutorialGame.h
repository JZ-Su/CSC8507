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
			

		protected:
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			void InitWorld();

			/*
			These are some of the world/object creation functions I created when testing the functionality
			in the module. Feel free to mess around with them to see different objects being created in different
			test scenarios (constraints, collision types, and so on). 
			*/

			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

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

			GameObject* ghost;
			MeshAnimation* bossAnimation;
			MeshAnimation* ghostAnimation;
			MeshAnimation* playerIdleAnimation;
			MeshAnimation* playerWalkAnimation;

			GameObject* floor;
			GameObject* goalArea;

			std::vector<GameObject*> linkObjects;

			NavigationGrid* GRID;

			float totalTime;
			float timeInterval;

			std::vector<std::vector<GameObject*>> keeperList;

			GameState gameState;

			MainMenuState mainMenuState;
			GameMode gameMode;

			GameObject* blocker;

			RankingPage rankingPage;
			std::string nameString;
			std::vector<RankScore> TimeTable, ScoreTable;
			void LoadRankingFile();

			void DrawAnim(GameObject* g, MeshAnimation* anim);
			void UpdateAnim(GameObject* g, MeshAnimation* anim);
			void UpdateBossAnim(GameObject* boss, MeshAnimation* bossAnimation, float dt);
			void UpdatePlayerAnim(Player* player, MeshAnimation* playerIdleAnimation, MeshAnimation* playerWalkAnimation, float dt);
			void UpdateGhostAnim(GameObject* ghost, MeshAnimation* ghostAnimation, float dt);
			float h = 0, v = 0;

			bool hasRotation = true;
			bool hasReverse = false;
			int mapIndex = 0;

			int currentLevel = 1;
		};
	}
}

