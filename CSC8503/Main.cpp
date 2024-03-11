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


class TestPacketReceiver : public PacketReceiver {
public:
	TestPacketReceiver(string name) {
		this->name = name;
	}
	void ReceivePacket(int type, GamePacket* payload, int source) {
		if (type == String_Message) {
			StringPacket* realPacket = (StringPacket*)payload;
			string msg = realPacket->GetStringFromData();
			std::cout << name << " received message : " << msg << std::endl;
		}
	}

protected:
	string name;
};

void TestNetworking() {
	NetworkBase::Initialise();

	TestPacketReceiver serverReceiver(" Server ");
	TestPacketReceiver clientAReceiver(" Client A");
	TestPacketReceiver clientBReceiver(" Client B");

	int port = NetworkBase::GetDefaultPort();

	GameServer* server = new GameServer(port, 3);
	GameClient* clientA = new GameClient();
	GameClient* clientB = new GameClient();

	server->RegisterPacketHandler(String_Message, &serverReceiver);
	clientA->RegisterPacketHandler(String_Message, &clientAReceiver);
	clientB->RegisterPacketHandler(String_Message, &clientBReceiver);

	bool canConnectA = clientA->Connect(127, 0, 0, 1, port);
	bool canConnectB = clientB->Connect(127, 0, 0, 1, port);

	for (int i = 0; i < 100; ++i) {
		/*StringPacket* a = new StringPacket(" Server says hello ! " + std::to_string(i));
		server->SendGlobalPacket(*a);

		StringPacket*b = new StringPacket(" Client A says hello ! " + std::to_string(i));
		StringPacket*c = new StringPacket(" Client B says hello ! " + std::to_string(i));
		clientA->SendPacket(*b);
		clientB->SendPacket(*c);*/

		server->UpdateServer();
		clientA->UpdateClient();
		clientB->UpdateClient();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	}
	NetworkBase::Destroy();
}

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
	Window* w = Window::CreateGameWindow("CSC8503 Game technology!", 1280, 720);

	if (!w->HasInitialised()) {
		return -1;
	}
	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);
	
	TutorialGame* g = new TutorialGame();

	//NetworkedGame* h = new NetworkedGame();
	w->GetTimer().GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!

	//sound
	//ISoundEngine* engine = createIrrKlangDevice();
	//if (!engine) {
	//	return 0;
	//}
	//engine->play2D("../externals/media/getout.ogg", true);

	//TestNetworking();
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
		//h->UpdateGame(dt);
	}
	Window::DestroyGameWindow();
}