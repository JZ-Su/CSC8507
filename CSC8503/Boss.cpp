#include "GameObject.h"
#include "Boss.h"
#include "Window.h"
#include "BasicExamples.h"
#include "RenderObject.h"
#include "GameWorld.h"

using namespace NCL;
using namespace CSC8503;
Boss::Boss(Player* player) {
	this->player = player;
	attackRange = 50.0f;
	bossHealth = 100.0f;
	distanceToTarget = calculateDistance(GetTransform().GetPosition(), player->GetTransform().GetPosition());
	patrol = new BehaviourAction("patrol", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise) {
			//std::cout << "Patrolling\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			if (distanceToTarget <= attackRange) {
				GetTransform().SetPosition(GetTransform().GetPosition() + Vector3(0, 1, 0));

				//std::cout << "Player detected close! Switching to attack mode.\n";
				return Success;
			}
			else {
				//std::cout << (GetTransform().GetPosition() - this->player->GetTransform().GetPosition()).Length() << std::endl;
			}
		}
		return state;//will be ongoing until success or condition to switch
		}
	);
	Dash = new BehaviourAction("Dash", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			//std::cout << "Attacking.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			//std::cout << "Ongoing.\n";
			GetTransform().SetPosition(this->player->GetTransform().GetPosition());
		}
		return state;
		}
	);
	MeleeAttack = new BehaviourAction("MeleeAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			//std::cout << "Attacking.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			//std::cout << "Ongoing.\n";
			//GetTransform().SetPosition(this->player->GetTransform().GetPosition());
		}
		return state;
		}
	);
	RemoteAttack = new BehaviourAction("RemoteAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			//std::cout << "Attacking.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			//std::cout << "Ongoing.\n";
			//GetTransform().SetPosition(this->player->GetTransform().GetPosition());
		}
		return state;
		}
	);
	Summon = new BehaviourAction("Summon", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			//std::cout << "Attacking.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			//std::cout << "Ongoing.\n";
			//GetTransform().SetPosition(player->GetTransform().GetPosition());
		}
		return state;
		}
	);

	BehaviourSequence* sequence = new BehaviourSequence("idle");
	sequence->AddChild(patrol);

	BehaviourSelector* selection = new BehaviourSelector("attack");
	selection->AddChild(RemoteAttack);
	rootSequence = new BehaviourSequence("Root Sequence");
	rootSequence->AddChild(sequence);
	rootSequence->AddChild(selection);

	rootSequence->Reset();
	state = Ongoing;
}

float Boss::calculateDistance(Vector3 pos1, Vector3 pos2) {
	float distance = std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2) + std::pow(pos2.z - pos1.z, 2));
	return distance;
}

void Boss::Update(float dt, Player* player) {
	if (behaviorTree != nullptr) {
		behaviorTree->Execute(dt);
	}
	if (distanceToTarget > attackRange) {
		patrol->Execute(dt);
	}
	else {
		MeleeAttack->Execute(dt);
	}
}