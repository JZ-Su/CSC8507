#include "GameObject.h"
#include "Player.h"
#include "Window.h"

using namespace NCL;
using namespace CSC8503;

std::vector<std::string> Player::itemList;

Player::Player( const std::string& objectname)
{
	//ResetJumpTimer(1.0f);
	name = "player";
	tag = objectname;
	if (name == tag)
	{
		health = 100;
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
	health = std::max(health, 0.0f);
	health = std::min(health, 100.0f);
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
		canJump = true;
	}
	if (otherObject->GetTag() == "item") {
		if (itemList.size() < 4) {
		itemList.push_back(otherObject->GetName());
		}
	}
	if (otherObject->GetTag() == "ghost") {
		this->GetTransform().SetPosition(Vector3(0, 10, 30));
	}
}

void Player::OnCollisionEnd(GameObject* otherObject) {
	if (otherObject->GetTag() == "Ground")
	{
		//changeLevel = true;
		canJump = false;
	}
}

void Player::UseItem(int i) {
	if (itemList.empty())  return;
	if (i > itemList.size() - 1)  return;

	if (itemList.at(i) == "redbottle") {
		float j = RandomValue(0, 3);
		if(j<=1)
			updateHealth(-10);
		else
			updateHealth(10);

        itemList.erase(itemList.begin() + i);
	}
}
