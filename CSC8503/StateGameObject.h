#pragma once
#include "GameObject.h"
#include "NavigationGrid.h"
#include "Player.h"
#include "State.h"

namespace NCL {
    namespace CSC8503 {
        class StateMachine;
        class StateGameObject : public GameObject  {
        public:
            StateGameObject();
            ~StateGameObject();

            virtual void Update(float dt);

        protected:
            void MoveLeft(float dt,Vector3 ownpos);
            void MoveRight(float dt, Vector3 ownpos);
            void MoveFront(float dt, Vector3 ownpos);
            void MoveBack(float dt, Vector3 ownpos);

            NavigationGrid* grid;
            NavigationPath* patrolPath;
            NavigationPath* chasingPath;

            StateMachine* stateMachine;
        };
    }
}
