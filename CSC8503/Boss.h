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
namespace NCL {
	namespace CSC8503 {
		class Boss : public GameObject {
		public:
			Boss(Player* player);
			~Boss();
			BehaviourSequence* rootSequence;
			BehaviourState state;
			void Update(float dt);
		private:
			float calculateDistance(Vector3 pos1, Vector3 pos2);
			float distanceToTarget;
			float remoteAttackRange;
			float meleeAttackRange;
			float bossHealth;
			BehaviourAction* Idle;
			BehaviourAction* MeleeAttack;
			BehaviourAction* RemoteAttack;
			BehaviourAction* Summon;
			BehaviourAction* Flinches;
			BehaviourAction* Death;
			Player* player;
			BehaviourParallel* Parallel;
			BehaviourSelector* Selector;
			BehaviourSequence* Sequence;
		};
	}
}