#include "Door.h"
#include "StateMachine.h"
#include "RenderObject.h"
#include "PhysicsObject.h"
#include "StateTransition.h"

using namespace NCL;
using namespace CSC8503;

Door::Door(Player* player, Vector3 position, float rotation) {
	stateMachine = new StateMachine();
	timer = 0.0f;
	isOpening = false;
	activation = true;
	defaultPosition = position;
	defaultRotation = rotation;

	State* keepState = new State([&](float dt)->void {
		this->timer = 0.0f;
		});
	stateMachine->AddState(keepState);

	State* opening = new State([&](float dt)->void {
		float halfLength = this->GetTransform().GetScale().x / 2;
		switch ((int)defaultRotation)
		{
		case 0:
			this->GetTransform().SetPosition(defaultPosition - Vector3(halfLength * (std::cosf(timer * PI / 2) - 1), 0, halfLength * std::sinf(timer * PI / 2)));
			this->GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation - 90 * timer, Vector3(0, 1, 0))));
			break;
		case 90:
			this->GetTransform().SetPosition(defaultPosition - Vector3(halfLength * std::sinf(timer * PI / 2), 0, halfLength * (1 - std::cosf(timer * PI / 2))));
			this->GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation - 90 * timer, Vector3(0, 1, 0))));
			break;
		case 180:
			this->GetTransform().SetPosition(defaultPosition + Vector3(halfLength * (1 - std::cosf(timer * PI / 2)), 0, halfLength * std::sinf(timer * PI / 2)));
			this->GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation + 90 * timer, Vector3(0, 1, 0))));
			break;
		case 270:
			this->GetTransform().SetPosition(defaultPosition + Vector3(halfLength * std::sinf(timer * PI / 2), 0, halfLength * (std::cosf(timer * PI / 2) - 1)));
			this->GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation + 90 * timer, Vector3(0, 1, 0))));
			break;
		default:
			break;
		}
		this->timer += dt;
		this->isOpening = true;
		});
	stateMachine->AddState(opening);

	State* closing = new State([&](float dt)->void {
		float halfLength = this->GetTransform().GetScale().x / 2;
		switch ((int)defaultRotation)
		{
		case 0:
			this->GetTransform().SetPosition(defaultPosition - Vector3(halfLength * (std::sinf(timer * PI / 2) - 1), 0, halfLength * std::cosf(timer * PI / 2)));
			this->GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation - 90 * (1 - timer), Vector3(0, 1, 0))));
			break;
		case 90:
			this->GetTransform().SetPosition(defaultPosition - Vector3(halfLength * std::cosf(timer * PI / 2), 0, halfLength * (1 - std::sinf(timer * PI / 2))));
			this->GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation - 90 * (1 - timer), Vector3(0, 1, 0))));
			break;
		case 180:
			this->GetTransform().SetPosition(defaultPosition + Vector3(halfLength * (1 - std::sinf(timer * PI / 2)), 0, halfLength * std::cosf(timer * PI / 2)));
			this->GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation + 90 * (1 - timer), Vector3(0, 1, 0))));
			break;
		case 270:
			this->GetTransform().SetPosition(defaultPosition + Vector3(halfLength * std::cosf(timer * PI / 2), 0, halfLength * (std::sinf(timer * PI / 2) - 1)));
			this->GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation + 90 * (1 - timer), Vector3(0, 1, 0))));
			break;
		default:
			break;
		}
		this->timer += dt;
		this->isOpening = false;
		});
	stateMachine->AddState(closing);

	StateTransition* aTob = new StateTransition(keepState, opening,
		[&](GameObject* player)->bool {
			if (isOpening == false && (player->GetTransform().GetPosition() - GetTransform().GetPosition()).Length() <= 15) {
				return true;
			}
			return false;
		}, player); 
	stateMachine->AddTransition(aTob);

	StateTransition* bToa = new StateTransition(opening, keepState,
		[&](float* timer)->bool {
			return *timer >= 1.0f;
		}, &timer);
	stateMachine->AddTransition(bToa);

	StateTransition* aToc = new StateTransition(keepState, closing,
		[&](GameObject* player)->bool {
			if (isOpening == true && (player->GetTransform().GetPosition() - GetTransform().GetPosition()).Length() >= 20) {
				return true;
			}
			return false;
		}, player);
	stateMachine->AddTransition(aToc);

	StateTransition* cToa = new StateTransition(closing, keepState,
		[&](float *timer)->bool {
			return *timer >= 1.0f;
		}, &timer); 
	stateMachine->AddTransition(cToa);
}

Door::~Door() {
	delete this;
}