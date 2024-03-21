#pragma once
#include "NetworkBase.h"
#include <list>

namespace NCL {
	namespace CSC8503 {
		class GameWorld;
		class GameServer : public NetworkBase {
		public:
			GameServer(int onPort, int maxClients);
			~GameServer();

			bool Initialise();
			void Shutdown();

			void SetGameWorld(GameWorld &g);

			bool SendGlobalPacket(int msgID);
			bool SendGlobalPacket(GamePacket& packet);
			bool SendPacketToPeer(int playerNum, GamePacket& packet);
			bool GetMultiNetPeer(int peerNum, int& peerID);
			bool GetNetPeer(int peerNum, int& peerID);
			virtual void UpdateServer();

		protected:
			int			port;
			int			clientMax;
			int			clientCount;
			GameWorld*	gameWorld;

			int incomingDataRate;
			int outgoingDataRate;

			//_ENetPeer* netPeer;
			//std::list<int> clientAddList;
			void clearPeerArray();
			void AddPeer(int val);
			void RemovePeer(int val);

			//void DeletPeer(int Peer);

			void DebugNetPeer();

			int* netPeers;
		};
	}
}
