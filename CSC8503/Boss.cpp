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
	remoteAttackRange = 90.0f;
	meleeAttackRange = 10.0f;
	bossHealth = 100.0f;
	isShooting = false;
	hasFireBallBullet = true;
	Idle = new BehaviourAction("Idle", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise) {
			//std::cout << "Idle init\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {

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
			//std::cout << "RAttacking init.\n";

			state = Ongoing;
		}
		else if (state == Ongoing) {
			this->distanceToTarget = calculateDistance(GetTransform().GetPosition(), this->player->GetTransform().GetPosition());
			if (this->distanceToTarget > this->remoteAttackRange) {
				isShooting = false;
				return Failure;
			}
			else {
				Debug::DrawLine(GetTransform().GetPosition(), this->player->GetTransform().GetPosition(), Debug::RED);
				Debug::DrawCollisionBox(this);
				Debug::DrawCollisionBox(this->player);
				std::cout << "attacking----" << this->distanceToTarget << std::endl;
				isShooting = true;
				return Ongoing;
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
	selection->AddChild(RemoteAttack);
	selection->AddChild(Idle);
	//	selection->AddChild(MeleeAttack);
	//	selection->AddChild(Flinches);
	//	selection->AddChild(Death);
	rootSequence = new BehaviourSequence("Root Sequence");
	rootSequence->AddChild(selection);
}

void Boss::Update(float dt) {

	if (rootSequence != nullptr) {
		rootSequence->Execute(dt);
	}

}

float Boss::calculateDistance(Vector3 pos1, Vector3 pos2) {
	float distance = std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2) + std::pow(pos2.z - pos1.z, 2));
	return distance;
}