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

//float Boss::calculateDistance(const Position& pos1,const  Position& pos2) {
//	float distance = std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2));
//	return distance;
//}
//
//void Boss::BossBehaviourTree(Boss* boss, Player* player) {
//	float attackRange = 10.0f;
//	float distanceTotarget=calculateDistance((boss->GetTransform().GetPosition().x,boss->GetTransform().GetPosition().y), player->GetTransform().GetPosition());
//	BehaviourAction* patrol = new BehaviourAction("patrol", [&](float dt, BehaviourState state)->BehaviourState {
//		if (state == Initialise) {
//			Debug::Print("Patrolling", boss->GetTransform().GetPosition());
//			state = Ongoing;
//		}
//		else if (state == Ongoing) {
//			if (distanceToTarget <= attackRange) {
//				std::cout << "Player detected close! Switching to attack mode.\n";
//				return Success; 
//			}
//		}
//		return state;//will be ongoing until success or condition to switch
//		});
//	BehaviourAction* attack = new BehaviourAction("attack", [&](float dt, BehaviourState state) -> BehaviourState {
//		if (state == Initialise) {
//			Debug::Print("Attacking", boss->GetTransform().GetPosition());
//			state = Ongoing;
//		}
//		else if (state == Ongoing) {
//			
//		}
//		return state;
//		});
//	if (distanceToTarget > attackRange) {
//		patrol->execute(0.0f, Initialise); 
//	}
//	else {
//		attack->execute(0.0f, Initialise); 
//	}
//}
