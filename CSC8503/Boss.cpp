#include "GameObject.h"
#include "Boss.h"
#include "Window.h"
#include "BasicExamples.h"
#include "RenderObject.h"
#include "GameWorld.h"
#include "TutorialGame.h"

using namespace NCL;
using namespace CSC8503;
Boss::Boss(Player* player) {
	this->player = player;
	remoteAttackRange = 110.0f;
	bossHealth = 100.0f;
	isAttack = false;
	isShooting = false;
	hasIceCubeBullet = true;
	hasFireBallBullet = true;
	chaseSpeed = 6.0f;
	attackRange = 30.0f;
	chaseRange = 40.0f;
	static float lastAttackTime = 4.0f;
	static float attackCooldown = 4.0f;
	Idle = new BehaviourAction("Idle", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise) {
			isChasing = false;
			state = Ongoing;
		}
		else if (state == Ongoing) {
			this->distanceToTarget = calculateDistance(this->GetTransform().GetPosition(), this->player->GetTransform().GetPosition());
			if (distanceToTarget > remoteAttackRange) {
				//std::cout << "idle.\n";
			}
			return Failure;
		}
		return state;//will be ongoing until success or condition to switch
		}
	);

	ChaseAndAttack = new BehaviourAction("ChaseAndAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		float playerHealth = this->player->GetHealth();
		float bossHealth = this->getBossHealth();
		Vector3 targetPosition;
		Vector3 bossPosition;
		targetPosition = this->player->GetTransform().GetPosition();
		bossPosition = this->GetTransform().GetPosition();
		float distanceToPlayer = calculateDistance(targetPosition, bossPosition);
		Vector3 direction = (targetPosition - bossPosition).Normalised();
		if (distanceToPlayer > chaseRange || this->getIsRencentlyHurt()||bossHealth <= 0) {
			return Failure;
		}
		if (bossHealth <= 50 && firstBelow50) {
			firstBelow50 = false;
			return Failure;
		}
		//if (bossHealth <= 0) {
		//    std::cout << "Boss health depleted.\n";
		//    return Failure;
		//}
		attackBool = (distanceToPlayer <= attackRange) && (lastAttackTime >= attackCooldown);
		attackCooldownBool = (distanceToPlayer <= chaseRange) && (lastAttackTime < attackCooldown);
		chaseBool = (distanceToPlayer <= chaseRange) && (distanceToPlayer >= attackRange);
		if (attackBool) {
			isAttack = true;
			lastAttackTime = 0.0f;
			Debug::DrawLine(bossPosition, bossPosition + direction * attackRange, Debug::RED);
			lastAttackTime += dt;
			return Ongoing;
		}
		else if ((chaseBool || attackCooldown)&& !this->player->getIsBeingHitBack()) {
			Quaternion targetOrientation = Quaternion::AxisAngleToQuaterion(Vector3(0, -1, 0), Maths::RadiansToDegrees(atan2(direction.x, -direction.z)));
			this->GetTransform().SetOrientation(targetOrientation);
			bossPosition += direction * chaseSpeed * dt;
			this->GetTransform().SetPosition(bossPosition);
			Debug::DrawLine(bossPosition, this->player->GetTransform().GetPosition(), Debug::GREEN);
			Debug::DrawLine(GetTransform().GetPosition(), this->player->GetTransform().GetPosition(), Debug::RED);
			Debug::DrawCollisionBox(this);
			Debug::DrawCollisionBox(this->player);
			isChasing = true;
			lastAttackTime += dt;
			return Ongoing;
		}
		else {
			lastAttackTime += dt;
			return Failure;
		}



		});
	RemoteAttack = new BehaviourAction("RemoteAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			//std::cout << "RAttacking init.\n";
			isChasing = false;
			state = Ongoing;
		}
		else if (state == Ongoing) {
			this->distanceToTarget = calculateDistance(this->GetTransform().GetPosition(), this->player->GetTransform().GetPosition());
			if (this->distanceToTarget > this->remoteAttackRange || this->distanceToTarget < this->chaseRange || this->getIsRencentlyHurt()||bossHealth<=0) {
				isShooting = false;
				//std::cout << "stop remote attacking" << std::endl;
				return Failure;
			}
			if (this->getBossHealth() <= 50 && firstBelow50) {
				firstBelow50 = false;
				return Failure;
			}
			else if (this->distanceToTarget <= this->remoteAttackRange && this->distanceToTarget >= chaseRange) {
				Vector3 targetPosition;
				Vector3 bossPosition;
				targetPosition = this->player->GetTransform().GetPosition();
				bossPosition = this->GetTransform().GetPosition();
				float distanceToPlayer = calculateDistance(targetPosition, bossPosition);
				Vector3 direction = (targetPosition - bossPosition).Normalised();
				Quaternion targetOrientation = Quaternion::AxisAngleToQuaterion(Vector3(0, -1, 0), Maths::RadiansToDegrees(atan2(direction.x, -direction.z)));
				this->GetTransform().SetOrientation(targetOrientation);
				Debug::DrawLine(GetTransform().GetPosition(), this->player->GetTransform().GetPosition(), Debug::RED);
				Debug::DrawCollisionBox(this);
				Debug::DrawCollisionBox(this->player);
				if (bulletTimer > 12.0F) {
					isShooting = true;
				}
				//std::cout << "remote attacking" << std::endl;
				return Ongoing;
			}
		}
		return state;
		}
	);
	//Inverter* antiRemoteAttack = new Inverter("antiRemoteAttack", RemoteAttack);
	Flinches = new BehaviourAction("Flinches", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			//std::cout << "Flinches init.\n";
			isChasing = false;
			state = Ongoing;
			flinchAnimationTimer = 0.0f;
		}
		else if (state == Ongoing) {
			if (bossHealth <= 0) {
				return Failure;
			}
			if (this->getIsRencentlyHurt()) {
				flinchAnimationTimer += dt;
				if (flinchAnimationTimer >= 0.28f) {
					this->SetIsRencentlyHurt(false);
					return Failure;
				}
				return Ongoing;
			}
			else {
				return Failure;
			}
		}
		return state;
		}
	);
	dizziness = new BehaviourAction("dizziness", [&](float dt, BehaviourState state) -> BehaviourState {
		const float healthThreshold = 50.0f;
		if (bossHealth > healthThreshold || !firstBelow50||bossHealth<=0) {
			return Failure;
		}
		if (state == Initialise) {
			stunTimer = 0.0f;
			return Ongoing;
		}
		else if (state == Ongoing) {
			stunTimer += dt;
			if (stunTimer >= stunDuration) {
				//std::cout << "I will destroy you!!!.\n";
				stunTimer = 0.0f;
				return Success;
			}
			return Ongoing;
		}
		return state;
		}
	);
	Death = new BehaviourAction("Death", [&](float dt, BehaviourState state) -> BehaviourState {
		if (bossHealth != 0) {
			return Failure;
		}
		if (bossHealth<=0) {
			this->setIsDead(true);
			return Success;
		}
		return state;
		}
	);

	BehaviourAction* dropItems = new BehaviourAction("DropItems", [&](float dt, BehaviourState state) -> BehaviourState {
		//std::cout << "drop items!" << std::endl;
		this->setIsDroppingMassiveItems(true);
		return Failure;
		});
	BehaviourSelector* selection = new BehaviourSelector("Boss attack state");
	BehaviourSequence* squence = new BehaviourSequence("Boss dizzy state");

	selection->AddChild(Death);
	selection->AddChild(Flinches);
	selection->AddChild(RemoteAttack);
	selection->AddChild(ChaseAndAttack);
	selection->AddChild(squence);
	selection->AddChild(Idle);

	squence->AddChild(dizziness);
	squence->AddChild(dropItems);

	//    selection->AddChild(Death);
	Root = new BehaviourSequence("Root Sequence");
	Root->AddChild(selection);
	//Root = new BehaviourSelector("Root");
	//Combat = new BehaviourParallel("Combat");
	//Combat->AddChild(dizziness);
	//Combat->AddChild(antiRemoteAttack);
	//Combat->AddChild(ChaseAndAttack);
	//Root->AddChild(Death);
	//Root->AddChild(Combat);
	//Root->AddChild(Idle);
}


void Boss::Update(float dt) {
	if (Root != nullptr) {
		Root->Execute(dt);
	}
}

float Boss::calculateDistance(Vector3 pos1, Vector3 pos2) {
	float distance = std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2) + std::pow(pos2.z - pos1.z, 2));
	return distance;
}

Boss::~Boss() {
	delete Idle;
	delete RemoteAttack;
	delete ChaseAndAttack;
	delete Flinches;
	delete Death;
	delete dizziness;
}