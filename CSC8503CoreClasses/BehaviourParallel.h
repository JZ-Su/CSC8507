#pragma once
#include "BehaviourNodeWithChildren.h"

class BehaviourParallel : public BehaviourNodeWithChildren {
public:
    BehaviourParallel(const std::string& nodeName) : BehaviourNodeWithChildren(nodeName) {}
    ~BehaviourParallel() {}
    BehaviourState Execute(float dt) override {
        std::cout << "Executing parallel behaviour " << name << "\n";
        bool anyNodeOngoing = false;
        bool anyNodeFailed = false;

        for (auto& i : childNodes) {
            BehaviourState nodeState = i->Execute(dt);
            switch (nodeState) {
            case Failure:
                anyNodeFailed = true;
                break;
            case Success:
                continue;
            case Ongoing:
                anyNodeOngoing = true;
                break;
            }
        }

        if (anyNodeFailed)
            return Failure;
        else if (anyNodeOngoing)
            return Ongoing;
        else
            return Success;
    }
};
