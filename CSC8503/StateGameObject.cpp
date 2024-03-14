#include "StateGameObject.h"
#include "StateTransition.h"
#include "StateMachine.h"
#include "State.h"
#include "PhysicsObject.h"
#include "RenderObject.h"

#include "Debug.h"
#include"Transform.h"
using namespace NCL;
using namespace CSC8503;

StateGameObject::StateGameObject() {
	//counter = 0.0f;
	//stateMachine = new StateMachine();

	//State* stateA = new State([&](float dt)->void {
	//	this->MoveLeft(dt, GetRenderObject()->GetTransform()->GetPosition());
	//	});
	//State* stateB = new State([&](float dt)->void {
	//	this->MoveRight(dt, GetRenderObject()->GetTransform()->GetPosition());
	//	});
	//stateMachine->AddState(stateA);
	//stateMachine->AddState(stateB);

	//Vector3 pos = Vector3();
	//stateMachine->AddTransition(new StateTransition(stateA, stateB,
	//	[&]()->bool {
	//		return this->counter > 3.0f;
	//	})
	//);

	//stateMachine->AddTransition(new StateTransition(stateB, stateA, 
	//	[&]()->bool {
	//		return this->counter < 0.0f;
	//	})
	//);
}

StateGameObject::StateGameObject(Vector3 startPos, Vector3 endPos, GameObject* player,const std::string& objectName) : GameObject(objectName) {
	stateMachine = new StateMachine();
	GhostStrat = startPos;
	GhostEnd = endPos;

	State* stateHunt = new State([&](float dt,GameObject* player)->void {
      Vector3 ghosthunt = (GetTransform().GetPosition() - player->GetTransform().GetPosition()).Normalised() * 0.05;
		GetTransform().SetPosition(GetTransform().GetPosition() - ghosthunt);
		Quaternion targetOrientation = Quaternion::AxisAngleToQuaterion(Vector3(0, -1, 0), Maths::RadiansToDegrees(atan2(ghosthunt.x, -ghosthunt.z)));
		this->GetTransform().SetOrientation(targetOrientation);
		},player);

	State* stateBackA = new State([&](float dt)->void {
		Vector3 ghostbackA = (GetTransform().GetPosition() - GhostStrat).Normalised() * 0.05;
		GetTransform().SetPosition(GetTransform().GetPosition()-ghostbackA);
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
		return GPdistance >= 40 && GBdistance>GAdistance;
		},player));
	stateMachine->AddTransition(new StateTransition(stateHunt, stateBackB, [&](GameObject* player)->bool {
		float GPdistance = (GetTransform().GetPosition() - player->GetTransform().GetPosition()).Length();
		float GAdistance = (GetTransform().GetPosition() - GhostStrat).Length();
		float GBdistance = (GetTransform().GetPosition() - GhostEnd).Length();
		return GPdistance >= 40 && GBdistance < GAdistance;
		},player));
	stateMachine->AddTransition(new StateTransition(stateBackA, stateHunt, [&](GameObject* player)->bool {
		float GPdistance = (GetTransform().GetPosition() - player->GetTransform().GetPosition()).Length();
		return GPdistance <= 20.0f;
		},player));
	stateMachine->AddTransition(new StateTransition(stateBackB, stateHunt, [&](GameObject* player)->bool {
		float GPdistance = (GetTransform().GetPosition() - player->GetTransform().GetPosition()).Length();
		return GPdistance <= 20.0f;
		},player));
	stateMachine->AddTransition(new StateTransition(stateBackA, stateBackB, [&]()->bool {
		float GAdistance = (GetTransform().GetPosition() - GhostStrat).Length();
		return GAdistance< 1;
		}));
	stateMachine->AddTransition(new StateTransition(stateBackB, stateBackA, [&]()->bool {
		float GBdistance = (GetTransform().GetPosition() - GhostEnd).Length();
		return GBdistance < 1;
		}));

}

StateGameObject::~StateGameObject() {
	delete stateMachine;
}

void StateGameObject::Update(float dt) {
	stateMachine->Update(dt);
}

void StateGameObject::MoveLeft(float dt, Vector3 ownpos) {
	//GetPhysicsObject()->AddForce({ -10,0,0 });
	GetRenderObject()->GetTransform()->SetPosition(Vector3(ownpos.x - (dt * 10), ownpos.y, ownpos.z));
	GetRenderObject()->GetTransform()->SetOrientation(Quaternion(0.0f, 1.0f, 0.0f, 0.5f));
}

