#include "Window.h"

#include "Debug.h"

#include "StateMachine.h"
#include "StateTransition.h"
#include "State.h"

#include "GameServer.h"
#include "GameClient.h"

#include "NavigationGrid.h"
#include "NavigationMesh.h"

#include "TutorialGame.h"
#include "NetworkedGame.h"

#include "PushdownMachine.h"

#include "PushdownState.h"

#include "BehaviourNode.h"
#include "BehaviourSelector.h"
#include "BehaviourSequence.h"
#include "BehaviourAction.h"

using namespace NCL;
using namespace CSC8503;

#include <chrono>
#include <thread>
#include <sstream>

//void TestBehaviourTree() {
//	float behaviourTimer;
//	float distanceToTarget;
//	BehaviourAction* findKey = new BehaviourAction("Find Key",
//		[&](float dt, BehaviourState state)->BehaviourState {
//			if (state == Initialise) {
//				std::cout << "Looking for a key!\n";
//				behaviourTimer = rand() % 100;
//				state = Ongoing;
//				
//			}
//			else if (state == Ongoing) {
//				behaviourTimer -= dt;
//				if (behaviourTimer <= 0.0f) {
//					std::cout << "Found a key!\n";
//					return Success;
//				}
//			}
//			return state;
//		}
//	);
//	BehaviourAction* goToRoom = new BehaviourAction("Go To Room",
//		[&](float dt, BehaviourState state)->BehaviourState {
//			if (state == Initialise) {
//				std::cout << "Going to the loot room!\n";
//				state = Ongoing;
//			}
//			else if (state == Ongoing) {
//				distanceToTarget -= dt;
//				if (distanceToTarget <= 0.0f) {
//					std::cout << "Reached room!\n";
//					return Success;
//				}
//			}
//			return state;
//		}
//	);
//	BehaviourAction* openDoor = new BehaviourAction("Open Door",
//		[&](float dt, BehaviourState state)->BehaviourState {
//			if (state == Initialise) {
//				std::cout << "Opening Door!\n";
//				return Success;
//			}
//			return state;
//		}
//	);
//	BehaviourAction* lookForTreasure = new BehaviourAction("Look For Treasure",
//		[&](float dt, BehaviourState state)->BehaviourState {
//			if (state == Initialise) {
//				std::cout << "Looking for treasure!\n";
//				return Ongoing;
//			}
//			else if (state == Ongoing) {
//				bool found = rand() % 2;
//				if (found) {
//					std::cout << "I found some treasure!\n";
//					return Success;
//				}
//				std::cout << "No treasure in here...\n";
//				return Failure;
//			}
//			return state;
//		}
//	);
//	BehaviourAction* lookForItems = new BehaviourAction("Look for Items",
//		[&](float dt, BehaviourState state)->BehaviourState {
//			if (state == Initialise) {
//				std::cout << "Looking for items!\n";
//				return Ongoing;
//			}
//			else if (state == Ongoing) {
//				bool found = rand() % 2;
//				if (found) {
//					std::cout << "I found some items!\n";
//					return Success;
//				}
//				std::cout << "No items in here...\n";
//				return Failure;
//			}
//			return state;
//		}
//	);
//	BehaviourSequence* sequence = new BehaviourSequence("Room Sequence");
//	sequence->AddChild(findKey);
//	sequence->AddChild(goToRoom);
//	sequence->AddChild(openDoor);
//
//	BehaviourSelector* selection = new BehaviourSelector("Loot Selection");
//	selection->AddChild(lookForTreasure);
//	selection->AddChild(lookForItems);
//
//	BehaviourSequence* rootSequence = new BehaviourSequence("Root Sequence");
//	rootSequence->AddChild(sequence);
//	rootSequence->AddChild(selection);
//
//	for (int i = 0; i < 5; i++) {
//		rootSequence->Reset();
//		behaviourTimer = 0.0f;
//		distanceToTarget = rand() % 250;
//		BehaviourState state = Ongoing;
//		std::cout << "We are going on an adventure!\n";
//		while (state == Ongoing) {
//			state = rootSequence->Execute(1.0f);
//		}
//		if (state == Success) {
//			std::cout << "What a successful adventure!\n";
//		}
//		else if (state == Failure) {
//			std::cout << "What a waste of time!\n";
//		}
//	}
//	std::cout << "All done!\n";
//}

