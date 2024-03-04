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
				std::cout << "idle.\n";
				return Ongoing;
			}
		}
		return state;//will be ongoing until success or condition to switch
		}
	);

	ChaseAndAttack = new BehaviourAction("ChaseAndAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		float playerHealth = this->player->GetHealth();
		float bossHealth = this->getBossHealth();
		Vector3 targetPosition;
		Vector3 bossPosition;
		chaseSpeed = 5.0f;
		attackRange = 20.0f;
		chaseRange = 80.0f;
		static float lastAttackTime = 0.0f;
		static float attackCooldown = 4.0f;
		targetPosition = this->player->GetTransform().GetPosition();
		bossPosition = this->GetTransform().GetPosition();
		float distanceToPlayer = calculateDistance(targetPosition, bossPosition);
		Vector3 direction = (targetPosition - bossPosition).Normalised();
		if (distanceToPlayer > chaseRange) {
			std::cout << "Player has escaped.\n";
			return Failure;
		}
		if (playerHealth <= 0) {
			std::cout << "Player health depleted.\n";
			return Success;
		}
		if (bossHealth <= 0) {
			std::cout << "Boss health depleted.\n";
			return Failure;
		}
		if ((distanceToPlayer <= attackRange) && (lastAttackTime >= attackCooldown)) {
			isAttack = true;
			lastAttackTime = 0.0f;
			Debug::DrawLine(bossPosition, bossPosition + direction * attackRange, Debug::RED);
		}
		else if ((distanceToPlayer <= chaseRange) && (distanceToPlayer >= attackRange)) {
			Quaternion targetOrientation = Quaternion::AxisAngleToQuaterion(Vector3(0, -1, 0), Maths::RadiansToDegrees(atan2(direction.x, -direction.z)));
			this->GetTransform().SetOrientation(targetOrientation);
			bossPosition += direction * chaseSpeed * dt;
			this->GetTransform().SetPosition(bossPosition);
			Debug::DrawLine(bossPosition, this->player->GetTransform().GetPosition(),Debug::GREEN);
			std::cout << "Chasing player...\n";
			std::cout << distanceToPlayer << std::endl;
		}
		lastAttackTime += dt;
		std::cout << "last attack time: " << lastAttackTime << std::endl;
		return Ongoing;
		});
	RemoteAttack = new BehaviourAction("RemoteAttack", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			//std::cout << "RAttacking init.\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			this->distanceToTarget = calculateDistance(this->GetTransform().GetPosition(), this->player->GetTransform().GetPosition());
			isShooting = false;
			//if (this->distanceToTarget > this->remoteAttackRange || this->distanceToTarget < this->chaseRange || this->getIsRencentlyHurt())
			if (this->distanceToTarget > this->remoteAttackRange) {
				return Failure;
			}
			else if (this->distanceToTarget <= this->remoteAttackRange && this->distanceToTarget >=chaseRange ) {
				Debug::DrawLine(GetTransform().GetPosition(), this->player->GetTransform().GetPosition(), Debug::RED);
				Debug::DrawCollisionBox(this);
				Debug::DrawCollisionBox(this->player);
				std::cout << "attacking----" << this->distanceToTarget << std::endl;
				isShooting = true;
				return Success;
			}
		}
		return state;
		}
	);
	Inverter* antiRemoteAttack = new Inverter("antiRemoteAttack", RemoteAttack);
	Flinches = new BehaviourAction("Flinches", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "Flinches init.\n";
			state = Ongoing;
			flinchAnimationTimer = 0.0f;
		}
		else if (state == Ongoing) {
			if (this->getIsRencentlyHurt()) {
				std::cout << "Flinches.\n";
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
		static float stunTimer = 0.0f;
		const float stunDuration = 4.0f;
		const float healthThreshold = 50.0f;
		if (bossHealth <= healthThreshold) {
			if (state == Initialise) {
				std::cout << "dizziness init.\n";
				stunTimer = 0.0f;
				return Ongoing;
			}
			else if (state == Ongoing) {
				std::cout << "dizziness.\n";
				stunTimer += dt;
				if (stunTimer >= stunDuration) {
					std::cout << "End of dizziness.\n";
					stunTimer = 0.0f;
					return Success;
				}
				return Ongoing;
			}
		}
		return Success;
		}
	);
	Inverter* antidizziness = new Inverter("antidizziness", dizziness);
	Death = new BehaviourAction("Death", [&](float dt, BehaviourState state) -> BehaviourState {
		if (state == Initialise) {
			std::cout << "Death init\n";
			state = Ongoing;
		}
		else if (state == Ongoing) {
			if (bossHealth <= 0) {
				std::cout << "Death.\n";
				// handleDeath();
				/*PlayDeathAnimation();*/
				//RemoveBossObject();
				return Success;
			}
			else if (bossHealth > 0) {
				std::cout << "boss live.\n";
				return Failure;
			}
		}
		return state;
		}
	);
	//BehaviourSelector* selection = new BehaviourSelector("FirstLevel");
	//selection->AddChild(Flinches);
	//selection->AddChild(RemoteAttack);
	//selection->AddChild(MeleeAttack);
	//selection->AddChild(Idle);
	////	selection->AddChild(Death);
	//rootSequence = new BehaviourSequence("Root Sequence");
	//rootSequence->AddChild(selection);
	Root = new BehaviourSelector("Root");
	Combat = new BehaviourParallel("Combat");
	Combat->AddChild(dizziness);
	Combat->AddChild(antiRemoteAttack);
	Combat->AddChild(ChaseAndAttack);
	Root->AddChild(Death);
	Root->AddChild(Combat);
	Root->AddChild(Idle);
}


void Boss::Update(float dt) {
	/*if (rootSequence != nullptr) {
		rootSequence->Execute(dt);
	}*/
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
}