#pragma once

#include "StateGameObject.h"

namespace NCL {
	namespace CSC8503 {
		class Door : public StateGameObject {
		public:
			Door(Player* player);
			~Door();

			bool GetActivation() const { return activation; }
			void Deactivation() { activation = false; }

		protected:
			float timer;
			bool isOpening;
			bool activation;
		};
	}
}