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
namespace NCL {
	namespace CSC8503 {
		class Boss : public GameObject {
		public:
			Boss(Player* player);
			~Boss();
			void Update(float dt, Player* player);
			BehaviourSequence* rootSequence;
			BehaviourState state;
			float calculateDistance(Vector3 pos1, Vector3 pos2);
		private:
			BehaviourNode* behaviorTree;
			float distanceToTarget;
			float attackRange;
			float bossHealth;
			BehaviourAction* patrol;
			BehaviourAction* Dash;
			BehaviourAction* MeleeAttack;
			BehaviourAction* RemoteAttack;
			BehaviourAction* Summon;
			Player* player;
		};
	}
}