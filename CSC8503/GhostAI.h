#pragma once

#include "StateGameObject.h"

namespace NCL {
	namespace CSC8503 {
		class GhostAI :public StateGameObject
		{
		public:
			GhostAI(Vector3 startPos, Vector3 endPos, GameObject* player, const std::string& name = "");
			~GhostAI();

		protected:
			Vector3 GhostStrat;
			Vector3 GhostEnd;
		};

	}
}
