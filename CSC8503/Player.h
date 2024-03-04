#pragma once
#include "GameObject.h"
#include "Window.h"
#include "GameWorld.h"
#include "GameUI.h"
namespace NCL {
	namespace CSC8503 {
		class Player :public GameObject
		{
		public:
			struct itemname {
				std::string tag;
			};

			Player( const std::string& objectname = "");
			~Player();
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
				health = 100;
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
			bool updateJumpTimer(float dt);
			float updateTimer(float dt);
			float updateHealth(float inhealth);
			float updateCollectibles(int collected);
			virtual void OnCollisionBegin(GameObject* otherObject) override;
			bool changeLevel = false;

			void addhealth(float addhealth) {
				health += addhealth;
			}

			void UseItem(Player* player,int i);
			void DrawItemui();
			void DeleteItem(int i);

		protected:
			float health=100;
			int collectibles;
			float timer;
			float jumpTimer = 0;
			bool isDead = false;
			bool canJump = false;
			bool isWalk = false;
			bool isJumping = false;
			
			static std::vector<itemname> playeritem;
			//static itemname Playeritem[4];
			
		};
	}
}