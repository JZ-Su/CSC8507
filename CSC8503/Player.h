#pragma once
#include "GameObject.h"
#include "Window.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
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
			void SetIsRencentlyHurt(bool IsRencentlyHurt) {
				isRencentlyHurt = IsRencentlyHurt;
			}
			bool getIsRencentlyHurt() {
				return isRencentlyHurt;
			}
			void SetIsAccelerated(bool IsAccelerated) {
				isAccelerated = IsAccelerated;
			}
			bool getIsAccelerated() {
				return isAccelerated;
			}
			void SetIsBeingHitBack(bool IsBeingHitBack) {
				isBeingHitBack = IsBeingHitBack;
			}
			bool getIsBeingHitBack() {
				return isBeingHitBack;
			}
			//bool updateJumpTimer(float dt);
			float updateTimer(float dt);
			float updateHealth(float inhealth);
			float updateCollectibles(int collected);
			virtual void OnCollisionBegin(GameObject* otherObject) override;
			virtual void OnCollisionEnd(GameObject* otherObject) override;
			bool changeLevel = false;

			void SetPreHealth(int inprehealth) { prehealth = inprehealth; }
			int GetPreHealth() {
				return prehealth;
			}

			void UseItem(int i);
			static const std::vector<std::string>& GetItemList() { return itemList; }

			char isSpacePressed;
			char btnState[4] = { 0,0,0,0 };
			Vector3 forceToBeAdded;
			Quaternion orientationNetPlayer;
			
			void MovePlayer(bool Forward, bool Backward, bool Right, bool Left)
			{
				Vector3 MoveDir = Vector3(0, 0, 0);
				if (Forward) { MoveDir += Vector3(0, 0, -1); }
				if (Backward) { MoveDir += Vector3(0, 0, 1); }
				if (Right) { MoveDir += Vector3(1, 0, 0); }
				if (Left) { MoveDir += Vector3(-1, 0, 0); }
				MoveDir = MoveDir.Normalised();

				MoveDir = GetTransform().GetOrientation()  * MoveDir;

				float f = getIsAccelerated() ? 3.0f : 1.5f;

				Vector3 force = MoveDir * f;
				
				//std::cout << "velocity : " << physicsObject->GetLinearVelocity().Length() << std::endl;
				this->physicsObject->AddForce(force);
				//Debug::DrawLine(this->transform.GetPosition(), this->transform.GetPosition() + force, Debug::RED, 0.0f);
			}

		protected:
			bool isRencentlyHurt;
			float health;
			int collectibles;
			float timer;
			float jumpTimer;
			bool isDead = false;
			bool isJumping = false;
			bool canJump = false;
			bool isWalk = false;
			bool isAccelerated = false;
			bool isBeingHitBack = false;
			//add timer
			int prehealth;
			static std::vector<std::string> itemList;
		};
	}
}