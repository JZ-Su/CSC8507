#pragma once
#include "GameObject.h"
#include "Window.h"
#include "GameWorld.h"
namespace NCL {
	namespace CSC8503 {
		class Player :public GameObject
		{
		public:
			Player( const std::string& objectname = "");
			~Player();
			void UpdatePlayer(float dt);
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
			void  ResetPlayerValues(float intimer)
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
			bool canJump = true;
			//add timer
		};
	}
}