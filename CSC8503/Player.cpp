#include "GameObject.h"
#include "Player.h"
#include "Window.h"

using namespace NCL;
using namespace CSC8503;
Player::Player( const std::string& objectname)
{
	//ResetJumpTimer(1.0f);
	name = "player";
	tag = objectname;
	if (name == tag)
	{
		health = 60;
		collectibles = 0;
		timer = 300;
	}

}
void Player::UpdatePlayer(float dt)
{
	//updateJumpTimer(dt);
}
float Player::updateTimer(float dt)
{
	if (timer > 0)
	{
		timer -= dt;
	}

	return timer;
}

float Player::updateHealth(float inhealth)
{
	health += inhealth;
	return health;
}
float Player::updateCollectibles(int collected)
{
	collectibles += collected;
	return collectibles;
}

void Player::OnCollisionBegin(GameObject* otherObject) {
	if (otherObject->GetTag() == "Bonus")
	{
		otherObject->Deactivate();
		this->updateCollectibles(1);
		//std::cout << "the collectible updated.......:" << this->GetCollectibles();
	}
	if ( otherObject->GetTag() == "Enemy")
	{
		this->updateHealth(-0.2);
		//std::cout << "the Health updated.......:" << this->GetHealth();
	}
	if ( otherObject->GetTag() == "Ground")
	{
		//changeLevel = true;

		canJump = true;
	}
	
}

void Player::OnCollisionEnd(GameObject* otherObject) {
	
	if (otherObject->GetTag() == "Ground")
	{
		//changeLevel = true;
		canJump = false;

	}
	
}