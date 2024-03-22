#pragma once
#include "TutorialGame.h"
#include "NetworkBase.h"
#include "PushdownState.h"


namespace NCL {
	namespace CSC8503 {
		class GameServer;
		class GameClient;
		class NetworkPlayer;
		class PushdownMachine;
		class DeltaPacket;
		class FullPacket;
		class ClientPacket;
		class RoundStatePacket;
		class PlayerStatePacket;
		class BulletStatePacket;
		class Item;

		enum PlayInputBtns {
			Up,
			Down,
			Right,
			Left,
			Sprint,
			Fire
		};


		class NetworkedGame : public TutorialGame, public PacketReceiver {
		public:
			NetworkedGame();
			~NetworkedGame();

			bool StartAsServer();
			bool StartAsClient(char a, char b, char c, char d);

			void UpdateGame(float dt);

			void SpawnPlayer();

			void StartLevel();

			void LevelOver();

			void ReceivePacket(int type, GamePacket* payload, int source) override;

			void OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b);

			bool isServer() { return thisServer != nullptr; }
			bool isClient() { return thisClient != nullptr; }

			int GetPlayerPeerID(int num) { return PlayersList[num]; }
			int GetClientPlayerNum();
			int GetClientPlayerNum(int peerID);
			void ServerUpdatePlayerList();

			bool isRoundStart() { return isRoundstart; }
			bool isGameOver() { return isGameover; }

			
		protected:

			//void InitialiseAssets() override;
			void UpdateAsServer(float dt);
			void UpdateAsClient(float dt);

			void BroadcastSnapshot(bool deltaFrame);
			void UpdateMinimumState();
			GameObject* AddNetPlayerToWorld(const Vector3& position, int playerNum);

			bool clientProcessFp(FullPacket* fp);
			bool clientProcessDp(DeltaPacket* dp);

			bool serverProcessCP(ClientPacket* cp, int source);

			bool clientProcessRp(RoundStatePacket* rp);

			void UpdateGamePlayerInput(float dt);

			void ServerSendRoundState();

			void ServerSendPlayerState();

			std::map<int, int> stateIDs;

			int GlobalStateID;

			bool isRoundstart;
			bool isGameover;

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;

			std::map<int, NetworkObject*> networkObjects;

			std::vector<int> PlayersList;
			std::vector<GameObject*> serverPlayers;
			GameObject* localPlayer;
			PushdownMachine* MenuSystem;
		};

		class InGame : public PushdownState
		{
			PushdownResult OnUpdate(float dt, PushdownState** newState) override
			{
				if (game)
				{
					NetworkedGame* thisGame = (NetworkedGame*)game;
					if (thisGame->isRoundStart()) { return RoundOnGoing(dt, thisGame); }
					else { return RoundOver(dt, newState, thisGame); }
					return RoundOnGoing(dt, thisGame);
				}
			}

		protected:
			PushdownResult RoundOver(float dt, PushdownState** newState, NetworkedGame* thisGame)
			{
				overDisplayTime -= dt;
				if (overDisplayTime > 0) { Debug::Print("Round Over!", Vector2(38, 35), Debug::RED); }
				else
				{
					
					Debug::Print("====================================", Vector2(15, 30), Debug::YELLOW);
					for (int i = 1; i < 5; ++i)
					{
						
					}
					Debug::Print("====================================", Vector2(15, 80), Debug::YELLOW);
					Debug::Print("Press Esc : Return to Multiplayer Lobby", Vector2(12, 90), Debug::YELLOW);
					if (Window::GetKeyboard()->KeyPressed(KeyCodes::ESCAPE)) { return PushdownResult::Pop; }
				}
				return PushdownResult::NoChange;
			}

			PushdownResult RoundOnGoing(float dt, NetworkedGame* thisGame)
			{
				startDisplayTime -= dt;
				if (startDisplayTime > 0) { Debug::Print("Round Start!", Vector2(38, 35), Debug::RED); }
				blinkTime -= dt;
				if (blinkTime < 0) {
					displayTreasureSign = !displayTreasureSign;
					blinkTime = 0.6f;
				}

				

				if (thisGame->isServer())
				{
					Debug::Print("You:Player 1", Vector2(5, 10), Debug::RED);
					
				}
				if (thisGame->isClient())
				{
					switch (thisGame->GetClientPlayerNum())
					{
					case 1:
						Debug::Print("You:Player 2", Vector2(5, 10), Debug::BLUE);
						break;
					case 2:
						Debug::Print("You:Player 3", Vector2(5, 10), Debug::YELLOW);
						break;
					case 3:
						Debug::Print("You:Player 4", Vector2(5, 10), Debug::CYAN);
						break;
					}
				}
				return PushdownResult::NoChange;
			}

