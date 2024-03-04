#include "Coin.h"

using namespace NCL;
using namespace CSC8503;

Coin::Coin(const std::string& objectname) {

}

Coin::~Coin() {

}

void Coin::OnCollisionBegin(GameObject* otherObject) {
	if (otherObject->GetTag() == "player") {
		GetTransform().SetPosition(Vector3(0, -50, 0));
		Deactivate();
	}
}

void Coin::OnCollisionEnd(GameObject* otherObject) {

}