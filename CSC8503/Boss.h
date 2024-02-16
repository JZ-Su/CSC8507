#pragma once
#include "StateGameObject.h"
#include "Window.h"
#include "GameWorld.h"
#include "Player.h"

#include "BehaviourNode.h"
#include "BehaviourSelector.h"
#include "BehaviourSequence.h"
#include "BehaviourAction.h"
namespace NCL {
	namespace CSC8503 {
		class Boss :public StateGameObject
		{
		public:
			Boss(const std::string& objectname = "");
			~Boss();
			void UpdateBoss(float dt);
			float  GetHealth()
			{
				return health;
			}
			void SetHealth(float inHealth)
			{
				health = inHealth;
			}
			int  GetCollectibles()
			{
				return collectibles;
			}
			float  GetTimer()
			{
				return timer;
			}
			void  ResetBossValues(float intimer)
			{
				health = 60;
				timer = 60 * intimer;
			}
			void SetCollectibles(int incollectibles)
			{
				collectibles = incollectibles;
			}
			int  GetIsDead()
			{
				return isDead;
			}
			void SetIsDead(bool inIsDead)
			{
				isDead = inIsDead;
			}
			bool  GetCanJump()
			{
				return canJump;
			}
			void SetCanJump(bool inCanJump)
			{
				canJump = inCanJump;
			}
			int  GetIsWalk()
			{
				return isWalk;
			}
			void SetIsWalk(bool inIsWalk)
			{
				isWalk = inIsWalk;
			}
			bool updateJumpTimer(float dt);
			/*void  ResetJumpTimer(float intimer)
			{
				jumpTimer = intimer;
			}*/
			//bool updateJumpTimer(float dt);
			float updateTimer(float dt);
			float updateHealth(float inhealth);
			float updateCollectibles(int collected);
			virtual void OnCollisionBegin(GameObject* otherObject) override;
			virtual void OnCollisionEnd(GameObject* otherObject) override;
			bool changeLevel = false;
		protected:
			int health;
			int collectibles;
			float timer;
			//float jumpTimer = 0.0f;
			bool isDead = false;
			bool canJump = false;
			bool isWalk = false;
			//add timer
			/*void BossBehaviourTree(Boss* boss, Player* player);*/
		};
	}
}