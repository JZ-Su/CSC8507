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
	remoteAttackRange = 30.0f;
	meleeAttackRange = 10.0f;
	bossHealth = 100.0f;
	Idle = new BehaviourAction("Idle", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise) {
			std::cout << "Idle\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			this->distanceToTarget = calculateDistance(GetTransform().GetPosition(), this->player->GetTransform().GetPosition());
			if (this->distanceToTarget <= this->remoteAttackRange) {
				GetTransform().SetPosition(GetTransform().GetPosition() + Vector3(0, 1, 0));

				/*std::cout << "Player detected close! Switching to attack mode.\n";*/
				std::cout << this->distanceToTarget << std::endl;
				
			}
			else {
				Debug::DrawLine(GetTransform().GetPosition(), this->player->GetTransform().GetPosition(), Debug::RED);
				Debug::DrawCollisionBox(this);
				Debug::DrawCollisionBox(this->player);
				/*std::cout << (GetTransform().GetPosition() - this->player->GetTransform().GetPosition()).Length() << std::endl;*/
				std::cout << this->distanceToTarget << std::endl;
				return Success;
			}
		}
		return state;//will be ongoing until success or condition to switch
		}
	);

	MeleeAttack = new BehaviourAction("MeleeAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "Attacking.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			std::cout << "MeleeAttack.\n";
			//GetTransform().SetPosition(this->player->GetTransform().GetPosition());
			return Success;
		}
		return state;
		}
	);
	RemoteAttack = new BehaviourAction("RemoteAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "Attacking.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			std::cout << "RemoteAttack.\n";
			//GetTransform().SetPosition(this->player->GetTransform().GetPosition());
			return Failure;
		}
		return state;
		}
	);
	Summon = new BehaviourAction("Summon", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "Attacking.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			std::cout << "Summon.\n";
			//GetTransform().SetPosition(player->GetTransform().GetPosition());
			return Success;
		}
		return state;
		}
	);
	Flinches = new BehaviourAction("Flinches", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "Attacking.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			std::cout << "Flinches.\n";
			GetTransform().SetPosition(this->player->GetTransform().GetPosition());
			return Success;
		}
		return state;
		}
	);
	Death = new BehaviourAction("Death", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "Attacking.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			std::cout << "Death.\n";
			GetTransform().SetPosition(this->player->GetTransform().GetPosition());
			return Success;
		}
		return state;
		}
	);

	BehaviourSequence* sequence = new BehaviourSequence("idle");
	sequence->AddChild(Idle);
	BehaviourSelector* selection = new BehaviourSelector("attack");
	selection->AddChild(RemoteAttack);
	rootSequence = new BehaviourSequence("Root Sequence");
	rootSequence->AddChild(sequence);
	rootSequence->AddChild(selection);
	if (rootSequence != nullptr) {
		rootSequence->Execute(1.0f);
	}
	if (distanceToTarget > remoteAttackRange) {
		Idle->Execute(1.0f);
	}
	else if (distanceToTarget > meleeAttackRange && distanceToTarget <= remoteAttackRange) {
		RemoteAttack->Execute(1.0f);
	}
	else if (distanceToTarget <= meleeAttackRange) {
		MeleeAttack->Execute(1.0f);
	}
	/*while (bossHealth > 0) {
		rootSequence->Reset();
		if (rootSequence != nullptr) {
			rootSequence->Execute(1.0f);
		}
		if (distanceToTarget > remoteAttackRange) {
			Idle->Execute(1.0f);
		}
		else if (distanceToTarget > meleeAttackRange && distanceToTarget <= remoteAttackRange) {
			RemoteAttack->Execute(1.0f);
		}
		else if (distanceToTarget <= meleeAttackRange) {
			MeleeAttack->Execute(1.0f);
		}
	}*/
}
float Boss::calculateDistance(Vector3 pos1, Vector3 pos2) {
	float distance = std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2) + std::pow(pos2.z - pos1.z, 2));
	return distance;
}
