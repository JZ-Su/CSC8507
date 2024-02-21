#include "GameObject.h"
#include "Boss.h"
#include "Window.h"
#include"BasicExamples.h"
#include <RenderObject.h>
#include"GameWorld.h"

using namespace NCL;
using namespace CSC8503;
//Boss::Boss(const std::string& objectname)
//{
//	//ResetJumpTimer(1.0f);
//	name = "boss";
//	tag = objectname;
//	if (name == tag)
//	{
//		health = 60;
//		collectibles = 0;
//		timer = 300;
//	}
//
//}
//void Boss::UpdateBoss(float dt)
//{
//	//updateJumpTimer(dt);
//}
//float Boss::updateTimer(float dt)
//{
//	if (timer > 0)
//	{
//		timer -= dt;
//	}
//
//	return timer;
//}
//bool Player::updateJumpTimer(float dt)
//{
//	if (jumpTimer <= 0)
//	{
//		canJump = true;
//		
//	}
//	else
//	{
//		jumpTimer -= dt;
//	}
//	return canJump;
//}
//float Boss::updateHealth(float inhealth)
//{
//	health += inhealth;
//	return health;
//}
//float Boss::updateCollectibles(int collected)
//{
//	collectibles += collected;
//	return collectibles;
//}

//void Boss::OnCollisionBegin(GameObject* otherObject) {
//	if (otherObject->GetTag() == "Bonus")
//	{
//		otherObject->Deactivate();
//		this->updateCollectibles(1);
//		//std::cout << "the collectible updated.......:" << this->GetCollectibles();
//	}
//	if (otherObject->GetTag() == "Enemy")
//	{
//		this->updateHealth(-0.2);
//		//std::cout << "the Health updated.......:" << this->GetHealth();
//	}
//	if (otherObject->GetTag() == "Ground")
//	{
//		//changeLevel = true;
//
//		canJump = true;
//	}
//
//}

//void Boss::OnCollisionEnd(GameObject* otherObject) {
//
//	if (otherObject->GetTag() == "Ground")
//	{
//		//changeLevel = true;
//		canJump = false;
//
//	}
//
//}

GameObjectWithBehavior::GameObjectWithBehavior(Player* player) {
	this->player = player;
	attackRange = 50.0f;
	distanceToTarget = calculateDistance(GetTransform().GetPosition(), player->GetTransform().GetPosition());
	patrol = new BehaviourAction("patrol", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise) {
			std::cout << "Patrolling\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			if (distanceToTarget <= attackRange) {
				GetTransform().SetPosition(GetTransform().GetPosition() + Vector3(0, 1, 0));

				std::cout << "Player detected close! Switching to attack mode.\n";
				return Success;
			}
			else {
				std::cout << (GetTransform().GetPosition() - player->GetTransform().GetPosition()).Length() << std::endl;
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
			std::cout << "Ongoing.\n";
			GetTransform().SetPosition(this->player->GetTransform().GetPosition());
		}
		return state;
		}
	);
	BehaviourAction* RemoteAttack = new BehaviourAction("RemoteAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "Attacking.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			std::cout << "Ongoing.\n";
			GetTransform().SetPosition(this->player->GetTransform().GetPosition());
		}
		return state;
		}
	);
	BehaviourAction* Summon = new BehaviourAction("Summon", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "Attacking.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			std::cout << "Ongoing.\n";
			GetTransform().SetPosition(player->GetTransform().GetPosition());
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

float GameObjectWithBehavior::calculateDistance(Vector3 pos1, Vector3 pos2) {
	float distance = std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2) + std::pow(pos2.z - pos1.z, 2));
	return distance;
}


void GameObjectWithBehavior::BossBehaviourTree(Player* player) {
	 
}

void GameObjectWithBehavior::Update(float dt, Player* player) {
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