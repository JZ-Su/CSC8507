#include "GameObject.h"
#include "Boss.h"
#include "Window.h"
#include"BasicExamples.h"

using namespace NCL;
using namespace CSC8503;
Boss::Boss(const std::string& objectname)
{
	//ResetJumpTimer(1.0f);
	name = "boss";
	tag = objectname;
	if (name == tag)
	{
		health = 60;
		collectibles = 0;
		timer = 300;
	}

}
void Boss::UpdateBoss(float dt)
{
	//updateJumpTimer(dt);
}
float Boss::updateTimer(float dt)
{
	if (timer > 0)
	{
		timer -= dt;
	}

	return timer;
}
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
float Boss::updateHealth(float inhealth)
{
	health += inhealth;
	return health;
}
float Boss::updateCollectibles(int collected)
{
	collectibles += collected;
	return collectibles;
}

void Boss::OnCollisionBegin(GameObject* otherObject) {
	if (otherObject->GetTag() == "Bonus")
	{
		otherObject->Deactivate();
		this->updateCollectibles(1);
		//std::cout << "the collectible updated.......:" << this->GetCollectibles();
	}
	if (otherObject->GetTag() == "Enemy")
	{
		this->updateHealth(-0.2);
		//std::cout << "the Health updated.......:" << this->GetHealth();
	}
	if (otherObject->GetTag() == "Ground")
	{
		//changeLevel = true;

		canJump = true;
	}

}

void Boss::OnCollisionEnd(GameObject* otherObject) {

	if (otherObject->GetTag() == "Ground")
	{
		//changeLevel = true;
		canJump = false;

	}

}



float Boss::calculateDistance(Vector3 pos1, Vector3 pos2) {
	float distance = std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2) + std::pow(pos2.z - pos1.z, 2));
	return distance;
}


void Boss::BossBehaviourTree(Player* player) {
	float attackRange = 50.0f;
	float distanceToTarget = calculateDistance(GetTransform().GetPosition(), player->GetTransform().GetPosition());
	BehaviourAction* patrol = new BehaviourAction("patrol", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise) {
			std::cout << "Patrolling\n";
			//Debug::Print("Patrolling", GetTransform().GetPosition());
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
		});
	BehaviourAction* attack = new BehaviourAction("attack", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			//Debug::Print("Attacking", GetTransform().GetPosition());
			std::cout << "Attacking.\n";

			state = Ongoing;
		}
		else if (state == Ongoing) {
			std::cout << "Ongoing.\n";

			GetTransform().SetPosition(player->GetTransform().GetPosition());
		}
		return state;
		});
	if (distanceToTarget > attackRange) {
		patrol->Execute(0.0f); 
	}
	else {
		attack->Execute(0.0f); 
	}
	BehaviourSequence* sequence = new BehaviourSequence("idle");
	sequence->AddChild(patrol);
	BehaviourSelector* selection = new BehaviourSelector("attack");
	selection->AddChild(attack);
	rootSequence = new BehaviourSequence("Root Sequence");
	rootSequence->AddChild(sequence);
	rootSequence->AddChild(selection);
	rootSequence->Reset();
	state = Ongoing;
}
