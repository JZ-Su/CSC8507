#pragma once

#include "StateGameObject.h"

namespace NCL {
	namespace CSC8503 {
		class Door : public StateGameObject {
		public:
			Door(Player* player, Vector3 position = Vector3(), float rotation = 0, float resDis = 15);
			~Door();

			bool GetActivation() const { return activation; }
			void Activation() { activation = true; }
			void Deactivation() { activation = false; }

			void SetDefaultPos(Vector3 pos) { defaultPosition = pos; }
			Vector3 GetDefaultPos() const { return defaultPosition; }

			void SetDefaultRotation(float r) { defaultRotation = r; }
			float GetDafaultRotation() const { return defaultRotation; }

			std::string GetState() const { return currentState; }
			float GetTimer() const { return timer; }

		protected:
			std::string currentState;
			State* keepState;
			State* opening;
			State* closing;

			float timer;
			bool isOpening;
			bool activation;

			Vector3 defaultPosition;
			float defaultRotation;

			float responseDistance;
		};
	}
}