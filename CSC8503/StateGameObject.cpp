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
