#pragma once
#include "GameWorld.h"
#include "../CSC8503/Player.h"
#include "../CSC8503/Boss.h"

namespace NCL {
	namespace CSC8503 {
		class PhysicsSystem	{
		public:
			PhysicsSystem(GameWorld& g);
			~PhysicsSystem();

			void Clear();

			void Update(float dt);

			void UseGravity(bool state) {
				applyGravity = state;
			}

			void SetGlobalDamping(float d) {
				globalDamping = d;
			}
			Vector3 GetGravity() const {
				return gravity;
			}
			void SetGravity(const Vector3& g);

			std::vector<GameObject*> GetCollisionDetectionList(GameObject* o);

			void HandleFireballBulletCollisionLogic(GameObject* i, GameObject* j);

			void HandleIceCubeBulletCollisionLogic(GameObject* i, GameObject* j);

			void HandleRollingRockCollisionLogic(GameObject* i, GameObject* j);

			void HandleCoinCollisionLogic(GameObject* i, GameObject* j);

			void HandleItemCollisionLogic(GameObject* i, GameObject* j);

			void ParryBackBullet(GameObject* bullet, Player* player, Boss* boss);

		protected:
			void BasicCollisionDetection();
			void BroadPhase();
			void NarrowPhase();

			void ClearForces();

			void IntegrateAccel(float dt);
			void IntegrateVelocity(float dt);

			void UpdateConstraints(float dt);

			void UpdateCollisionList();
			void UpdateObjectAABBs();

			void ImpulseResolveCollision(GameObject& a , GameObject&b, CollisionDetection::ContactPoint& p) const;

			GameWorld& gameWorld;

			bool	applyGravity;
			bool    skipImpulseResolveCollision;
			Vector3 gravity;
			float	dTOffset;
			float	globalDamping;
			float lineardamping;
			std::set<CollisionDetection::CollisionInfo> allCollisions;
			std::set<CollisionDetection::CollisionInfo> broadphaseCollisions;
			std::vector<CollisionDetection::CollisionInfo> broadphaseCollisionsVec;
			bool useBroadPhase		= true;
			int numCollisionFrames	= 5;
		};
	}
}

