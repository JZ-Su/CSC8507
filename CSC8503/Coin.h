#pragma once
#include "GameObject.h"
#include "GameWorld.h"

namespace NCL {
	namespace CSC8503 {
		class Coin :public GameObject
		{
		public:
			Coin(const std::string& objectname = "");
			~Coin();

			virtual void OnCollisionBegin(GameObject* otherObject) override;
			virtual void OnCollisionEnd(GameObject* otherObject) override;

		private:

		};
	}
}