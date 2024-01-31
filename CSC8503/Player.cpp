#include "GameObject.h"
#include "Player.h"
#include "Window.h"

using namespace NCL;
using namespace CSC8503;
Player::Player(GameWorld& game, const std::string& objectname) :world(game)
{

	name = "player";
	tag = objectname;
	if (name == tag)
	{
		health = 60;
		collectibles = 0;
		timer = 300;
	}

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
	if (this->tag == "player" && otherObject->GetTag() == "Bonus")
	{
		otherObject->deactivate();
		this->updateCollectibles(1);
		//std::cout << "the collectible updated.......:" << this->GetCollectibles();
	}
	if (this->tag == "player" && otherObject->GetTag() == "Enemy")
	{
		this->updateHealth(-0.2);
		//std::cout << "the Health updated.......:" << this->GetHealth();
	}
	/*if (this->tag == "player" && otherObject->GetName() == "Key1")
	{
		otherObject->deactivate();
		changeLevel = true;

	}
	if (this->tag == "player" && otherObject->GetName() == "Key2")
	{
		otherObject->deactivate();
		changeLevel = true;

	}*/
}