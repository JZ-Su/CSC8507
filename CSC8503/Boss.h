#pragma once
#include "GameObject.h"
#include "StateGameObject.h"
#include "Window.h"
#include "GameWorld.h"
#include "Player.h"

#include "BehaviourNode.h"
#include "BehaviourSelector.h"
#include "BehaviourSequence.h"
#include "BehaviourAction.h"
#include "BehaviourNodeWithChildren.h"
#include"BehaviourParallel.h"
#include"Inverter.h"
namespace NCL {
	namespace CSC8503 {
		class Boss : public GameObject {
		public:
			Boss(Player* player);
			~Boss();
			BehaviourState state;
			void Update(float dt);
			bool getShooting() {
				return isShooting;
			}
			bool getIsAttack() {
				return isAttack;
			}
			void setIsAttack(bool IsAttack) {
				isAttack = IsAttack;
			}
			void SetShooting(bool IsShooting) {
				isShooting = IsShooting;
			}
			void SetIsRencentlyHurt(bool IsRencentlyHurt) {
				isRencentlyHurt = IsRencentlyHurt;
			}
			bool getIsRencentlyHurt() {
				return isRencentlyHurt;
			}
			bool getHasIceCubeBullet() {
				return hasIceCubeBullet;
			}
			void setHasIceCubeBullet(bool HasIceCubeBullet) {
				hasIceCubeBullet = HasIceCubeBullet;
			}
			bool getHasFireBallBullet() {
				return hasFireBallBullet;
			}
			void setHasFireBallBullet(bool HasFireBallBullet) {
				hasFireBallBullet = HasFireBallBullet;
			}
			bool getIsChasing() {
				return isChasing;
			}
			void setIsChasing(bool IsChasing) {
				isChasing = IsChasing;
			}
			float getBossHealth() {
				return bossHealth;
			}
			void decreaseBossHealth(float Health) {
				bossHealth -= Health;
			}
			void increaseBossHealth(float Health) {
				bossHealth += Health;
			}
			float attackRange;
		private:
			bool isShooting;
			bool isAttack;
			bool isRencentlyHurt;
			bool hasIceCubeBullet;
			bool hasFireBallBullet;
			bool attackBool;
			bool attackCooldownBool;
			bool chaseBool;
			bool isChasing;
			float calculateDistance(Vector3 pos1, Vector3 pos2);
			float distanceToTarget;
			float remoteAttackRange;
			float chaseRange;
			float chaseSpeed;
			float bossHealth;
			float flinchAnimationTimer;
			BehaviourAction* Idle;
			BehaviourAction* ChaseAndAttack;
			BehaviourAction* RemoteAttack;
			BehaviourAction* Summon;
			BehaviourAction* Flinches;
			BehaviourAction* dizziness;
			BehaviourAction* Death;
			Player* player;
			BehaviourParallel* Combat;
			BehaviourSequence* Root;
			BehaviourParallel* Parallel;

		};
	}
}