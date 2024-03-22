#include "GameServer.h"
#include "NetworkedGame.h"
#include "NetworkPlayer.h"
#include "NetworkObject.h"
#include "GameServer.h"
#include "GameClient.h"
#include "GameWorld.h"
#include "PushdownMachine.h"
#include "RenderObject.h"
#include "PhysicsObject.h"


#define COLLISION_MSG 30

struct MessagePacket : public GamePacket {
	short playerID;
	short messageID;

	MessagePacket() {
		type = Message;
		size = sizeof(short) * 2;
	}
};

NetworkedGame::NetworkedGame()	{

	thisServer = nullptr;
	thisClient = nullptr;

	NetworkBase::Initialise();
	timeToNextPacket  = 0.0f;
	packetsToSnapshot = 0;


	MenuSystem = new PushdownMachine(new MultiplayerMenu());
	MenuSystem->SetGame(this);
	PlayersList.clear();
	for (int i = 0; i < 4; ++i)
	{
		PlayersList.push_back(-1);
	}
	//InitialiseAssets();
	isRoundstart = false;
}

NetworkedGame::~NetworkedGame()	{
	delete thisServer;
	delete thisClient;
	delete MenuSystem;
}

bool NetworkedGame::StartAsServer() {
	if (thisServer != nullptr)
	{
		return true;
	}
	thisServer = new GameServer(NetworkBase::GetDefaultPort(), 3);

	thisServer->RegisterPacketHandler(Received_State, this);
	return true;
}

bool NetworkedGame::StartAsClient(char a, char b, char c, char d) {
	if (thisClient != nullptr)
	{
		return true;
	}
	thisClient = new GameClient();
	thisClient->Connect(a, b, c, d, NetworkBase::GetDefaultPort());

	thisClient->RegisterPacketHandler(Player_Connected, this);
	thisClient->RegisterPacketHandler(Player_Disconnected, this);
	thisClient->RegisterPacketHandler(Delta_State, this);
	thisClient->RegisterPacketHandler(Full_State, this);
	thisClient->RegisterPacketHandler(Message, this);
	thisClient->RegisterPacketHandler(Round_State, this);

	//StartLevel();
	return true;
}

void NetworkedGame::UpdateGame(float dt) {
	if (!MenuSystem->Update(dt))
	{
		isGameover = true;
	}
	timeToNextPacket -= dt;
	if (timeToNextPacket < 0) {
		if (isRoundstart)
		{
			if (thisServer) {
				UpdateAsServer(dt);
			}
			else if (thisClient) {
				UpdateAsClient(dt);
			}
			
		}
		timeToNextPacket += 1.0f / 60.0f; //20hz server/client update
	}

	if (thisServer) {
		thisServer->UpdateServer();
		ServerUpdatePlayerList();
		ServerSendRoundState();
		if(isRoundStart())
			UpdateGamePlayerInput(dt);
		physics->Update(dt);
		if (isRoundStart()) {
			//SpawnPlayer();
		}
	}
	else if (thisClient) {
		thisClient->UpdateClient();
		if (isRoundStart()) {
			//SpawnPlayer();
		}
	}

	
	if (isRoundstart)
	{
		
	}
	

	TutorialGame::UpdateGame(dt);
}

void NetworkedGame::UpdateAsServer(float dt) {
	packetsToSnapshot--;
	if (packetsToSnapshot < 0) {
		BroadcastSnapshot(false);
		packetsToSnapshot = 5;
	}
	else {
		BroadcastSnapshot(true);
	}
}

void NetworkedGame::UpdateAsClient(float dt) {
	ClientPacket newPacket;

	
	
	newPacket.forceToBeAdded = player->forceToBeAdded;
	newPacket.orientationNetPlayer = player->orientationNetPlayer;
	newPacket.btnStates[0] = player->IsJumping();
	
	newPacket.lastID = GlobalStateID;

	
	thisClient->SendPacket(newPacket);
}

void NetworkedGame::BroadcastSnapshot(bool deltaFrame) {
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;

	world->GetObjectIterators(first, last);

	for (auto i = first; i != last; ++i) {
		NetworkObject* o = (*i)->GetNetworkObject();
		if (!o) {
			continue;
		}
		//TODO - you'll need some way of determining
		//when a player has sent the server an acknowledgement
		//and store the lastID somewhere. A map between player
		//and an int could work, or it could be part of a 
		//NetworkPlayer struct. 
		int playerState = 0;
		GamePacket* newPacket = nullptr;
		if (o->WritePacket(&newPacket, deltaFrame, playerState)) {
			thisServer->SendGlobalPacket(*newPacket);
			delete newPacket;
		}
	}
}

