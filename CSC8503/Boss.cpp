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
			std::cout << "Idle init\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			/*this->distanceToTarget = calculateDistance(GetTransform().GetPosition(), this->player->GetTransform().GetPosition());
			if (this->distanceToTarget <= this->remoteAttackRange) {
				GetTransform().SetPosition(GetTransform().GetPosition() + Vector3(0, 1, 0));
				std::cout << this->distanceToTarget << std::endl;
			}
			else {
				Debug::DrawLine(GetTransform().GetPosition(), this->player->GetTransform().GetPosition(), Debug::RED);
				Debug::DrawCollisionBox(this);
				Debug::DrawCollisionBox(this->player);
				std::cout << this->distanceToTarget << std::endl;
				return Success;
			}*/
			std::cout << "idle.\n";
			return Failure;
		}
		return state;//will be ongoing until success or condition to switch
		}
	);

	MeleeAttack = new BehaviourAction("MeleeAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "MAttacking init.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			std::cout << "MeleeAttack.\n";
			return Failure;
		}
		return state;
		}
	);
	RemoteAttack = new BehaviourAction("RemoteAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "RAttacking init.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			std::cout << "RemoteAttack.\n";
			return Failure;
		}
		return state;
		}
	);
	Flinches = new BehaviourAction("Flinches", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "Flinches init.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			std::cout << "Flinches.\n";
			GetTransform().SetPosition(this->player->GetTransform().GetPosition());
			return Failure;
		}
		return state;
		}
	);
	Death = new BehaviourAction("Death", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "Death init\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			std::cout << "Death.\n";
			GetTransform().SetPosition(this->player->GetTransform().GetPosition());
			return Failure;
		}
		return state;
		}
	);
	BehaviourSelector* selection = new BehaviourSelector("FirstLevel");
	selection->AddChild(Idle);
	selection->AddChild(MeleeAttack);
	selection->AddChild(RemoteAttack);
	selection->AddChild(Flinches);
	selection->AddChild(Death);
	rootSequence = new BehaviourSequence("Root Sequence");
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
}

void Boss::Update(float dt) {
	this->distanceToTarget = calculateDistance(GetTransform().GetPosition(), player->GetTransform().GetPosition());
	if (this->bossHealth > 0) {
		rootSequence->Reset();
		if (rootSequence != nullptr) {
			rootSequence->Execute(dt);
		}
		if (this->distanceToTarget > remoteAttackRange) {
			Idle->Execute(dt);
		}
		else if (this->distanceToTarget > meleeAttackRange && distanceToTarget <= remoteAttackRange) {
			RemoteAttack->Execute(dt);
		}
		else if (this->distanceToTarget <= meleeAttackRange) {
			MeleeAttack->Execute(dt);
		}
	}
}

float Boss::calculateDistance(Vector3 pos1, Vector3 pos2) {
	float distance = std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2) + std::pow(pos2.z - pos1.z, 2));
	return distance;
}
