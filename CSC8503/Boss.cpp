#include "GameObject.h"
#include "Boss.h"
#include "Window.h"

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