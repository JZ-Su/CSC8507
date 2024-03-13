#pragma once
#include "GameObject.h"
#include "NetworkBase.h"
#include "NetworkState.h"

namespace NCL::CSC8503 {
	class GameObject;

	struct FullPacket : public GamePacket {
		int		objectID = -1;
		NetworkState fullState;

		FullPacket() {
			type = Full_State;
			size = sizeof(FullPacket) - sizeof(GamePacket);
		}
	};

	struct DeltaPacket : public GamePacket {
		int		fullID		= -1;
		int		objectID	= -1;
		char	pos[3];
		char	orientation[4];

		DeltaPacket() {
			type = Delta_State;
			size = sizeof(DeltaPacket) - sizeof(GamePacket);
		}
	};

	struct ClientPacket : public GamePacket {
		int		lastID;

		char	buttonstates[5];

		ClientPacket() {
			size = sizeof(ClientPacket);
		}
	};

	struct PLayerListPacket : public GamePacket
	{
		char playerList[4];
		PLayerListPacket(std::vector<int>& serverPlayerList)
		{
			type = Message;
			size = sizeof(PLayerListPacket);

			for (int i = 0; i < 4; ++i)
			{
				playerList[i] = serverPlayerList[i] + 48;
			}
		}

		void GetPlayerList(std::vector<int>& clientPlayerList)
		{
			for (int i = 0; i < 4; ++i)
			{
				clientPlayerList[i] = playerList[i] - 48;
			}
		}
	};
	struct PlayerStatePacket : public GamePacket
	{
		char playerNum;
		char state[4] = { 0,0,0,0 };

		PlayerStatePacket()
		{
			type = Player_State;
			size = sizeof(PlayerStatePacket);
		}
	};
	class NetworkObject		{
	public:
		NetworkObject(GameObject& o, int id);
		virtual ~NetworkObject();

		//Called by clients
		virtual bool ReadPacket(GamePacket& p);
		//Called by servers
		virtual bool WritePacket(GamePacket** p, bool deltaFrame, int stateID);

		void UpdateStateHistory(int minID);
		int getNetWorkID()const { return networkID; }
		GameObject* getGameObjectPtr() const { return &object; }

	protected:

		NetworkState& GetLatestNetworkState();

		bool GetNetworkState(int frameID, NetworkState& state);

		virtual bool ReadDeltaPacket(DeltaPacket &p);
		virtual bool ReadFullPacket(FullPacket &p);

		virtual bool WriteDeltaPacket(GamePacket**p, int stateID);
		virtual bool WriteFullPacket(GamePacket**p);

		GameObject& object;

		NetworkState lastFullState;

		std::vector<NetworkState> stateHistory;

		int deltaErrors;
		int fullErrors;

		int networkID;
	};
}