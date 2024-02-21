#pragma once
#include "BehaviourNode.h"

class BehaviourNodeWithChildren : public BehaviourNode	{
public:
	BehaviourNodeWithChildren(const std::string& nodeName) : BehaviourNode(nodeName){};
	~BehaviourNodeWithChildren() {
		for (auto& i : childNodes) {
			delete i;
		}
	}
	void AddChild(BehaviourNode* n) {
		childNodes.emplace_back(n);
	}

	void Reset() override {
		currentState = Initialise;
		for (auto& i : childNodes) {
			i->Reset();
		}
	}
protected:
	std::vector<BehaviourNode*> childNodes;
};

class ParallelBehaviour : public BehaviourNodeWithChildren {
public:
    BehaviourState Execute(float dt) override {
        bool allChildrenSucceeded = true;
        bool anyChildRunning = false;

        for (auto& child : childNodes) {
            BehaviourState childState = child->Execute(dt);

            if (childState == BehaviourState::Failure) {
                allChildrenSucceeded = false;
            }
            else if (childState == BehaviourState::Ongoing) {
                anyChildRunning = true;
            }
        }

        if (allChildrenSucceeded) {
            return BehaviourState::Success;
        }
        else if (anyChildRunning) {
            return BehaviourState::Ongoing;
        }
        else {
            return BehaviourState::Failure;
        }
    }
};