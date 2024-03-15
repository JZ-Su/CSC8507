#include "GhostAI.h"
#include "StateGameObject.h"
#include "StateTransition.h"
#include "StateMachine.h"
#include "State.h"
#include "PhysicsObject.h"
#include "RenderObject.h"

using namespace NCL;
using namespace CSC8503;

GhostAI::GhostAI(Vector3 startPos, Vector3 endPos, GameObject* player, const std::string& objectName) {
	stateMachine = new StateMachine();
	GhostStrat = startPos;
	GhostEnd = endPos;

	State* stateHunt = new State([&](float dt, GameObject* player)->void {
		Vector3 ghosthunt = (GetTransform().GetPosition() - player->GetTransform().GetPosition()).Normalised() * 0.15;
		GetTransform().SetPosition(GetTransform().GetPosition() - ghosthunt);
		Quaternion targetOrientation = Quaternion::AxisAngleToQuaterion(Vector3(0, -1, 0), Maths::RadiansToDegrees(atan2(ghosthunt.x, -ghosthunt.z)));
		this->GetTransform().SetOrientation(targetOrientation);
		}, player);

	State* stateBackA = new State([&](float dt)->void {
		Vector3 ghostbackA = (GetTransform().GetPosition() - GhostStrat).Normalised() * 0.05;
		GetTransform().SetPosition(GetTransform().GetPosition() - ghostbackA);
		Quaternion targetOrientation = Quaternion::AxisAngleToQuaterion(Vector3(0, -1, 0), Maths::RadiansToDegrees(atan2(ghostbackA.x, -ghostbackA.z)));
		this->GetTransform().SetOrientation(targetOrientation);
		});

	State* stateBackB = new State([&](float dt)->void {
		Vector3 ghostbackB = (GetTransform().GetPosition() - GhostEnd).Normalised() * 0.05;
		GetTransform().SetPosition(GetTransform().GetPosition() - ghostbackB);
		Quaternion targetOrientation = Quaternion::AxisAngleToQuaterion(Vector3(0, -1, 0), Maths::RadiansToDegrees(atan2(ghostbackB.x, -ghostbackB.z)));
		this->GetTransform().SetOrientation(targetOrientation);
		});

	stateMachine->AddState(stateBackB);
	stateMachine->AddState(stateBackA);
	stateMachine->AddState(stateHunt);

	stateMachine->AddTransition(new StateTransition(stateHunt, stateBackA, [&](GameObject* player)->bool {
		float GPdistance = (GetTransform().GetPosition() - player->GetTransform().GetPosition()).Length();
		float GAdistance = (GetTransform().GetPosition() - GhostStrat).Length();
		float GBdistance = (GetTransform().GetPosition() - GhostEnd).Length();
		return GPdistance >= 40 && GBdistance > GAdistance;
		}, player));
	stateMachine->AddTransition(new StateTransition(stateHunt, stateBackB, [&](GameObject* player)->bool {
		float GPdistance = (GetTransform().GetPosition() - player->GetTransform().GetPosition()).Length();
		float GAdistance = (GetTransform().GetPosition() - GhostStrat).Length();
		float GBdistance = (GetTransform().GetPosition() - GhostEnd).Length();
		return GPdistance >= 40 && GBdistance < GAdistance;
		}, player));
	stateMachine->AddTransition(new StateTransition(stateBackA, stateHunt, [&](GameObject* player)->bool {
		float GPdistance = (GetTransform().GetPosition() - player->GetTransform().GetPosition()).Length();
		return GPdistance <= 20.0f;
		}, player));
	stateMachine->AddTransition(new StateTransition(stateBackB, stateHunt, [&](GameObject* player)->bool {
		float GPdistance = (GetTransform().GetPosition() - player->GetTransform().GetPosition()).Length();
		return GPdistance <= 20.0f;
		}, player));
	stateMachine->AddTransition(new StateTransition(stateBackA, stateBackB, [&]()->bool {
		float GAdistance = (GetTransform().GetPosition() - GhostStrat).Length();
		return GAdistance < 1;
		}));
	stateMachine->AddTransition(new StateTransition(stateBackB, stateBackA, [&]()->bool {
		float GBdistance = (GetTransform().GetPosition() - GhostEnd).Length();
		return GBdistance < 1;
		}));
}