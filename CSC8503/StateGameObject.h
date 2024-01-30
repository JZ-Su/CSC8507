#pragma once
#include "GameObject.h"
#include "NavigationGrid.h"
#include "State.h"

namespace NCL {
    namespace CSC8503 {
        enum formerState
        {
            forward,
            reverse
        };
        class StateMachine;
        class StateGameObject : public GameObject  {
        public:
            StateGameObject();
            StateGameObject(Vector3 startPos, Vector3 endPos, GameObject* player, const std::string& name = "");
            ~StateGameObject();

            virtual void Update(float dt);

        protected:
            void MoveLeft(float dt);
            void MoveRight(float dt);
            void MoveFront(float dt);
            void MoveBack(float dt);
            void FollowPath(float dt, NavigationPath path, bool inversePath);

            formerState state;
            NavigationGrid* grid;
            NavigationPath* patrolPath;
            NavigationPath* chasingPath;
            bool FindPlayer(GameObject* player);

            StateMachine* stateMachine;
            float counter;
            int nodeIndex;
            bool isChasing;
        };
    }
}
