#pragma once

#include "StateGameObject.h"

namespace NCL {
	namespace CSC8503 {
		class Door : public StateGameObject {
		public:
			Door(Player* player);
			~Door();

		protected:
			float timer;
			bool isOpening;
		};
	}
}