//class PauseScreen : public PushdownState {
//	PushdownResult OnUpdate(float dt, PushdownState** newState) override {
//		if (Window::GetKeyboard()->KeyPressed(KeyCodes::U)) {
//			return PushdownResult::Pop;
//		}
//		return PushdownResult::NoChange;
//	}
//	void OnAwake() override {
//		std::cout << "Press U to unpause game!\n";
//	}
//};
//
//class GameScreen : public PushdownState {
//	PushdownResult OnUpdate(float dt, PushdownState** newState) override {
//		pauseReminder -= dt;
//		if (pauseReminder < 0) {
//			std::cout << "Coins mined: " << coinsMined << std::endl;
//			std::cout << "Press P to pause game, or M to return main menu!\n";
//			pauseReminder += 1.0f;
//		}
//		if (Window::GetKeyboard()->KeyDown(KeyCodes::P)) {
//			*newState = new PauseScreen();
//			return PushdownResult::Push;
//		}
//		if (Window::GetKeyboard()->KeyDown(KeyCodes::M)) {
//			std::cout << "Returning to main menu!\n";
//			return PushdownResult::Pop;
//		}
//		if (rand() % 7 == 0) {
//			coinsMined++;
//		}
//		return PushdownResult::NoChange;
//	};
//	void OnAwake() override {
//		std::cout << "Preparing to mine coins!\n";
//	};
//protected:
//	int coinsMined = 0;
//	float pauseReminder = 1;
//};
//
//class IntroScreen : public PushdownState {
//	PushdownResult OnUpdate(float dt, PushdownState** newState) override {
//		if (Window::GetKeyboard()->KeyPressed(KeyCodes::SPACE)) {
//			*newState = new GameScreen();
//			return PushdownResult::Push;
//		}
//		if (Window::GetKeyboard()->KeyPressed(KeyCodes::ESCAPE)) {
//			return PushdownResult::Pop;
//		}
//		return PushdownResult::NoChange;
//	};
//	void OnAwake() override {
//		std::cout << "Welcome to a really awesome game!\n";
//		std::cout << "Press SPACE to begin or ESCAPE to quit!\n";
//		Debug::Print("Welcome to a really awesome game!", Vector2(50, 50), Vector4(1, 0, 0, 1));
//	}
//};
//
//void TestPushdownAutomata(Window* w) {
//	PushdownMachine machine(new IntroScreen());
//	while (w->UpdateWindow()) {
//		float dt = w->GetTimer().GetTimeDeltaSeconds();
//		if (!machine.Update(dt)) {
//			return;
//		}
//	}
//}

/*

The main function should look pretty familar to you!
We make a window, and then go into a while loop that repeatedly
runs our 'game' until we press escape. Instead of making a 'renderer'
and updating it, we instead make a whole game, and repeatedly update that,
instead. 

This time, we've added some extra functionality to the window class - we can
hide or show the 

*/
int main() {
	Window*w = Window::CreateGameWindow("CSC8503 Game technology!", 1280, 720);

	if (!w->HasInitialised()) {
		return -1;
	}	
	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);

	TutorialGame* g = new TutorialGame();
	w->GetTimer().GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!

	//TestBehaviourTree();
	//TestPushdownAutomata(w); 

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyCodes::ESCAPE)) {
		float dt = w->GetTimer().GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		if (Window::GetKeyboard()->KeyPressed(KeyCodes::PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyCodes::NEXT)) {
			w->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KeyCodes::T)) {
			w->SetWindowPosition(0, 0);
		}

		w->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));

		switch (g->GetState())
		{
		case NCL::CSC8503::MainMenu:
			g->ShowMainMenu(dt);
			break;
		case NCL::CSC8503::Ranking:
			g->ShowRanking(dt);
			break;
		case NCL::CSC8503::ModeSelect:
			g->SelectGameMode(dt);
			break;
		case NCL::CSC8503::Start:
			g->InitGame();
			break;
		case NCL::CSC8503::OnGoing:
			g->UpdateGame(dt);
			break;
		case NCL::CSC8503::Pause:
			g->ShowPause(dt);
			break;
		case NCL::CSC8503::Win:
			g->ShowWin(dt);
			break;
		case NCL::CSC8503::Lose:
			g->ShowLose(dt);
			break;
		default:
			break;
		}

		if (g->GetState() == Exit) break;

	}
	Window::DestroyGameWindow();
}