#include "GameObject.h"
#include "Player.h"
#include "Window.h"

using namespace NCL;
using namespace CSC8503;


std::vector<Player::itemname> Player::playeritem;

Player::Player( const std::string& objectname)
{

	name = "player";
	tag = objectname;
	if (name == tag)
	{
		health = 60;
		collectibles = 0;
		timer = 300;
	}
//itemname newentry;
//newentry.tag = "RedBottle";
//playeritem.emplace_back(newentry);
//newentry.tag = "GreenBottle";
//playeritem.emplace_back(newentry);
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
bool Player::updateJumpTimer(float dt)
{
	if (jumpTimer > 0)
	{
		timer -= dt;
	}
	else
	{
		canJump = true;
	}
	return canJump;
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
		otherObject->Deactivate();
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
		//changeLevel = true;

		canJump = true;
	}
	//if (otherObject->GetTag() == "RedBottle") {
	//	playeritem.emplace_back(otherObject);
	//	
	//}

	
}

void Player::OnCollisionEnd(GameObject* otherObject) {
	
	if (otherObject->GetTag() == "Ground")
	{
		//changeLevel = true;
		canJump = false;

	}
	
}

void Player::DrawItemui() {
	int distance = 0.2;
	int i = 0;

	if (!playeritem.empty()) {
		//for (int i = 0; i < playeritem.size(); i++) {
		//	playeritem[i];
		//}
		//for (const auto& item : playeritem) {
		//	if (playeritem.size() < 4) {
		//		if ((playeritem.begin() + i)->tag == "RedBottle") {
		//			GameUI::CreateGameUI({ Vector3(-0.5,-0.7,-1) + Vector3(i * distance,0,0),Vector3(-0.5,-0.9,-1) + Vector3(i * distance,0,0) ,
		//			Vector3(-0.3,-0.9,-1) + Vector3(i * distance,0,0),Vector3(-0.3,-0.7,-1) + Vector3(i * distance,0,0) },
		//				{ Vector2(0.0f,1.0f), Vector2(0.0f,0.0f), Vector2(1.0f,0.0f), Vector2(1.0f,1.0f) }, 512, 512, 4, 0, "redbootle.png", "Item");
		//			++i;
		//			break;

		//		}
		//		if ((playeritem.begin() + i)->tag == "GreenBottle") {
		//			GameUI::CreateGameUI({ Vector3(-0.5,-0.7,-1) + Vector3(i * distance,0,0),Vector3(-0.5,-0.9,-1) + Vector3(i * distance,0,0) ,
		//			Vector3(-0.3,-0.9,-1) + Vector3(i * distance,0,0),Vector3(-0.3,-0.7,-1) + Vector3(i * distance,0,0) },
		//				{ Vector2(0.0f,1.0f), Vector2(0.0f,0.0f), Vector2(1.0f,0.0f), Vector2(1.0f,1.0f) }, 512, 512, 4, 0, "greenbottle.png", "Item");
		//			++i;
		//			break;
		//		}
		//	}
		//}
			//  itemname Playeritem[4] = {null};
			//	for (int i = 0; i < 4;i++) {
			//	if(playeritem[i]==null)
			//		continue;
			//	if (playeritem[i].tag == "RedBottle") {
			//	
			//		GameUI::CreateGameUI({ Vector3(-0.5,-0.7,-1) + Vector3(i*distance,0,0),Vector3(-0.5,-0.9,-1)+Vector3(i * distance,0,0) ,
			//			Vector3(-0.3,-0.9,-1) + Vector3(i * distance,0,0),Vector3(-0.3,-0.7,-1) + Vector3(i * distance,0,0) },
			//			{ Vector2(0.0f,1.0f), Vector2(0.0f,0.0f), Vector2(1.0f,0.0f), Vector2(1.0f,1.0f) }, 512, 512, 4, 0, "redbootle.png", "Item");
			//	}
			//	else if (playeritem[i].tag == "GreenBottle") {
			//		GameUI::CreateGameUI({ Vector3(-0.5,-0.7,-1) + Vector3(i * distance,0,0),Vector3(-0.5,-0.9,-1) + Vector3(i * distance,0,0) ,
			//			Vector3(-0.3,-0.9,-1) + Vector3(i * distance,0,0),Vector3(-0.3,-0.7,-1) + Vector3(i * distance,0,0) },
			//			{ Vector2(0.0f,1.0f), Vector2(0.0f,0.0f), Vector2(1.0f,0.0f), Vector2(1.0f,1.0f) }, 512, 512, 4, 0, "greenbottle.png", "Item");
			//	}
			//}
	}
}




void Player::UseItem(Player* player, int i) {
	if (!playeritem.empty()) {
		for (const auto& item : playeritem) {
			if ((playeritem.begin()+i)->tag == "RedBottle") {
			     player->addhealth(10);
			}
		if((playeritem.begin() + i)->tag == "GreenBottle")
		           player->addhealth(-10);
		}

		//for (int i = 0; i < 4; i++) {
		//	if (playeritem[i].tag == "RedBottle") {
		//		player->addhealth(10);
		//
		//	}
		//	else if (playeritem[i].tag == "GreenBottle") {
		//		player->addhealth(-10);
		//	}
		//}



	}
}

void Player::DeleteItem(int i) {
	playeritem.erase(playeritem.begin() + i);
}