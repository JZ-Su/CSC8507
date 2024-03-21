#pragma once
#include "Transform.h"
#include "CollisionVolume.h"

using std::vector;

namespace NCL::CSC8503 {
	class NetworkObject;
	class RenderObject;
	class PhysicsObject;

	class GameObject	{
	public:
		GameObject(const std::string& name = "");
		~GameObject();

		void SetBoundingVolume(CollisionVolume* vol) {
			boundingVolume = vol;
		}

		const CollisionVolume* GetBoundingVolume() const {
			return boundingVolume;
		}

		bool IsActive() const {
			return isActive;
		}
		void Deactivate()  {
			isActive = false;
		}

		void Activate() {
			isActive = true;
		}

		Transform& GetTransform() {
			return transform;
		}

		RenderObject* GetRenderObject() const {
			return renderObject;
		}

		PhysicsObject* GetPhysicsObject() const {
 			return physicsObject;
		}

		NetworkObject* GetNetworkObject() const {
			return networkObject;
		}

		void SetRenderObject(RenderObject* newObject) {
			renderObject = newObject;
		}

		void SetPhysicsObject(PhysicsObject* newObject) {
			physicsObject = newObject;
		}
		void SetNetworkObject(NetworkObject* newObject) {
			networkObject = newObject;
		}

		const std::string& GetName() const {
			return name;
		}
		const std::string& GetTag() const {
			return tag;
		}

		void SetTag(std::string inTag) {
			tag = inTag;
		}
		const std::string& GetLayer() const {
			return layer;
		}

		void SetLayer(std::string inLayer) {
			layer = inLayer;
		}

		virtual void OnCollisionBegin(GameObject* otherObject) {
			//std::cout << "OnCollisionBegin event occured!\n";
		}

		virtual void OnCollisionEnd(GameObject* otherObject) {
			//std::cout << "OnCollisionEnd event occured!\n";
		}

		bool GetBroadphaseAABB(Vector3&outsize) const;

		void UpdateBroadphaseAABB();

		void SetWorldID(int newID) {
			worldID = newID;
		}

		int	GetWorldID() const {
			return worldID;
		}

		void SetIsHiding(bool IsHiding, Vector3 position) {
			isHiding = IsHiding;
			this->GetTransform().SetPosition(position);
		}

		void SetIsNotHiding() {
			isHiding = false;
		}

		bool GetIsHiding() const {
			return isHiding;
		}

		void SetIsBolckBack(bool IsBlockBack) {
			isBlockBack = IsBlockBack;
		}

		bool GetHurtBossAlready() const {
			return hurtBossAlready;
		}

		void SetHurtBossAlready(bool HurtBossAlready) {
			hurtBossAlready = HurtBossAlready;
		}

		bool GetIsBlockBack() const {
			return isBlockBack;
		}
		void SetExistenceTime(float ExistenceTime) {
			existenceTime = ExistenceTime;
		}

		float GetExistenceTime() const {
			return existenceTime;
		}
		void UpdateExistenceTime(float dt) {
			existenceTime += dt;
		}

		bool isEnable;

		void SetCollisionResponse(bool b) { hasCollision = b; }
		bool GetCollisionResponse() const { return hasCollision; }

	protected:
		Transform			transform;

		CollisionVolume*	boundingVolume;
		PhysicsObject*		physicsObject;
		RenderObject*		renderObject;
		NetworkObject*		networkObject;

		bool		isActive;
		int			worldID;
		float		existenceTime;
		std::string	name;
		std::string	tag;
		std::string	layer;

		Vector3 broadphaseAABB;

		bool hasCollision;
		bool isHiding;
		bool isBlockBack;
		bool hurtBossAlready=false;
	};
}

