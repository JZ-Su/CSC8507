#include "StateGameObject.h"
#include "StateTransition.h"
#include "StateMachine.h"
#include "State.h"
#include "PhysicsObject.h"
#include "RenderObject.h"

#include "Debug.h"

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

StateGameObject::StateGameObject(Vector3 startPos, Vector3 endPos, GameObject* player, const std::string& objectName) : GameObject(objectName) {
	stateMachine = new StateMachine();
	patrolPath = new NavigationPath();
	chasingPath = new NavigationPath();
	counter = 0.0f;
	nodeIndex = 0;
	isChasing = false;
	grid = new NavigationGrid("TestGrid3.txt", Vector3(-95, 2, -95));
	bool found = (*grid).FindPath(startPos, endPos, *patrolPath);

	if (found) {
		//Forward patrol
		State* stateA = new State([&](float dt)->void {
			counter = 0.0f;
			isChasing = false;
			state = forward;
			FollowPath(dt, *patrolPath, false);
			}
		);
		//Reverse patrol
		State* stateB = new State([&](float dt)->void {
			counter = 0.0f;
			isChasing = false;
			state = reverse;
			FollowPath(dt, *patrolPath, true);
			}
		);
		//Chasing
		State* stateChasing = new State([&](float dt, GameObject* player)->void {
			Vector3 playerposition = player->GetRenderObject()->GetTransform()->GetPosition();
			Vector3 ghostPostion = GetRenderObject()->GetTransform()->GetPosition();
			Vector3 ghostgo = (playerposition - ghostPostion).Normalised();
			GetPhysicsObject()->AddForce(ghostgo * 1000);

 			NavigationGrid* grid = new NavigationGrid("TestGrid3.txt", Vector3(-95, 2, -95));
			bool found = (*grid).FindPath(GetRenderObject()->GetTransform()->GetPosition(), player->GetRenderObject()->GetTransform()->GetPosition(), *chasingPath);

			if (!isChasing) {
				isChasing = true;

			}

			if (found) {
				nodeIndex = 0;
				FollowPath(dt, *chasingPath, false);
			}
			else {
				counter += dt;
				FollowPath(dt, *chasingPath, false);
			}
			}, player
		);
	
		stateMachine->AddState(stateA);
		stateMachine->AddState(stateB);
		stateMachine->AddState(stateChasing);

		stateMachine->AddTransition(new StateTransition(stateA, stateB,
			[&](Vector3 endPos)->bool {
				Vector3 position = this->GetRenderObject()->GetTransform()->GetPosition();
				return (position - endPos).Length() <= position.y;
			}, endPos)
		);
		stateMachine->AddTransition(new StateTransition(stateB, stateA,
			[&](Vector3 startPos)->bool {
				Vector3 position = this->GetRenderObject()->GetTransform()->GetPosition();
				return (position - startPos).Length() <= position.y;
			}, startPos)
		);
		stateMachine->AddTransition(new StateTransition(stateA, stateChasing,
			[&](GameObject* player)->bool {
				if (this->isChasing) return false;
				return FindPlayer(player);
			}, player)
		);
		stateMachine->AddTransition(new StateTransition(stateB, stateChasing,
			[&](GameObject* player)->bool {
				if (this->isChasing) return false;
				return FindPlayer(player);
			}, player)
		);
	/*	stateMachine->AddTransition(new StateTransition(stateChasing, stateA,
			[&](GameObject* player)->bool {
			
			}, player)
			*/
			/*);*/
	}
}

StateGameObject::StateGameObject(GameObject* player, const std::string& objectName) : GameObject(objectName) {
	stateMachine = new StateMachine();
	patrolPath = new NavigationPath();
	chasingPath = new NavigationPath();
	counter = 0.0f;
	nodeIndex = 0;

	State* stateChasing = new State([&](float dt, GameObject* player)->void {
		NavigationGrid* grid = new NavigationGrid("TestGrid3.txt", Vector3(-100, 2, -100));
		bool found = (*grid).FindPath(GetRenderObject()->GetTransform()->GetPosition(), Vector3(10, -2, 10), *chasingPath);
		//Debug::DrawLine(GetRenderObject()->GetTransform()->GetPosition() + Vector3(0,5,0), player->GetRenderObject()->GetTransform()->GetPosition() + Vector3(0, 5, 0), Debug::GREEN);

		FollowPath(dt, *chasingPath, false);
		}, player
	);
	stateMachine->AddState(stateChasing);

	 
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
	GetRenderObject()->GetTransform()->SetOrientation(Quaternion(0.0f, 1.0f, 0.0f, 0.0f));
}

