#include "Door.h"
#include "StateMachine.h"
#include "RenderObject.h"
#include "PhysicsObject.h"
#include "StateTransition.h"

using namespace NCL;
using namespace CSC8503;

/*
	Door rotation:

	   270 degree-> |
		    		|
					| 
 180 degree-> ------ ------  <- 0/-360 degree
		            |
		            |
		90 degree-> |

	positive number: counterclockwise rotate
	negative number: clockwise rotate
*/
Door::Door(Player* player, Vector3 position, float rotation, float resDis) {
	stateMachine = new StateMachine();
	timer = 0.0f;
	isOpening = false;
	activation = true;
	defaultPosition = position;
	defaultRotation = rotation;
	responseDistance = resDis;

	keepState = new State([&](float dt)->void {
		currentState = "keepState";
		timer = 0.0f;
		});
	stateMachine->AddState(keepState);

	opening = new State([&](float dt)->void {
		currentState = "opening";
		float halfLength = GetTransform().GetScale().x / 2;
		switch ((int)defaultRotation)
		{
		case 0:
			GetTransform().SetPosition(defaultPosition + Vector3(halfLength * (std::cosf(timer * PI / 2) - 1), 0,halfLength * std::sinf(timer * PI / 2) ));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation - 90 * timer, Vector3(0, 1, 0))));
			break;
		case 90:
			GetTransform().SetPosition(defaultPosition - Vector3(halfLength * std::sinf(timer * PI / 2), 0, halfLength * (1 - std::cosf(timer * PI / 2))));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation - 90 * timer, Vector3(0, 1, 0))));
			break;
		case 180:
			GetTransform().SetPosition(defaultPosition - Vector3(halfLength * (std::cosf(timer * PI / 2) - 1), 0, halfLength * std::sinf(timer * PI / 2)));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation - 90 * timer, Vector3(0, 1, 0))));
			break;
		case 270:
			GetTransform().SetPosition(defaultPosition + Vector3(halfLength * std::sinf(timer * PI / 2), 0, halfLength * (1 - std::cosf(timer * PI / 2))));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation - 90 * timer, Vector3(0, 1, 0))));
			break;
		case -90:
			GetTransform().SetPosition(defaultPosition + Vector3(halfLength * std::sinf(timer * PI / 2), 0, halfLength * (std::cosf(timer * PI / 2) - 1)));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation + 90 * timer, Vector3(0, 1, 0))));
			break;
		case -180:
			GetTransform().SetPosition(defaultPosition + Vector3(halfLength * (1 - std::cosf(timer * PI / 2)), 0, halfLength * std::sinf(timer * PI / 2)));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation + 90 * timer, Vector3(0, 1, 0))));
			break;
		case -270:
			GetTransform().SetPosition(defaultPosition - Vector3(halfLength * std::sinf(timer * PI / 2), 0, halfLength * (std::cosf(timer * PI / 2) - 1)));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation + 90 * timer, Vector3(0, 1, 0))));
			break;
		case -360:
			GetTransform().SetPosition(defaultPosition - Vector3(halfLength * (1 - std::cosf(timer * PI / 2)), 0, halfLength * std::sinf(timer * PI / 2)));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation + 90 * timer, Vector3(0, 1, 0))));
			break;
		default:
			break;
		}
		timer += dt;
		isOpening = true;
		});
	stateMachine->AddState(opening);

	closing = new State([&](float dt)->void {
		currentState = "closing";
		float halfLength = GetTransform().GetScale().x / 2;
		switch ((int)defaultRotation)
		{
		case 0:
			GetTransform().SetPosition(defaultPosition + Vector3(halfLength * (std::sinf(timer * PI / 2) - 1), 0, halfLength * std::cosf(timer * PI / 2)));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation - 90 * (1 - timer), Vector3(0, 1, 0))));
			break;
		case 90:
			GetTransform().SetPosition(defaultPosition - Vector3(halfLength * std::cosf(timer * PI / 2), 0, halfLength * (1 - std::sinf(timer * PI / 2))));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation - 90 * (1 - timer), Vector3(0, 1, 0))));
			break;
		case 180:
			GetTransform().SetPosition(defaultPosition - Vector3(halfLength * (std::sinf(timer * PI / 2) - 1), 0, halfLength * std::cosf(timer * PI / 2)));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation - 90 * (1 - timer), Vector3(0, 1, 0))));
			break;
		case 270:
			GetTransform().SetPosition(defaultPosition + Vector3(halfLength * std::cosf(timer * PI / 2), 0, halfLength * (1 - std::sinf(timer * PI / 2))));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation - 90 * (1 - timer), Vector3(0, 1, 0))));
			break;
		case -90:
			GetTransform().SetPosition(defaultPosition + Vector3(halfLength * std::cosf(timer * PI / 2), 0, halfLength * (std::sinf(timer * PI / 2) - 1)));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation + 90 * (1 - timer), Vector3(0, 1, 0))));
			break;
		case -180:
			GetTransform().SetPosition(defaultPosition + Vector3(halfLength * (1 - std::sinf(timer * PI / 2)), 0, halfLength * std::cosf(timer * PI / 2)));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation + 90 * (1 - timer), Vector3(0, 1, 0))));
			break;
		case -270:
			GetTransform().SetPosition(defaultPosition - Vector3(halfLength * std::cosf(timer * PI / 2), 0, halfLength * (std::sinf(timer * PI / 2) - 1)));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation + 90 * (1 - timer), Vector3(0, 1, 0))));
			break;
		case -360:
			GetTransform().SetPosition(defaultPosition - Vector3(halfLength * (1 - std::sinf(timer * PI / 2)), 0, halfLength * std::cosf(timer * PI / 2)));
			GetTransform().SetOrientation(Quaternion(Matrix4::Rotation(defaultRotation + 90 * (1 - timer), Vector3(0, 1, 0))));
			break;
		default:
			break;
		}
		timer += dt;
		isOpening = false;
		});
	stateMachine->AddState(closing);

	StateTransition* aTob = new StateTransition(keepState, opening,
		[&](GameObject* player)->bool {
			if (!activation) return false;
			if (!isOpening && (player->GetTransform().GetPosition() - GetTransform().GetPosition()).Length() <= responseDistance) {
				return true;
			}
			return false;
		}, player); 
	stateMachine->AddTransition(aTob);

	StateTransition* bToa = new StateTransition(opening, keepState,
		[&](float* timer)->bool {
			if (!activation) return false;
			return *timer >= 1.0f;
		}, &timer);
	stateMachine->AddTransition(bToa);

	StateTransition* aToc = new StateTransition(keepState, closing,
		[&](GameObject* player)->bool {
			if (!activation) return false;
			if (isOpening && (player->GetTransform().GetPosition() - GetTransform().GetPosition()).Length() >= responseDistance + 5) {
				return true;
			}
			return false;
		}, player);
	stateMachine->AddTransition(aToc);

	StateTransition* cToa = new StateTransition(closing, keepState,
		[&](float *timer)->bool {
			if (!activation) return false;
			return *timer >= 1.0f;
		}, &timer); 
	stateMachine->AddTransition(cToa);
}

Door::~Door() {
	delete this;
}