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
		class NetworkedGame : public TutorialGame, public PacketReceiver {
		public:
			NetworkedGame();
			~NetworkedGame();

			void StartAsServer();
			void StartAsClient(char a, char b, char c, char d);

			void UpdateGame(float dt) override;

			void SpawnPlayer();

			void StartLevel();

			void ReceivePacket(int type, GamePacket* payload, int source) override;

			void OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b);

		protected:
			void UpdateAsServer(float dt);
			void UpdateAsClient(float dt);

			void BroadcastSnapshot(bool deltaFrame);
			void UpdateMinimumState();
			std::map<int, int> stateIDs;

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;

			std::vector<NetworkObject*> networkObjects;

			std::map<int, GameObject*> serverPlayers;
			GameObject* localPlayer;
			PushdownMachine* MenuSystem;
		};

		class MultiplayerMenu : public PushdownState
		{
			PushdownResult OnUpdate(float dt, PushdownState** newState) override
			{
				Debug::Print("Press 1 : Start As Server", Vector2(5, 10), Debug::YELLOW);
				Debug::Print("Press 2 : Start As Client", Vector2(5, 20), Debug::YELLOW);
				Debug::Print("Press Esc : Main Menu", Vector2(5, 70), Debug::YELLOW);

				if (game)
				{
					NetworkedGame* thisGame = (NetworkedGame*)game;
					if (Window::GetKeyboard()->KeyPressed(KeyCodes::NUM1))
					{
						thisGame->StartAsServer();
						//*newState = new MultiplayerLobby();
						return PushdownResult::Push;
					}
					if (Window::GetKeyboard()->KeyPressed(KeyCodes::NUM2))
					{
						//if (thisGame->StartAsClient(127, 0, 0, 1))
						{
							thisGame->StartAsClient(127, 0, 0, 1);
							//*newState = new MultiplayerLobby();
							
						}
						return PushdownResult::Push;
						//Debug::Print("Failed to connect the server!!!", Vector2(50, 30), Debug::RED);
					}
				}
				if (Window::GetKeyboard()->KeyPressed(KeyCodes::ESCAPE))
				{
					return PushdownResult::Pop;
				}
				return PushdownResult::NoChange;
			}
		};
	}
}