void StateGameObject::MoveRight(float dt, Vector3 ownpos) {
	//GetPhysicsObject()->AddForce({ 10,0,0 });
	GetRenderObject()->GetTransform()->SetPosition(Vector3(ownpos.x + (dt * 10), ownpos.y, ownpos.z));
	GetRenderObject()->GetTransform()->SetOrientation(Quaternion(0.0f, -1.0f, 0.0f, 0.0f));
}

void StateGameObject::MoveFront(float dt, Vector3 ownpos) {
	//GetPhysicsObject()->AddForce({ 0,0,-10 });
	GetRenderObject()->GetTransform()->SetPosition(Vector3(ownpos.x, ownpos.y, ownpos.z - (dt * 10)));
	GetRenderObject()->GetTransform()->SetOrientation(Quaternion(0.0f, 0.0f, 1.0f, 0.0f));
}

void StateGameObject::MoveBack(float dt, Vector3 ownpos) {
	//GetPhysicsObject()->AddForce({ 0,0,10 });
	GetRenderObject()->GetTransform()->SetPosition(Vector3(ownpos.x, ownpos.y, ownpos.z + (10 * dt)));
	GetRenderObject()->GetTransform()->SetOrientation(Quaternion(0.0f, 0.0f, -1.0f, 0.0f));
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
			Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
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
	//else {
	//	//Vector3 begin = pathNodes[nodeIndex];
	//	//Vector3 end = pathNodes[nodeIndex + 1];
	//	//Vector3 direction = end - begin;
	//	//Vector3 velocity = GetPhysicsObject()->GetLinearVelocity();

	//	this->MoveRight(dt, GetRenderObject()->GetTransform()->GetPosition());
	//	//if (GetRenderObject()->GetTransform()->GetPosition() != end) {
	//	//	if (direction.x > 0) {
	//	//		this->MoveRight(dt, GetRenderObject()->GetTransform()->GetPosition());
	//	//	}
	//	//	if (direction.x < 0) {
	//	//		this->MoveLeft(dt, GetRenderObject()->GetTransform()->GetPosition());
	//	//	}
	//	//	if (direction.z > 0) {
	//	//		this->MoveBack(dt, GetRenderObject()->GetTransform()->GetPosition());
	//	//	}
	//	//	if (direction.z < 0) {
	//	//		this->MoveFront(dt, GetRenderObject()->GetTransform()->GetPosition());
	//	//	}
	//	//}
	//	/*if ((GetRenderObject()->GetTransform()->GetPosition() - end).Length() <= 0.5) {
	//		if (!isChasing) {
	//			GetRenderObject()->GetTransform()->SetPosition(end);
	//			GetPhysicsObject()->ClearForces();
	//			GetPhysicsObject()->SetLinearVelocity(Vector3());
	//		}
	//		nodeIndex++;
	//		nodeIndex = std::min(nodeIndex, (int)pathNodes.size() - 1);
	//	}*/
	//}
	pathNodes.clear();
}

bool StateGameObject::FindPlayer(GameObject* player) {
	float deltaX = std::abs(player->GetRenderObject()->GetTransform()->GetPosition().x - GetRenderObject()->GetTransform()->GetPosition().x);
	float deltaZ = std::abs(player->GetRenderObject()->GetTransform()->GetPosition().z - GetRenderObject()->GetTransform()->GetPosition().z);
	if (deltaX <= 15 && deltaZ <= 15) return true;
	return false;
}

bool StateGameObject::timecouculate(float dt) {
	time += dt;
	if (time > 3.0)return true;


}