void StateGameObject::MoveRight(float dt, Vector3 ownpos) {
	//GetPhysicsObject()->AddForce({ 10,0,0 });
	GetRenderObject()->GetTransform()->SetPosition(Vector3(ownpos.x + (dt * 10), ownpos.y, ownpos.z));
	GetRenderObject()->GetTransform()->SetOrientation(Quaternion(0.0f, 1.0f, 0.0f, -0.5f));
}

void StateGameObject::MoveFront(float dt, Vector3 ownpos) {
	//GetPhysicsObject()->AddForce({ 0,0,-10 });
	GetRenderObject()->GetTransform()->SetPosition(Vector3(ownpos.x, ownpos.y, ownpos.z - (dt * 10)));
	GetRenderObject()->GetTransform()->SetOrientation(Quaternion(0.0f, 1.0f, 0.0f, 0.0f));
}

void StateGameObject::MoveBack(float dt, Vector3 ownpos) {
	//GetPhysicsObject()->AddForce({ 0,0,10 });
	GetRenderObject()->GetTransform()->SetPosition(Vector3(ownpos.x, ownpos.y, ownpos.z + (10 * dt)));
	GetRenderObject()->GetTransform()->SetOrientation(Quaternion(0.0f, 1.0f, 0.0f, -1.0f));
}

void StateGameObject::FollowPath(float dt, NavigationPath path, bool inversePath) {
	std::vector<Vector3> pathNodes;
	Vector3 pos;
	while (path.PopWaypoint(pos)) {
		pathNodes.push_back(pos);
	}

	if (!isChasing) {
		for (int i = 1; i < pathNodes.size(); i++) {
			Vector3 a = pathNodes[i - 1];
			Vector3 b = pathNodes[i];
			//Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
		}
	}

	if (inversePath) {
		Vector3 begin = pathNodes[nodeIndex];
		Vector3 end = pathNodes[nodeIndex - 1];
		Vector3 direction = end - begin;
		if (GetRenderObject()->GetTransform()->GetPosition() != end) {
			if (direction.x > 0) {
				this->MoveRight(dt, GetRenderObject()->GetTransform()->GetPosition());
			}
			if (direction.x < 0) {
				this->MoveLeft(dt, GetRenderObject()->GetTransform()->GetPosition());
			}
			if (direction.z > 0) {
				this->MoveBack(dt, GetRenderObject()->GetTransform()->GetPosition());
			}
			if (direction.z < 0) {
				this->MoveFront(dt, GetRenderObject()->GetTransform()->GetPosition());
			}
		}
	
	}
	else {
		Vector3 begin = pathNodes[nodeIndex];
		Vector3 end = pathNodes[nodeIndex + 1];
		Vector3 direction = end - begin;
		Debug::DrawLine(begin, end,Debug::RED);

		if (GetRenderObject()->GetTransform()->GetPosition() != end) {
			if (direction.x > 0) {
				this->MoveRight(dt, GetRenderObject()->GetTransform()->GetPosition());
			}
			if (direction.x < 0) {
				this->MoveLeft(dt, GetRenderObject()->GetTransform()->GetPosition());
			}
			if (direction.z > 0) {
				this->MoveBack(dt, GetRenderObject()->GetTransform()->GetPosition());
			}
			if (direction.z < 0) {
				this->MoveFront(dt, GetRenderObject()->GetTransform()->GetPosition());
			}
		}

	}
	pathNodes.clear();
}

bool StateGameObject::FindPlayer(GameObject* player) {
	float deltaX = std::abs(player->GetRenderObject()->GetTransform()->GetPosition().x - GetRenderObject()->GetTransform()->GetPosition().x);
	float deltaZ = std::abs(player->GetRenderObject()->GetTransform()->GetPosition().z - GetRenderObject()->GetTransform()->GetPosition().z);
	if (deltaX <= 10 && deltaZ <= 10) return true;
	return false;
}

bool StateGameObject::leavePlayer(GameObject* player) {
	float deltaX = std::abs(player->GetRenderObject()->GetTransform()->GetPosition().x - GetRenderObject()->GetTransform()->GetPosition().x);
	float deltaZ = std::abs(player->GetRenderObject()->GetTransform()->GetPosition().z - GetRenderObject()->GetTransform()->GetPosition().z);
	if (deltaX>=20 && deltaZ >= 20) return true;
	return false;
}