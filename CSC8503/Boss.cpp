#include "GameObject.h"
#include "Boss.h"
#include "Window.h"
#include "BasicExamples.h"
#include "RenderObject.h"
#include "GameWorld.h"

using namespace NCL;
using namespace CSC8503;
Boss::Boss(Player* player) {
	this->player = player;
	// 更新目标位置
	Vector3 targetPosition = GetTransform().GetPosition() + Vector3(0, 0, 8);
	remoteAttackRange = 40.0f;
	meleeAttackRange = 60.0f;
	bossHealth = 100.0f;
	Idle = new BehaviourAction("Idle", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise) {
			std::cout << "Idle init\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			this->distanceToTarget = calculateDistance(GetTransform().GetPosition(), this->player->GetTransform().GetPosition());
			if (this->distanceToTarget <= this->remoteAttackRange) {
				Debug::DrawLine(GetTransform().GetPosition(), this->player->GetTransform().GetPosition(), Debug::RED);
				Debug::DrawCollisionBox(this);
				Debug::DrawCollisionBox(this->player);
				std::cout << this->distanceToTarget << std::endl;
				return Failure;
			}
			else {

				// 检查当前位置是否已经接近目标位置
				float distanceToTarget = calculateDistance(GetTransform().GetPosition(), targetPosition);
				if (distanceToTarget <= 1.0f) { // 如果已经接近目标位置，则生成新的目标位置
					std::random_device rd;
					std::mt19937 gen(rd());
					std::uniform_real_distribution<float> dis(-20.0f, 20.0f); // offset

					float offsetX = dis(gen);
					float offsetZ = dis(gen);

					// 更新目标位置
					targetPosition = GetTransform().GetPosition() + Vector3(offsetX, 0, offsetZ);
				}

				// 计算移向目标位置的方向
				Vector3 direction = targetPosition - GetTransform().GetPosition();
				direction.Normalise();

				// 计算移动的距离（速度 * 时间）
				float moveDistance = 10.0f * dt;

				// 根据移动距离更新物体位置
				Vector3 newPosition = GetTransform().GetPosition() + direction * moveDistance;
				GetTransform().SetPosition(newPosition);

				std::cout << targetPosition <<"idle.\n";
				return Ongoing;
			}
		}
		return state;//will be ongoing until success or condition to switch
		}
	);

	MeleeAttack = new BehaviourAction("MeleeAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "MeleeAttacking init.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			if (distanceToTarget <= meleeAttackRange) {
				std::cout << "MeleeAttack.\n";
				/*return Success;*/
				return Failure;
			}
			else {
				std::cout << "Player out of range for melee attack.\n";
				return Failure;
			}
		}
		return state;
		}
	);
	RemoteAttack = new BehaviourAction("RemoteAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "RemoteAttacking init.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			if (distanceToTarget <= remoteAttackRange) {
				std::cout << "RemoteAttack.\n";
				/*return Success;*/
				return Failure;
			}
			else {
				std::cout << "Player out of range for remote attack.\n";
				return Failure;
			}
		}
		return state;
		}
	);
	Flinches = new BehaviourAction("Flinches", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "Flinches init.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			std::cout << "Flinches.\n";
			/*return Ongoing;*/
			return Failure;
		}
		return state;
		}
	);
	Death = new BehaviourAction("Death", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "Death init\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			std::cout << "Death.\n";
			/*PlayDeathAnimation();*/
			//RemoveBossObject();
			return Ongoing;
		}
		return state;
		}
	);
	Selector = new BehaviourSelector("FirstLevel");
	Selector->AddChild(Idle);
	Selector->AddChild(MeleeAttack);
	Selector->AddChild(RemoteAttack);
	Selector->AddChild(Flinches);
	//Selector->AddChild(Death);
}

void Boss::Update(float dt) {
	distanceToTarget = calculateDistance(GetTransform().GetPosition(), player->GetTransform().GetPosition());
	Selector->Execute(dt);
}

float Boss::calculateDistance(Vector3 pos1, Vector3 pos2) {
	float distance = std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2) + std::pow(pos2.z - pos1.z, 2));
	return distance;
}

Boss::~Boss() {
	delete Idle;
	delete MeleeAttack;
	delete RemoteAttack;
	delete Flinches;
	delete Death;
	delete Parallel;
}