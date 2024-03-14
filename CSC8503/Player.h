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
				health = std::max(health, 0.0f);
				health = std::min(health, 100.0f);
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
			int  GetIsWalk()
			{
				return isWalk;
			}
			void SetIsWalk(bool inIsWalk)
			{
				isWalk = inIsWalk;
			}
			bool IsJumping() const {
				return isJumping;
			}

			void SetIsJumping(bool jumping) {
				isJumping = jumping;
			}
			void setJumpTimer(float timer) {
				jumpTimer = timer;
			}
			bool updateJumpTimer(float dt) {
				if (IsJumping()) {
					jumpTimer -= dt;
					if (jumpTimer <= 0.0f) {
						return true;
					}
				}
				return false;
			}
			//bool updateJumpTimer(float dt);
			float updateTimer(float dt);
			float updateHealth(float inhealth);
			float updateCollectibles(int collected);
			virtual void OnCollisionBegin(GameObject* otherObject) override;
			virtual void OnCollisionEnd(GameObject* otherObject) override;
			bool changeLevel = false;

			void addhealth(float addhealth) {
				health += addhealth;
				health = std::max(health, 0.0f);
				health = std::min(health, 100.0f);
			}

			void SetPreHealth(int inprehealth) { prehealth = inprehealth; }
			int GetPreHealth() {
				return prehealth;
			}

			void UseItem(int i);
			static const std::vector<std::string>& GetItemList() { return itemList; }

		protected:
			float health;
			int collectibles;
			float timer;
			float jumpTimer;
			bool isDead = false;
			bool canJump = false;
			bool isWalk = false;
			bool isJumping = false;
			//add timer
			int prehealth;
			static std::vector<std::string> itemList;
		};
	}
}