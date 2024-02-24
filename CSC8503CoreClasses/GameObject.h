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

		bool isEnable = true;

		void SetCollisionResponse(bool b) { hasCollision = b; }
		bool GetCollisionResponse() const { return hasCollision; }

		void SetDefaultPos(Vector3 pos) { defaultPosition = pos; }
		Vector3 GetDefaultPos() const { return defaultPosition; }


		void SetVolumeSize(const Vector3& size) { volumeSize = size; }
		Vector3 GetVolumeSize() const { return volumeSize; }

	protected:
		Transform			transform;

		CollisionVolume*	boundingVolume;
		PhysicsObject*		physicsObject;
		RenderObject*		renderObject;
		NetworkObject*		networkObject;

		bool		isActive;
		int			worldID;
		std::string	name;
		std::string	tag;
		std::string	layer;

		Vector3 broadphaseAABB;

		bool hasCollision;

		Vector3 center;
		Vector3 volumeSize;
		Vector3 defaultPosition;
	};
}

