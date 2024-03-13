#include "GameServer.h"
#include "GameWorld.h"
#include "./enet/enet.h"
using namespace NCL;
using namespace CSC8503;

GameServer::GameServer(int onPort, int maxClients)	{
	port		= onPort;
	clientMax	= maxClients;
	clientCount = 0;
	netHandle	= nullptr;
	netPeers = new int[maxClients];
	clearPeerArray();
	Initialise();
}

GameServer::~GameServer()	{
	Shutdown();
}

void GameServer::Shutdown() {
	SendGlobalPacket(BasicNetworkMessages::Shutdown);
	enet_host_destroy(netHandle);
	netHandle = nullptr;
}

bool GameServer::Initialise() {
	ENetAddress address;
	 address.host = ENET_HOST_ANY;
	 address.port = port;
	netHandle = enet_host_create(&address, clientMax, 1, 0, 0);
		if (!netHandle) {
		std::cout << __FUNCTION__ <<
		 " failed to create network handle ! " << std::endl;
		 return false;	
	}
		 return true;
}


bool GameServer::SendGlobalPacket(int msgID) {
	GamePacket packet;
	 packet.type = msgID;
	 return SendGlobalPacket(packet);	
}

 bool GameServer::SendGlobalPacket(GamePacket& packet) {
	 ENetPacket* dataPacket = enet_packet_create(&packet,
	 packet.GetTotalSize(), 0);
	enet_host_broadcast(netHandle, 0, dataPacket);
	 return true;	
}

 bool GameServer::SendPacketToPeer(int sendAdd, GamePacket& payload) {
	 ENetPacket* dataPacket = enet_packet_create(&payload,
		 payload.GetTotalSize(), 0);

	 return true;
	// enet_peer_send(netPeer, 0, dataPacket);
 }

void GameServer::UpdateServer() {
	if (!netHandle) { return; }
	 ENetEvent event;
	 while (enet_host_service(netHandle, &event, 0) > 0) {
		 int type = event.type;
		 ENetPeer * p = event.peer;
		 int peer = p -> incomingPeerID;
		
			if (type == ENetEventType::ENET_EVENT_TYPE_CONNECT) {
			 std::cout << " Server : New client connected " << std::endl;
			 std::cout << " client add : New client connected " <<peer << std::endl;
			AddPeer(peer);
		}
		 else if (type == ENetEventType::ENET_EVENT_TYPE_DISCONNECT) {
			 std::cout << " Server : A client has disconnected " << std::endl;
			 std::cout << " client removed : New client connected " << peer << std::endl;
			 RemovePeer(peer);
		}
		 else if (type == ENetEventType::ENET_EVENT_TYPE_RECEIVE) {
			 GamePacket * packet = (GamePacket*)event.packet -> data;
			 ProcessPacket(packet, peer);
			
		}
		 enet_packet_destroy(event.packet);
		
	}
}
void GameServer::clearPeerArray()
{
	for (int i = 0; i < clientMax; ++i)
	{
		netPeers[i] = -1;
	}
}

void GameServer::RemovePeer(int Peer)
{
	for (int i = 0; i < clientMax; ++i)
	{
		if (netPeers[i] == Peer)
		{
			netPeers[i] = -1;
			--clientCount;
			return;
		}
	}
}

void GameServer::DebugNetPeer()
{
	for (int i = 0; i < clientMax; ++i)
	{
		std::cout << i << " : PeerID: " << netPeers[i] << std::endl;;
	}
}

bool GameServer::GetMultiNetPeer(int peerNum, int& peerID)
{
	if (peerNum >= clientMax) { return false; }
	if (netPeers[peerNum] == -1) { return false; }
	peerID = netPeers[peerNum];
	return true;
}

void GameServer::AddPeer(int Peer)
{
	int minVacantPos = clientMax;
	for (int i = 0; i < clientMax; ++i)
	{
		if (netPeers[i] == Peer)
		{
			return;
		}
		if (netPeers[i] == -1)
		{
			minVacantPos = std::min(minVacantPos, i);
		}
	}
	if (minVacantPos < clientMax)
	{
		netPeers[minVacantPos] = Peer;
	}
}
bool GameServer::GetNetPeer(int peerNum, int& peerID)
{
	if (peerNum >= clientMax) { return false; }
	if (netPeers[peerNum] == -1) { return false; }
	peerID = netPeers[peerNum];
	return true;
}

void GameServer::SetGameWorld(GameWorld &g) {
	gameWorld = &g;
}