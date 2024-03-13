#pragma once
#include "GameObject.h"
#include "NavigationGrid.h"
#include "Player.h"
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
            void MoveLeft(float dt,Vector3 ownpos);
            void MoveRight(float dt, Vector3 ownpos);
            void MoveFront(float dt, Vector3 ownpos);
            void MoveBack(float dt, Vector3 ownpos);
            void FollowPath(float dt, NavigationPath path, bool inversePath);

            formerState state;
            NavigationGrid* grid;
            NavigationPath* patrolPath;
            NavigationPath* chasingPath;
            bool FindPlayer(GameObject* player);
            bool leavePlayer(GameObject* player);

            StateMachine* stateMachine;
            float counter;
            int nodeIndex;
            bool isChasing;
            Vector3 GhostStrat;
            Vector3 GhostEnd;
     
        };
    }
}