			float startDisplayTime = 1.2f;
			float overDisplayTime = 1.2f;
			float blinkTime = 0.6f;
			bool displayTreasureSign = true;
		};

		class MultiplayerLobby : public PushdownState
		{
			PushdownResult OnUpdate(float dt, PushdownState** newState) override
			{
				Debug::Print("MultiPlayer Lobby", Vector2(33, 10), Debug::YELLOW);
				Debug::Print("================================================", Vector2(5, 20), Debug::YELLOW);
				Debug::Print("================================================", Vector2(5, 70), Debug::YELLOW);
				Debug::Print("Press Esc : MultiPlayer Menu", Vector2(5, 90), Debug::YELLOW);
				if (game)
				{
					NetworkedGame* thisGame = (NetworkedGame*)game;
					if (thisGame->isRoundStart())
					{
						*newState = new InGame();
						return PushdownResult::Push;
					}
					if (thisGame->isServer())
					{
						Debug::Print("is You", Vector2(40, 30), Debug::RED);
						Debug::Print("Press S : Game Start!", Vector2(5, 80), Debug::YELLOW);
						if (Window::GetKeyboard()->KeyPressed(KeyCodes::S))
						{
							thisGame->StartLevel();
						}
					}
					if (thisGame->isClient())
					{
						int num = thisGame->GetClientPlayerNum();
						Debug::Print("is You", Vector2(40, 30 + num * 10), Debug::RED);
						Debug::Print("Wait for Server to start game....", Vector2(5, 80), Debug::YELLOW);
					}
					if (thisGame->GetPlayerPeerID(0) != -1)
					{
						Debug::Print("Player 1", Vector2(5, 30), Debug::RED);
					}
					if (thisGame->GetPlayerPeerID(1) != -1)
					{
						Debug::Print("Player 2", Vector2(5, 40), Debug::BLUE);
					}
					if (thisGame->GetPlayerPeerID(2) != -1)
					{
						Debug::Print("Player 3", Vector2(5, 50), Debug::YELLOW);
					}
					if (thisGame->GetPlayerPeerID(3) != -1)
					{
						Debug::Print("Player 4", Vector2(5, 60), Debug::CYAN);
					}
				}
				if (Window::GetKeyboard()->KeyPressed(KeyCodes::ESCAPE))
				{
					return PushdownResult::Pop;
				}
				return PushdownResult::NoChange;
			}
		};

		class MultiplayerMenu : public PushdownState
		{
			PushdownResult OnUpdate(float dt, PushdownState** newState) override
			{
				Debug::Print("Press NUMPAD1 : Start As Server", Vector2(5, 10), Debug::YELLOW);
				Debug::Print("Press NUMPAD2 : Start As Client", Vector2(5, 20), Debug::YELLOW);
				Debug::Print("Press P : Pause Game", Vector2(5, 95), Debug::YELLOW);

				if (game)
				{
					NetworkedGame* thisGame = (NetworkedGame*)game;
					if (Window::GetKeyboard()->KeyPressed(KeyCodes::NUMPAD1))
					{
						thisGame->StartAsServer();
						*newState = new MultiplayerLobby();
						return PushdownResult::Push;
					}
					if (Window::GetKeyboard()->KeyPressed(KeyCodes::NUMPAD2))
					{
						if (thisGame->StartAsClient(127, 0, 0, 1))
						{
							*newState = new MultiplayerLobby();
							return PushdownResult::Push;
						}
						Debug::Print("Failed to connect the server!!!", Vector2(50, 30), Debug::RED);
					}
				}
				if (Window::GetKeyboard()->KeyPressed(KeyCodes::ESCAPE))
				{
					return PushdownResult::Pop;
				}
				return PushdownResult::NoChange;
			}
		};

		class HintMenu : public PushdownState
		{
			PushdownResult OnUpdate(float dt, PushdownState** newState) override
			{
				Debug::Print("Mouse Move for Rotate Player", Vector2(5, 10), Debug::YELLOW);
				Debug::Print("Mouse Left Btn for Player Fire", Vector2(5, 17), Debug::YELLOW);
				Debug::Print("W for Move Up", Vector2(5, 24), Debug::YELLOW);
				Debug::Print("S for Move Down", Vector2(5, 31), Debug::YELLOW);
				Debug::Print("A for Move Left", Vector2(5, 38), Debug::YELLOW);
				Debug::Print("D for Move Right", Vector2(5, 45), Debug::YELLOW);
				Debug::Print("Shift for Sprint Forward", Vector2(5, 52), Debug::YELLOW);


				Debug::Print("Press Esc : Back to Main menu ", Vector2(5, 80), Debug::YELLOW);
				if (Window::GetKeyboard()->KeyPressed(KeyCodes::ESCAPE))
				{
					return PushdownResult::Pop;
				}
				return PushdownResult::NoChange;
			}

		};

	
	}
}