void NetworkedGame::UpdateMinimumState() {
	//Periodically remove old data from the server
	int minID = INT_MAX;
	int maxID = 0; //we could use this to see if a player is lagging behind?

	for (auto i : stateIDs) {
		minID = std::min(minID, i.second);
		maxID = std::max(maxID, i.second);
	}
	//every client has acknowledged reaching at least state minID
	//so we can get rid of any old states!
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;
	world->GetObjectIterators(first, last);

	for (auto i = first; i != last; ++i) {
		NetworkObject* o = (*i)->GetNetworkObject();
		if (!o) {
			continue;
		}
		o->UpdateStateHistory(minID); //clear out old states so they arent taking up memory...
	}
}

void NetworkedGame::SpawnPlayer() {
	serverPlayers.clear();
	for (int i = 0; i < 4; ++i)
	{
		if (GetPlayerPeerID(i) != -1)
		{
			//if (serverPlayers[i] == nullptr) {
				Vector3 pos;
				switch (i)
				{
				case 0:
					pos = Vector3(0, 10, 30);
					break;
				case 1:
					pos = Vector3(2, 10, 30);
					break;
				case 2:
					pos = Vector3(0, 10, 32);
					break;
				case 3:
					pos = Vector3(2, 10, 32);
					break;
				}
				serverPlayers.push_back(AddNetPlayerToWorld(pos, i));
			//}
		}
	}
	if (isServer())
	{
		if(player != serverPlayers[0])
			player = dynamic_cast<Player*>( serverPlayers[0]);
	}
	else if (isClient())
	{
		if (player != serverPlayers[GetClientPlayerNum()])
			player = dynamic_cast<Player*>(serverPlayers[GetClientPlayerNum()]);
	}
	LockCameraToObject(player);
}

void NetworkedGame::StartLevel() {
	
	GlobalStateID = -1;
	SpawnPlayer();
	isRoundstart = true;
	
}

void NetworkedGame::LevelOver()
{
	isRoundstart = false;
}

void NetworkedGame::ReceivePacket(int type, GamePacket* payload, int source) {
	switch (type)
	{
	case BasicNetworkMessages::Message: {
		PLayerListPacket* realPacket = (PLayerListPacket*)payload;
		realPacket->GetPlayerList(PlayersList);
		break;
	}
	case BasicNetworkMessages::Round_State: {
		RoundStatePacket* realPacket = (RoundStatePacket*)payload;
		clientProcessRp(realPacket);
		break;
	}
	case BasicNetworkMessages::Full_State: {
		FullPacket* realPacket = (FullPacket*)payload;
		clientProcessFp(realPacket);
		break;
	}
	case BasicNetworkMessages::Delta_State: {
		DeltaPacket* realPacket = (DeltaPacket*)payload;
		clientProcessDp(realPacket);
		break;
	}
	case BasicNetworkMessages::Received_State: {
		ClientPacket* realPacket = (ClientPacket*)payload;
		serverProcessCP(realPacket, source);
		break;
	}
	
	}
}

void NetworkedGame::OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b) {
	if (thisServer) { //detected a collision between players!
		MessagePacket newPacket;
		newPacket.messageID = COLLISION_MSG;
		newPacket.playerID  = a->GetPlayerNum();

		thisClient->SendPacket(newPacket);

		newPacket.playerID = b->GetPlayerNum();
		thisClient->SendPacket(newPacket);
	}
}

int NetworkedGame::GetClientPlayerNum()
{
	if (thisClient)
	{
		for (int i = 1; i < 4; ++i)
		{
			if (PlayersList[i] == thisClient->GetPeerID())
			{
				return i;
			}
		}
	}
	return -1;
}

int NetworkedGame::GetClientPlayerNum(int peerID)
{
	for (int i = 1; i < 4; ++i)
	{
		if (PlayersList[i] == peerID)
		{
			return i;
		}
	}
	return -1;
}
void NetworkedGame::ServerUpdatePlayerList()
{
	if (thisServer == nullptr)
	{
		return;
	}
	PlayersList[0] = 0;
	int peerID;
	for (int i = 0; i < 3; ++i)
	{
		if (thisServer->GetNetPeer(i, peerID))
		{
			PlayersList[i + 1] = peerID;
		}
		else
		{
			PlayersList[i + 1] = -1;
		}
	}
	PLayerListPacket plist(PlayersList);
	thisServer->SendGlobalPacket(plist);
}



