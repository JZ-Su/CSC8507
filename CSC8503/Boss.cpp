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

void BossBehaviourTree() {

}

//void TutorialGame::GooseBehaviourTree() {
//	float behaviourTimer;
//	float distanceTotarget;
//	BehaviourAction* sleep = new BehaviourAction("Sleep", [&](float dt, BehaviourState state)->BehaviourState {
//		if (state == Initialise) {
//			behaviourTimer = 100;
//			std::cout << "sleep!\n";
//			Debug::Print("Sleep:", Goose->GetTransform().GetPosition());
//			state = Ongoing;
//		}
//		else if (state == Ongoing) {
//			behaviourTimer -= dt;
//			Debug::Print("Awake!", Goose->GetTransform().GetPosition());
//			if (behaviourTimer <= 0.0f || totaltime >= 1) {
//				std::cout << "awake!\n";
//
//				return Success;
//			}
//		}
//		return state;//will be ongoing until success
//		});
//	BehaviourAction* goToDoor = new BehaviourAction("gotodoor", [&](float dt, BehaviourState state)->BehaviourState {
//		if (state == Initialise) {
//			std::cout << "Going to the door\n";
//			Debug::Print("Going to the door", Goose->GetTransform().GetPosition());
//			state = Ongoing;
//		}
//		else if (state == Ongoing) {
//			GoosePathfindingDoor();
//			while (physics->IsGooseDoor()) {
//				std::cout << "Going to the door!\n";
//				return Success;
//			}
//		}
//		return state;//will be ongoing until success
//		});
//	BehaviourAction* bypassDoor = new BehaviourAction("Bypass Door", [&](float dt, BehaviourState state)->BehaviourState {
//		if (state == Initialise) {
//			std::cout << "Bypass Door\n";
//			Debug::Print("Bypass Door!", Goose->GetTransform().GetPosition());
//			state == Ongoing;
//		}
//		if (state == Ongoing) {
//			Goose->GetTransform().SetPosition(Vector3(0, -17, 80));
//			std::cout << "Bypass Door!\n";
//			return Success;
//		}
//		return state;
//		});
//	BehaviourAction* speak = new BehaviourAction("speak", [&](float dt, BehaviourState state)->BehaviourState {
//		if (state == Initialise) {
//			Debug::Print("I will catch you!", Goose->GetTransform().GetPosition());
//			std::cout << "I will catch you!\n";
//			return Ongoing;
//		}
//		else if (state == Ongoing) {
//			bool found = rand() % 2;
//			if (found) {
//				std::cout << "I will catch you!\n";
//				Debug::Print("I will catch you!", Goose->GetTransform().GetPosition());
//				return Failure;
//			}
//		}
//		return state;
//		});
//	BehaviourAction* chase = new BehaviourAction("chase player", [&](float dt, BehaviourState state)->BehaviourState {
//		if (state == Initialise) {
//			std::cout << "Chasing player!\n";
//			Debug::Print("Chasing player!", Goose->GetTransform().GetPosition());
//			return Ongoing;
//		}
//		else if (state == Ongoing) {
//			GoosePathfinding();
//			while (physics->IsGoosePlayer()) {
//				std::cout << "Catch you!\n";
//				return Failure;
//			}
//		}
//		return state;
//		});
//	BehaviourSequence* sequence = new BehaviourSequence("Room Sequence");
//	sequence->AddChild(sleep);
//	sequence->AddChild(goToDoor);
//	sequence->AddChild(bypassDoor);
//	BehaviourSelector* selection = new BehaviourSelector("Loot Selection");
//	selection->AddChild(speak);
//	selection->AddChild(chase);
//	rootSequence = new BehaviourSequence("Root Sequence");
//	rootSequence->AddChild(sequence);
//	rootSequence->AddChild(selection);
//	//for (int i = 0; i < 5; ++i) {
//	rootSequence->Reset();
//	state = Ongoing;
//	/*behaviourTimer = 0.0f;
//	distanceTotarget = rand() % 250;*/
//	//BehaviourState state = Ongoing;
//	//std::cout << "We are going on an adventure!\n";
//	//if (state == Ongoing) {
//	//	state = rootSequence->Execute(1.0f);//fake dt
//	//}
///*	if (state == Failure) {
//		std::cout << "What a waste of time!\n";
//	}*/
//	//	}
//		//std::cout << "All done!\n";
//}