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
	remoteAttackRange = 100.0f;
	meleeAttackRange = 60.0f;
	bossHealth = 100.0f;
	Idle = new BehaviourAction("Idle", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise) {
			std::cout << "Idle init\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			this->distanceToTarget = calculateDistance(GetTransform().GetPosition(), this->player->GetTransform().GetPosition());
			if (this->distanceToTarget <= this->remoteAttackRange) {
				Debug::DrawLine(GetTransform().GetPosition(), this->player->GetTransform().GetPosition(), Debug::RED);
				Debug::DrawCollisionBox(this);
				Debug::DrawCollisionBox(this->player);
				std::cout << this->distanceToTarget << std::endl;
				return Success;
			}
			else {
				std::cout << "idle.\n";
				return Ongoing;
			}
		}
		return state;//will be ongoing until success or condition to switch
		}
	);

	MeleeAttack = new BehaviourAction("MeleeAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "MeleeAttacking init.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			if (distanceToTarget <= meleeAttackRange) {
				std::cout << "MeleeAttack.\n";
				return Success;
			}
			else {
				std::cout << "Player out of range for melee attack.\n";
				return Failure;
			}
		}
		return state;
		}
	);
	RemoteAttack = new BehaviourAction("RemoteAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "RemoteAttacking init.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			if (distanceToTarget <= remoteAttackRange) {
				std::cout << "RemoteAttack.\n";
				return Success;
			}
			else {
				std::cout << "Player out of range for remote attack.\n";
				return Failure;
			}
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
			/*return Ongoing;*/
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
			/*PlayDeathAnimation();*/
			//RemoveBossObject();
			return Ongoing;
		}
		return state;
		}
	);
	Selector = new BehaviourSelector("FirstLevel");
	Selector->AddChild(Idle);
	Selector->AddChild(MeleeAttack);
	Selector->AddChild(RemoteAttack);
	Selector->AddChild(Flinches);
	Selector->AddChild(Death);
}

void Boss::Update(float dt) {
	distanceToTarget = calculateDistance(GetTransform().GetPosition(), player->GetTransform().GetPosition());
	Selector->Execute(dt);
}

float Boss::calculateDistance(Vector3 pos1, Vector3 pos2) {
	float distance = std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2) + std::pow(pos2.z - pos1.z, 2));
	return distance;
}

Boss::~Boss() {
	delete Idle;
	delete MeleeAttack;
	delete RemoteAttack;
	delete Flinches;
	delete Death;
	delete Parallel;
}