GameObject* NetworkedGame::AddNetPlayerToWorld(const Vector3& position, int playerNum)
{
	GameObject* thisplayer = playerlist[playerNum];
	

	//character->SetRenderObject(new RenderObject(&character->GetTransform(), charMesh, nullptr, basicShader));
	//character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
	thisplayer->SetNetworkObject(new NetworkObject(*thisplayer, playerNum));
	networkObjects.insert(std::pair<int, NetworkObject*>(playerNum, thisplayer->GetNetworkObject()));
	//character->GetPhysicsObject()->SetInverseMass(inverseMass);
	//character->GetPhysicsObject()->InitCubeInertia();


	world->AddGameObject(thisplayer);
	return thisplayer;
	
}

bool NetworkedGame::clientProcessFp(FullPacket* fp)
{
	auto itr = networkObjects.find(fp->objectID);
	if (itr == networkObjects.end()) {
		std::cout << "Client Num" << GetClientPlayerNum() << "can't find netObject" << std::endl;
		return false;
	}
	itr->second->ReadPacket(*fp);
	if (fp->fullState.stateID > GlobalStateID) { GlobalStateID = fp->fullState.stateID; }
	return true;
}
bool NetworkedGame::clientProcessDp(DeltaPacket* dp)
{
	auto itr = networkObjects.find(dp->objectID);
	if (itr == networkObjects.end()) {
		std::cout << "Client Num" << GetClientPlayerNum() << "can't find netObject" << std::endl;
		return false;
	}
	itr->second->ReadPacket(*dp);
	return true;
}

bool NetworkedGame::serverProcessCP(ClientPacket* cp, int source)
{
	//std::cout << "recieved";
	int playerID = GetClientPlayerNum(source);
	if (playerID != -1)
	{
		Player* thePlayer = (Player*)(serverPlayers[playerID]);
		thePlayer->forceToBeAdded=(cp->forceToBeAdded);
		//if (cp->btnStates[Sprint] == 1) { thePlayer->PlayerSprint(); }
		//if (cp->btnStates[Fire] == 1) { thePlayer->PlayerFire(); }
		thePlayer->orientationNetPlayer = cp->orientationNetPlayer;
		thePlayer->isSpacePressed = cp->btnStates[0];
		/*thePlayer->SetBtnState(Right, cp->btnStates[Right]);
		thePlayer->SetBtnState(Left, cp->btnStates[Left]);*/

		auto i = stateIDs.find(playerID);
		if (i == stateIDs.end()) { stateIDs.insert(std::pair<int, int>(playerID, cp->lastID)); }
		else { i->second = cp->lastID; }
		return true;
	}
	return false;
}

bool NetworkedGame::clientProcessRp(RoundStatePacket* rp)
{
	if (rp->isRoundStart != isRoundStart())
	{
		switch (rp->isRoundStart)
		{
		case true:
			this->isRoundstart = true;
			this->StartLevel();
			break;
		case false:
			this->isRoundstart = false;
			this->LevelOver();
			break;
		}
	}
	return true;
}

void NetworkedGame::UpdateGamePlayerInput(float dt)
{
	if (thisServer)
	{
		if (player != nullptr)
		{
			if (player->isSpacePressed == 1 ? true : false) {
				Vector3 velocity = lockedObject->GetPhysicsObject()->GetLinearVelocity();
				player->GetPhysicsObject()->SetLinearVelocity(Vector3(velocity.x, 2, velocity.z));
			}
			
			player->GetPhysicsObject()->AddForce(player->forceToBeAdded);
			player->GetTransform().SetOrientation(player->orientationNetPlayer);
		}
		for (int i = 1; i < serverPlayers.size(); ++i)
		{
			if (serverPlayers[i] != nullptr)
			{
				Player* thePlayer = (Player*)(serverPlayers[i]);
				if (thePlayer->isSpacePressed == 1 ? true : false) {
					Vector3 velocity = lockedObject->GetPhysicsObject()->GetLinearVelocity();
					thePlayer->GetPhysicsObject()->SetLinearVelocity(Vector3(velocity.x, 2, velocity.z));
				}
				
				thePlayer->GetPhysicsObject()->AddForce(thePlayer->forceToBeAdded);
				thePlayer->GetTransform().SetOrientation(thePlayer->orientationNetPlayer);
			}
		}
		
	}

	
}

void NetworkedGame::ServerSendRoundState()
{
	RoundStatePacket state;
	state.isRoundStart = this->isRoundstart;
	
	thisServer->SendGlobalPacket(state);
}

void NetworkedGame::ServerSendPlayerState()
{
	if (isRoundstart)
	{
		for (int i = 1; i < 4; ++i)
		{
			if (serverPlayers[i] != nullptr)
			{
				PlayerStatePacket state;
				NetworkPlayer* thisPlayer = (NetworkPlayer*)(serverPlayers[i]);
				state.playerNum = i;
				
				thisServer->SendGlobalPacket(state);
			}
		}
	}
}