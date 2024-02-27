#pragma once
#include "BehaviourNode.h"

class Inverter : public BehaviourNode {
private:
    BehaviourNode* childNode;

public:
    Inverter(const std::string& nodeName, BehaviourNode* child) : BehaviourNode(nodeName), childNode(child) {}
    ~Inverter() {}

    BehaviourState Execute(float dt) override {
        BehaviourState childState = childNode->Execute(dt);
        if (childState == Success)
            return Failure;
        else if (childState == Failure)
            return Success;
        else
            return childState;
    }

    void Reset() override {
        childNode->Reset();
        currentState = Initialise;
    }
};