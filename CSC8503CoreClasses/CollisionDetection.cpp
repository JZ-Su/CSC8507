#include "CollisionDetection.h"
#include "CollisionVolume.h"
#include "AABBVolume.h"
#include "OBBVolume.h"
#include "SphereVolume.h"
#include "Window.h"
#include "Maths.h"
#include "Debug.h"

using namespace NCL;

bool CollisionDetection::RayPlaneIntersection(const Ray& r, const Plane& p, RayCollision& collisions) {
	float ln = Vector3::Dot(p.GetNormal(), r.GetDirection());

	if (ln == 0.0f) {
		return false; //direction vectors are perpendicular!
	}

	Vector3 planePoint = p.GetPointOnPlane();

	Vector3 pointDir = planePoint - r.GetPosition();

	float d = Vector3::Dot(pointDir, p.GetNormal()) / ln;

	collisions.collidedAt = r.GetPosition() + (r.GetDirection() * d);

	return true;
}

bool CollisionDetection::RayIntersection(const Ray& r, GameObject& object, RayCollision& collision) {
	bool hasCollided = false;

	const Transform& worldTransform = object.GetTransform();
	const CollisionVolume* volume = object.GetBoundingVolume();

	if (!volume) {
		return false;
	}

	switch (volume->type) {
	case VolumeType::AABB:		hasCollided = RayAABBIntersection(r, worldTransform, (const AABBVolume&)*volume, collision); break;
	case VolumeType::OBB:		hasCollided = RayOBBIntersection(r, worldTransform, (const OBBVolume&)*volume, collision); break;
	case VolumeType::Sphere:	hasCollided = RaySphereIntersection(r, worldTransform, (const SphereVolume&)*volume, collision); break;

	case VolumeType::Capsule:	hasCollided = RayCapsuleIntersection(r, worldTransform, (const CapsuleVolume&)*volume, collision); break;
	}

	return hasCollided;
}

bool CollisionDetection::RayBoxIntersection(const Ray& r, const Vector3& boxPos, const Vector3& boxSize, RayCollision& collision) {
	Vector3 boxMin = boxPos - boxSize;
	Vector3 boxMax = boxPos + boxSize;

	Vector3 rayPos = r.GetPosition();
	Vector3 rayDir = r.GetDirection();

	Vector3 tVals(-1, -1, -1);

	for (int i = 0; i < 3; i++) {
		if (rayDir[i] > 0) {
			tVals[i] = (boxMin[i] - rayPos[i]) / rayDir[i];
		}
		else if (rayDir[i] < 0) {
			tVals[i] = (boxMax[i] - rayPos[i]) / rayDir[i];
		}
	}
	float bestT = tVals.GetMaxElement();
	if (bestT < 0.0f) {
		return false;
	}

	Vector3 intersection = rayPos + (rayDir * bestT);
	const float epsilon = 0.0001f; // an amount of leeway in our calcs
	for (int i = 0; i < 3; i++) {
		if (intersection[i] + epsilon < boxMin[i] || intersection[i] - epsilon>boxMax[i]) {
			return false;
		}
	}
	collision.collidedAt = intersection;
	collision.rayDistance = bestT;
	return true;
}

bool CollisionDetection::RayAABBIntersection(const Ray& r, const Transform& worldTransform, const AABBVolume& volume, RayCollision& collision) {
	Vector3 boxPos = worldTransform.GetPosition();
	Vector3 boxSize = volume.GetHalfDimensions();
	return RayBoxIntersection(r, boxPos, boxSize, collision);
}

bool CollisionDetection::RayOBBIntersection(const Ray& r, const Transform& worldTransform, const OBBVolume& volume, RayCollision& collision) {
	Quaternion orientation = worldTransform.GetOrientation();
	Vector3 position = worldTransform.GetPosition();

	Matrix3 transform = Matrix3(orientation);
	Matrix3 invTransform = Matrix3(orientation.Conjugate());

	Vector3 localRayPos = r.GetPosition() - position;

	Ray tempRay(invTransform * localRayPos, invTransform * r.GetDirection());

	bool collided = RayBoxIntersection(tempRay, Vector3(), volume.GetHalfDimensions(), collision);

	if (collided) {
		collision.collidedAt = transform * collision.collidedAt + position;
	}
	return collided;
}

bool CollisionDetection::RaySphereIntersection(const Ray& r, const Transform& worldTransform, const SphereVolume& volume, RayCollision& collision) {
	Vector3 spherePos = worldTransform.GetPosition();
	float sphereRadius = volume.GetRadius();

	//Get the direction between the ray origin and the sphere origin
	Vector3 dir = (spherePos - r.GetPosition());

	//Then project the sphere's origin onto our ray direction vector
	float sphereProj = Vector3::Dot(dir, r.GetDirection());

	if (sphereProj < 0.0f) {
		return false; //point is behind the ray
	}

	//Get closest point on ray line to sphere
	Vector3 point = r.GetPosition() + (r.GetDirection() * sphereProj);

	float sphereDist = (point - spherePos).Length();

	if (sphereDist > sphereRadius) {
		return false;
	}

	float offset = sqrt((sphereRadius * sphereRadius) - (sphereDist * sphereDist));
	collision.rayDistance = sphereProj - offset;
	collision.collidedAt = r.GetPosition() + (r.GetDirection() * collision.rayDistance);
	return true;
}

bool CollisionDetection::RayCapsuleIntersection(const Ray& r, const Transform& worldTransform, const CapsuleVolume& volume, RayCollision& collision) {
	Vector3 capTop = worldTransform.GetPosition() + Vector3(0,  volume.GetHalfHeight() - volume.GetRadius() / 2, 0);
	Vector3 capBot = worldTransform.GetPosition() + Vector3(0, -volume.GetHalfHeight() + volume.GetRadius() / 2, 0);
	float radius = volume.GetRadius() / 2; 

	Vector3 topDir = capTop - r.GetPosition();
	float topProj = Vector3::Dot(topDir, r.GetDirection());
	if (topProj > 0.0f) {
		Vector3 point = r.GetPosition() + (r.GetDirection() * topProj);
		float dist = (point - capTop).Length();
		if (dist < radius) {
			float offset = sqrt((radius * radius) - (dist * dist));
			collision.rayDistance = topProj - offset;
			collision.collidedAt = r.GetPosition() + (r.GetDirection() * collision.rayDistance);
			return true;
		}
	}

	Vector3 botDir = capBot - r.GetPosition();
	float botProj = Vector3::Dot(botDir, r.GetDirection());
	if (botProj > 0.0f) {
		Vector3 point = r.GetPosition() + (r.GetDirection() * botProj);
		float dist = (point - capBot).Length();
		if (dist < radius) {
			float offset = sqrt((radius * radius) - (dist * dist));
			collision.rayDistance = botProj - offset;
			collision.collidedAt = r.GetPosition() + (r.GetDirection() * collision.rayDistance);
			return true;
		}
	}

	return false;
}

bool CollisionDetection::ObjectIntersection(GameObject* a, GameObject* b, CollisionInfo& collisionInfo) {
	const CollisionVolume* volA = a->GetBoundingVolume();
	const CollisionVolume* volB = b->GetBoundingVolume();

	if (!volA || !volB) {
		return false;
	}

	collisionInfo.a = a;
	collisionInfo.b = b;

	Transform& transformA = a->GetTransform();
	Transform& transformB = b->GetTransform();

	VolumeType pairType = (VolumeType)((int)volA->type | (int)volB->type);

	//Two AABBs
	if (pairType == VolumeType::AABB) {
		return AABBIntersection((AABBVolume&)*volA, transformA, (AABBVolume&)*volB, transformB, collisionInfo);
	}
	//Two Spheres
	if (pairType == VolumeType::Sphere) {
		return SphereIntersection((SphereVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
	}
	//Two OBBs
	if (pairType == VolumeType::OBB) {
		return OBBIntersection((OBBVolume&)*volA, transformA, (OBBVolume&)*volB, transformB, collisionInfo);
	}
	//Two Capsules

	//AABB vs Sphere pairs
	if (volA->type == VolumeType::AABB && volB->type == VolumeType::Sphere) {
		return AABBSphereIntersection((AABBVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
	}
	if (volA->type == VolumeType::Sphere && volB->type == VolumeType::AABB) {
		collisionInfo.a = b;
		collisionInfo.b = a;
		return AABBSphereIntersection((AABBVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
	}

	//OBB vs sphere pairs
	if (volA->type == VolumeType::OBB && volB->type == VolumeType::Sphere) {
		return OBBSphereIntersection((OBBVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
	}
	if (volA->type == VolumeType::Sphere && volB->type == VolumeType::OBB) {
		collisionInfo.a = b;
		collisionInfo.b = a;
		return OBBSphereIntersection((OBBVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
	}

	//Capsule vs other interactions
	if (volA->type == VolumeType::Capsule && volB->type == VolumeType::Sphere) {
		return SphereCapsuleIntersection((CapsuleVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
	}
	if (volA->type == VolumeType::Sphere && volB->type == VolumeType::Capsule) {
		collisionInfo.a = b;
		collisionInfo.b = a;
		return SphereCapsuleIntersection((CapsuleVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
	}

	if (volA->type == VolumeType::Capsule && volB->type == VolumeType::AABB) {
		return AABBCapsuleIntersection((CapsuleVolume&)*volA, transformA, (AABBVolume&)*volB, transformB, collisionInfo);
	}
	if (volB->type == VolumeType::Capsule && volA->type == VolumeType::AABB) {
		collisionInfo.a = b;
		collisionInfo.b = a;
		return AABBCapsuleIntersection((CapsuleVolume&)*volB, transformB, (AABBVolume&)*volA, transformA, collisionInfo);
	}

	//AABB vs OBB
	if (volA->type == VolumeType::AABB && volB->type == VolumeType::OBB) {
		return AABBOBBIntersection((AABBVolume&)*volA, transformA, (OBBVolume&)*volB, transformB, collisionInfo);
	}
	if (volA->type == VolumeType::OBB && volB->type == VolumeType::AABB) {
		collisionInfo.a = b;
		collisionInfo.b = a;
		return AABBOBBIntersection((AABBVolume&)*volB, transformB, (OBBVolume&)*volA, transformA, collisionInfo);
	}

	return false;
}

bool CollisionDetection::AABBTest(const Vector3& posA, const Vector3& posB, const Vector3& halfSizeA, const Vector3& halfSizeB) {
	Vector3 delta = posB - posA;
	Vector3 totalSize = halfSizeA + halfSizeB;

	if (abs(delta.x) < totalSize.x &&
		abs(delta.y) < totalSize.y &&
		abs(delta.z) < totalSize.z) {
		return true;
	}
	return false;
}

//AABB/AABB Collisions
bool CollisionDetection::AABBIntersection(const AABBVolume& volumeA, const Transform& worldTransformA,
	const AABBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	Vector3 boxAPos = worldTransformA.GetPosition();
	Vector3 boxBPos = worldTransformB.GetPosition();
	Vector3 boxASize = volumeA.GetHalfDimensions();
	Vector3 boxBSize = volumeB.GetHalfDimensions();

	bool overlap = AABBTest(boxAPos, boxBPos, boxASize, boxBSize);
	if (overlap) {
		static const Vector3 faces[6] = {
			Vector3(-1, 0, 0), Vector3(1, 0, 0),
			Vector3(0, -1, 0), Vector3(0, 1, 0),
			Vector3(0, 0, -1), Vector3(0, 0, 1),
		};

		Vector3 maxA = boxAPos + boxASize;
		Vector3 minA = boxAPos - boxASize;
		Vector3 maxB = boxBPos + boxBSize;
		Vector3 minB = boxBPos - boxBSize;

		float distances[6] = {
			(maxB.x - minA.x), (maxA.x - minB.x),
			(maxB.y - minA.y), (maxA.y - minB.y),
			(maxB.z - minA.z), (maxA.z - minB.z),
		};
		float penetration = FLT_MAX;
		Vector3 bestAxis;

		for (int i = 0; i < 6; i++) {
			if (distances[i] < penetration) {
				penetration = distances[i];
				bestAxis = faces[i];
			}
		}
		collisionInfo.AddContactPoint(Vector3(), Vector3(), bestAxis, penetration);
		return true;
	}
	return false;
}

//Sphere / Sphere Collision
bool CollisionDetection::SphereIntersection(const SphereVolume& volumeA, const Transform& worldTransformA,
	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	float radii = volumeA.GetRadius() + volumeB.GetRadius();
	Vector3 delta = worldTransformB.GetPosition() - worldTransformA.GetPosition();
	float deltaLength = delta.Length();

	if (deltaLength < radii) {
		float penetration = radii - deltaLength;
		Vector3 normal = delta.Normalised();
		Vector3 localA = normal * volumeA.GetRadius();
		Vector3 localB = -normal * volumeB.GetRadius();

		collisionInfo.AddContactPoint(localA, localB, normal, penetration);
		return true;
	}
	return false;
}

//AABB - Sphere Collision
bool CollisionDetection::AABBSphereIntersection(const AABBVolume& volumeA, const Transform& worldTransformA,
	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	Vector3 boxSize = volumeA.GetHalfDimensions();
	Vector3 delta = worldTransformB.GetPosition() - worldTransformA.GetPosition();
	Vector3 closestPointOnBox = Vector3::Clamp(delta, -boxSize, boxSize);
	Vector3 localPoint = delta - closestPointOnBox;
	float distance = localPoint.Length();

	if (distance < volumeB.GetRadius()) {
		Vector3 collisionNormal = localPoint.Normalised();
		float penetration = (volumeB.GetRadius() - distance);

		Vector3 localA = Vector3();
		Vector3 localB = -collisionNormal * volumeB.GetRadius();

		collisionInfo.AddContactPoint(localA, localB, collisionNormal, penetration);
		return true;
	}
	return false;
}

bool  CollisionDetection::OBBSphereIntersection(const OBBVolume& volumeA, const Transform& worldTransformA,
	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	Vector3 boxSize = volumeA.GetHalfDimensions();
	Vector3 boxPos = worldTransformA.GetPosition();
	Matrix4 boxWorldTransform = Matrix4();
	boxWorldTransform.SetPositionVector(boxPos);
	Matrix4 minusTransform = Matrix4();
	minusTransform.SetPositionVector(-boxPos);
	
	Vector3 detectionPoint[27];
	detectionPoint[0] = Vector3(boxPos.x + boxSize.x, boxPos.y + boxSize.y, boxPos.z + boxSize.z);
	detectionPoint[1] = Vector3(boxPos.x + boxSize.x, boxPos.y + boxSize.y, boxPos.z);
	detectionPoint[2] = Vector3(boxPos.x + boxSize.x, boxPos.y + boxSize.y, boxPos.z - boxSize.z);

	detectionPoint[3] = Vector3(boxPos.x + boxSize.x, boxPos.y - boxSize.y, boxPos.z + boxSize.z);
	detectionPoint[4] = Vector3(boxPos.x + boxSize.x, boxPos.y - boxSize.y, boxPos.z);
	detectionPoint[5] = Vector3(boxPos.x + boxSize.x, boxPos.y - boxSize.y, boxPos.z - boxSize.z);

	detectionPoint[6] = Vector3(boxPos.x + boxSize.x, boxPos.y, boxPos.z + boxSize.z);
	detectionPoint[7] = Vector3(boxPos.x + boxSize.x, boxPos.y, boxPos.z);
	detectionPoint[8] = Vector3(boxPos.x + boxSize.x, boxPos.y, boxPos.z - boxSize.z);

	detectionPoint[9]  = Vector3(boxPos.x, boxPos.y + boxSize.y, boxPos.z + boxSize.z);
	detectionPoint[10] = Vector3(boxPos.x, boxPos.y + boxSize.y, boxPos.z);
	detectionPoint[11] = Vector3(boxPos.x, boxPos.y + boxSize.y, boxPos.z - boxSize.z);

	detectionPoint[12] = Vector3(boxPos.x, boxPos.y - boxSize.y, boxPos.z + boxSize.z);
	detectionPoint[13] = Vector3(boxPos.x, boxPos.y - boxSize.y, boxPos.z);
	detectionPoint[14] = Vector3(boxPos.x, boxPos.y - boxSize.y, boxPos.z - boxSize.z);

	detectionPoint[15] = Vector3(boxPos.x, boxPos.y, boxPos.z + boxSize.z);
	detectionPoint[16] = Vector3(boxPos.x, boxPos.y, boxPos.z);
	detectionPoint[17] = Vector3(boxPos.x, boxPos.y, boxPos.z - boxSize.z);

	detectionPoint[18] = Vector3(boxPos.x - boxSize.x, boxPos.y + boxSize.y, boxPos.z + boxSize.z);
	detectionPoint[19] = Vector3(boxPos.x - boxSize.x, boxPos.y + boxSize.y, boxPos.z);
	detectionPoint[20] = Vector3(boxPos.x - boxSize.x, boxPos.y + boxSize.y, boxPos.z - boxSize.z);

	detectionPoint[21] = Vector3(boxPos.x - boxSize.x, boxPos.y - boxSize.y, boxPos.z + boxSize.z);
	detectionPoint[22] = Vector3(boxPos.x - boxSize.x, boxPos.y - boxSize.y, boxPos.z);
	detectionPoint[23] = Vector3(boxPos.x - boxSize.x, boxPos.y - boxSize.y, boxPos.z - boxSize.z);

	detectionPoint[24] = Vector3(boxPos.x - boxSize.x, boxPos.y, boxPos.z + boxSize.z);
	detectionPoint[25] = Vector3(boxPos.x - boxSize.x, boxPos.y, boxPos.z);
	detectionPoint[26] = Vector3(boxPos.x - boxSize.x, boxPos.y, boxPos.z - boxSize.z);

	for (int i = 0; i < 27; i++) {
		detectionPoint[i] = boxWorldTransform * volumeA.GetQuaternion() * minusTransform * detectionPoint[i];
		Vector3 delta = worldTransformB.GetPosition() - detectionPoint[i];
		if (delta.Length() < volumeB.GetRadius()) {
			Vector3 collisionNormal = (worldTransformB.GetPosition() - detectionPoint[i]).Normalised();
			float penetration = volumeB.GetRadius() - (worldTransformB.GetPosition() - detectionPoint[i]).Length();
			Vector3 localA = Vector3();
			Vector3 localB = -collisionNormal * volumeB.GetRadius();
			collisionInfo.AddContactPoint(localA, localB, collisionNormal, penetration);
			return true;
		}
	}
	return false;
}

bool CollisionDetection::AABBCapsuleIntersection(
	const CapsuleVolume& volumeA, const Transform& worldTransformA,
	const AABBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	Vector3 boxSize = volumeB.GetHalfDimensions();

	Vector3 capsuleTop = worldTransformA.GetPosition() + Vector3(0, volumeA.GetHalfHeight() - volumeA.GetRadius() / 2, 0);
	Vector3 deltaTop = capsuleTop - worldTransformB.GetPosition();
	Vector3 closestPointOnBoxByTop = Vector3::Clamp(deltaTop, -boxSize, boxSize);
	Vector3 localPointByTop = deltaTop - closestPointOnBoxByTop;
	float distanceByTop = localPointByTop.Length();
	if (distanceByTop < volumeA.GetRadius() / 2) {
		Vector3 collisionNormal = localPointByTop.Normalised();
		float penetration = volumeA.GetRadius() / 2 - distanceByTop;
		Vector3 localA = Vector3();
		Vector3 localB = -collisionNormal * volumeA.GetRadius() / 2;
		collisionInfo.AddContactPoint(localA, localB, collisionNormal, penetration);
		return true;
	}

	Vector3 capsuleBot = worldTransformA.GetPosition() + Vector3(0, -volumeA.GetHalfHeight() + volumeA.GetRadius() / 2, 0);
	Vector3 deltaBot = capsuleBot - worldTransformB.GetPosition();
	Vector3 closestPointOnBoxByBot = Vector3::Clamp(deltaBot, -boxSize, boxSize);
	Vector3 localPointByBot = deltaBot - closestPointOnBoxByBot;
	float distanceByBot = localPointByBot.Length();
	if (distanceByBot < volumeA.GetRadius() / 2) { 
		Vector3 collisionNormal = localPointByBot.Normalised(); 
		float penetration = -(volumeA.GetRadius() / 2 - distanceByBot);
		Vector3 localA = Vector3();
		Vector3 localB = collisionNormal * volumeA.GetRadius() / 2;
		collisionInfo.AddContactPoint(localA, localB, collisionNormal, penetration);
		return true;
	}

	return false;
}

bool CollisionDetection::SphereCapsuleIntersection(
	const CapsuleVolume& volumeA, const Transform& worldTransformA,
	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {
	
	float radii = volumeA.GetRadius() + volumeB.GetRadius();

	Vector3 capsuleTop = worldTransformA.GetMatrix() * Vector3(0, volumeA.GetHalfHeight(), 0);
	Vector3 deltaTop = capsuleTop - worldTransformB.GetPosition();
	float topLength = deltaTop.Length();
	if (topLength < radii) {
		float penetration = radii - topLength;
		Vector3 normal = deltaTop.Normalised();
		Vector3 localA = normal * volumeA.GetRadius();
		Vector3 localB = normal * volumeB.GetRadius();
		collisionInfo.AddContactPoint(localA, localB, normal, penetration);
		return true;
	}

	Vector3 capsuleBot = worldTransformA.GetMatrix() * Vector3(0, -volumeA.GetHalfHeight(), 0);
	Vector3 deltaBot = capsuleBot - worldTransformB.GetPosition();
	float botLength = deltaBot.Length();
	if (botLength < radii) {
		float penetration = radii - botLength;
		Vector3 normal = deltaBot.Normalised();
		Vector3 localA = normal * volumeA.GetRadius();
		Vector3 localB = normal * volumeB.GetRadius();
		collisionInfo.AddContactPoint(localA, localB, normal, penetration);
		return true;
	}
	return false;
}

bool CollisionDetection::AABBOBBIntersection(const AABBVolume& volumeA, const Transform& worldTransformA,
	const OBBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	Vector3 relativePosition = worldTransformB.GetPosition() - worldTransformA.GetPosition();
	Vector3 AFor = worldTransformA.GetOrientation() * Vector3(0, 0, 1);
	Vector3 ARight = worldTransformA.GetOrientation() * Vector3(1, 0, 0);
	Vector3 AUp = worldTransformA.GetOrientation() * Vector3(0, 1, 0);

	Vector3 BFor = worldTransformB.GetOrientation() * Vector3(0, 0, 1);
	Vector3 BRight = worldTransformB.GetOrientation() * Vector3(1, 0, 0);
	Vector3 BUp = worldTransformB.GetOrientation() * Vector3(0, 1, 0);

	Vector3 boxASize = volumeA.GetHalfDimensions();
	Vector3 boxBSize = volumeB.GetHalfDimensions();

	vector<float> deltas;

	bool seperationPlane[15] = { false };

	float penDistance = FLT_MAX;
	Vector3 normal;
	Vector3 pointA;
	Vector3 pointB;
	Vector3 planes[15] = {
		// 6 faces
		ARight.Normalised(),
		AUp.Normalised(),
		AFor.Normalised(),
		BRight.Normalised(),
		BUp.Normalised(),
		BFor.Normalised(),

		//9 possible edges
		Vector3::Cross(ARight, BRight).Normalised(),
		Vector3::Cross(ARight, BUp).Normalised(),
		Vector3::Cross(ARight, BFor).Normalised(),
		Vector3::Cross(AUp, BRight).Normalised(),
		Vector3::Cross(AUp, BUp).Normalised(),
		Vector3::Cross(AUp, BFor).Normalised(),
		Vector3::Cross(AFor, BRight).Normalised(),
		Vector3::Cross(AFor, BUp).Normalised(),
		Vector3::Cross(AFor, BFor).Normalised()
	};
	for (int i = 0; i < 15; i++)
	{
		if (!useSAT(relativePosition, planes[i], worldTransformA, worldTransformB, volumeA.GetHalfDimensions(), volumeB.GetHalfDimensions(), penDistance, normal, pointA, pointB))
		{
			return false;
		}
	}
	collisionInfo.AddContactPoint(pointA, pointB, normal, penDistance);
	return true;

}

bool CollisionDetection::OBBIntersection(const OBBVolume& volumeA, const Transform& worldTransformA,
	const OBBVolume& volumeB, const Transform& worldTransformB, NCL::CollisionDetection::CollisionInfo& collisionInfo) {

	Vector3 relativePosition = worldTransformB.GetPosition() - worldTransformA.GetPosition();
	Vector3 AFor = worldTransformA.GetOrientation() * Vector3(0, 0, 1);
	Vector3 ARight = worldTransformA.GetOrientation() * Vector3(1, 0, 0);
	Vector3 AUp = worldTransformA.GetOrientation() * Vector3(0, 1, 0);

	Vector3 BFor = worldTransformB.GetOrientation() * Vector3(0, 0, 1);
	Vector3 BRight = worldTransformB.GetOrientation() * Vector3(1, 0, 0);
	Vector3 BUp = worldTransformB.GetOrientation() * Vector3(0, 1, 0);

	Vector3 boxASize = volumeA.GetHalfDimensions();
	Vector3 boxBSize = volumeB.GetHalfDimensions();

	vector<float> deltas;

	bool seperationPlane[15] = { false };

	float penDistance = FLT_MAX;
	Vector3 normal;
	Vector3 pointA;
	Vector3 pointB;
	Vector3 planes[15] = {
		// 6 faces
		ARight.Normalised(),
		AUp.Normalised(),
		AFor.Normalised(),
		BRight.Normalised(),
		BUp.Normalised(),
		BFor.Normalised(),

		//9 possible edges
		Vector3::Cross(ARight, BRight).Normalised(),
		Vector3::Cross(ARight, BUp).Normalised(),
		Vector3::Cross(ARight, BFor).Normalised(),
		Vector3::Cross(AUp, BRight).Normalised(),
		Vector3::Cross(AUp, BUp).Normalised(),
		Vector3::Cross(AUp, BFor).Normalised(),
		Vector3::Cross(AFor, BRight).Normalised(),
		Vector3::Cross(AFor, BUp).Normalised(),
		Vector3::Cross(AFor, BFor).Normalised()
	};
	for (int i = 0; i < 15; i++)
	{
		if (!useSAT(relativePosition, planes[i], worldTransformA, worldTransformB, volumeA.GetHalfDimensions(), volumeB.GetHalfDimensions(), penDistance, normal, pointA, pointB))
		{
			return false;
		}
	}
	collisionInfo.AddContactPoint(pointA, pointB, normal, penDistance);
	return true;

}
bool CollisionDetection::useSAT(const Vector3 delta, Vector3 plane, const Transform& worldTransformA, const Transform& worldTransformB,
	const Vector3 halfSizeA, const Vector3 halfSizeB, float& penetrationDistance, Vector3& normal, Vector3& pointA, Vector3& pointB) {
	Vector3 AFor = worldTransformA.GetOrientation() * Vector3(0, 0, 1);
	Vector3 ARight = worldTransformA.GetOrientation() * Vector3(1, 0, 0);
	Vector3 AUp = worldTransformA.GetOrientation() * Vector3(0, 1, 0);
	Vector3 BFor = worldTransformB.GetOrientation() * Vector3(0, 0, 1);
	Vector3 BRight = worldTransformB.GetOrientation() * Vector3(1, 0, 0);
	Vector3 BUp = worldTransformB.GetOrientation() * Vector3(0, 1, 0);
	float deltaPlaneDot = fabs(Vector3::Dot(delta, plane));
	float boxVerticesDot =
		fabs(Vector3::Dot(ARight * halfSizeA.x, plane)) +
		fabs(Vector3::Dot(AUp * halfSizeA.y, plane)) +
		fabs(Vector3::Dot(AFor * halfSizeA.z, plane)) +
		fabs(Vector3::Dot(BRight * halfSizeB.x, plane)) +
		fabs(Vector3::Dot(BUp * halfSizeB.y, plane)) +
		fabs(Vector3::Dot(BFor * halfSizeB.z, plane));

	bool result = deltaPlaneDot <= boxVerticesDot;
	if (result) {
		float delta2 = boxVerticesDot - deltaPlaneDot;
		if (delta2 < penetrationDistance && plane.LengthSquared() > 0) {
			penetrationDistance = delta2;

			if (Vector3::Dot(plane, delta) < 0) { plane *= -1; }

			normal = plane;
			pointA = OBBSupport(worldTransformA, plane);
			pointB = OBBSupport(worldTransformB, -plane);
		}
		return true;
	}
	return false;
}
Vector3 CollisionDetection::OBBSupport(const Transform& worldTransform, Vector3 worldDir) {
	Vector3 vertex;
	Vector3 localDirection = worldTransform.GetOrientation().Conjugate() * worldDir;

	vertex.x = localDirection.x < 0 ? -0.5f : 0.5f;
	vertex.y = localDirection.y < 0 ? -0.5f : 0.5f;
	vertex.z = localDirection.z < 0 ? -0.5f : 0.5f;

	return (Matrix4::Scale(worldTransform.GetScale()) * worldTransform.GetOrientation() *
		Matrix4::Translation(vertex)).GetPositionVector();
}

Matrix4 GenerateInverseView(const Camera& c) {
	float pitch = c.GetPitch();
	float yaw = c.GetYaw();
	Vector3 position = c.GetPosition();

	Matrix4 iview =
		Matrix4::Translation(position) *
		Matrix4::Rotation(-yaw, Vector3(0, -1, 0)) *
		Matrix4::Rotation(-pitch, Vector3(-1, 0, 0));

	return iview;
}

Matrix4 GenerateInverseProjection(float aspect, float nearPlane, float farPlane, float fov) {
	float negDepth = nearPlane - farPlane;

	float invNegDepth = negDepth / (2 * (farPlane * nearPlane));

	Matrix4 m;

	float h = 1.0f / tan(fov * PI_OVER_360);

	m.array[0][0] = aspect / h;
	m.array[1][1] = tan(fov * PI_OVER_360);
	m.array[2][2] = 0.0f;

	m.array[2][3] = invNegDepth;//// +PI_OVER_360;
	m.array[3][2] = -1.0f;
	m.array[3][3] = (0.5f / nearPlane) + (0.5f / farPlane);

	return m;
}

Vector3 CollisionDetection::Unproject(const Vector3& screenPos, const PerspectiveCamera& cam) {
	Vector2i screenSize = Window::GetWindow()->GetScreenSize();

	float aspect = (float)screenSize.x / (float)screenSize.y;
	float fov = cam.GetFieldOfVision();
	float nearPlane = cam.GetNearPlane();
	float farPlane = cam.GetFarPlane();

	//Create our inverted matrix! Note how that to get a correct inverse matrix,
	//the order of matrices used to form it are inverted, too.
	Matrix4 invVP = GenerateInverseView(cam) * GenerateInverseProjection(aspect, fov, nearPlane, farPlane);

	Matrix4 proj = cam.BuildProjectionMatrix(aspect);

	//Our mouse position x and y values are in 0 to screen dimensions range,
	//so we need to turn them into the -1 to 1 axis range of clip space.
	//We can do that by dividing the mouse values by the width and height of the
	//screen (giving us a range of 0.0 to 1.0), multiplying by 2 (0.0 to 2.0)
	//and then subtracting 1 (-1.0 to 1.0).
	Vector4 clipSpace = Vector4(
		(screenPos.x / (float)screenSize.x) * 2.0f - 1.0f,
		(screenPos.y / (float)screenSize.y) * 2.0f - 1.0f,
		(screenPos.z),
		1.0f
	);

	//Then, we multiply our clipspace coordinate by our inverted matrix
	Vector4 transformed = invVP * clipSpace;

	//our transformed w coordinate is now the 'inverse' perspective divide, so
	//we can reconstruct the final world space by dividing x,y,and z by w.
	return Vector3(transformed.x / transformed.w, transformed.y / transformed.w, transformed.z / transformed.w);
}

Ray CollisionDetection::BuildRayFromMouse(const PerspectiveCamera& cam) {
	Vector2 screenMouse = Window::GetMouse()->GetAbsolutePosition();
	Vector2i screenSize = Window::GetWindow()->GetScreenSize();

	//We remove the y axis mouse position from height as OpenGL is 'upside down',
	//and thinks the bottom left is the origin, instead of the top left!
	Vector3 nearPos = Vector3(screenMouse.x,
		screenSize.y - screenMouse.y,
		-0.99999f
	);

	//We also don't use exactly 1.0 (the normalised 'end' of the far plane) as this
	//causes the unproject function to go a bit weird. 
	Vector3 farPos = Vector3(screenMouse.x,
		screenSize.y - screenMouse.y,
		0.99999f
	);

	Vector3 a = Unproject(nearPos, cam);
	Vector3 b = Unproject(farPos, cam);
	Vector3 c = b - a;

	c.Normalise();

	return Ray(cam.GetPosition(), c);
}

Ray CollisionDetection::BuildRayFromScreenCenter(const PerspectiveCamera& c) {
	Vector2i screenSize = Window::GetWindow()->GetScreenSize();

	Vector3 nearPos = Vector3(0.5 * screenSize.x, 0.5 * screenSize.y, -0.99999f);
	Vector3 farPos  = Vector3(0.5 * screenSize.x, 0.5 * screenSize.y,  0.99999f);

	Vector3 a = Unproject(nearPos, c);
	Vector3 b = Unproject(farPos, c);
	Vector3 v = b - a;

	v.Normalise();

	return Ray(c.GetPosition(), v);
}

//http://bookofhook.com/mousepick.pdf
Matrix4 CollisionDetection::GenerateInverseProjection(float aspect, float fov, float nearPlane, float farPlane) {
	Matrix4 m;

	float t = tan(fov * PI_OVER_360);

	float neg_depth = nearPlane - farPlane;

	const float h = 1.0f / t;

	float c = (farPlane + nearPlane) / neg_depth;
	float e = -1.0f;
	float d = 2.0f * (nearPlane * farPlane) / neg_depth;

	m.array[0][0] = aspect / h;
	m.array[1][1] = tan(fov * PI_OVER_360);
	m.array[2][2] = 0.0f;

	m.array[2][3] = 1.0f / d;

	m.array[3][2] = 1.0f / e;
	m.array[3][3] = -c / (d * e);

	return m;
}

/*
And here's how we generate an inverse view matrix. It's pretty much
an exact inversion of the BuildViewMatrix function of the Camera class!
*/
Matrix4 CollisionDetection::GenerateInverseView(const Camera& c) {
	float pitch = c.GetPitch();
	float yaw = c.GetYaw();
	Vector3 position = c.GetPosition();

	Matrix4 iview =
		Matrix4::Translation(position) *
		Matrix4::Rotation(yaw, Vector3(0, 1, 0)) *
		Matrix4::Rotation(pitch, Vector3(1, 0, 0));

	return iview;
}


/*
If you've read through the Deferred Rendering tutorial you should have a pretty
good idea what this function does. It takes a 2D position, such as the mouse
position, and 'unprojects' it, to generate a 3D world space position for it.

Just as we turn a world space position into a clip space position by multiplying
it by the model, view, and projection matrices, we can turn a clip space
position back to a 3D position by multiply it by the INVERSE of the
view projection matrix (the model matrix has already been assumed to have
'transformed' the 2D point). As has been mentioned a few times, inverting a
matrix is not a nice operation, either to understand or code. But! We can cheat
the inversion process again, just like we do when we create a view matrix using
the camera.

So, to form the inverted matrix, we need the aspect and fov used to create the
projection matrix of our scene, and the camera used to form the view matrix.

*/
Vector3	CollisionDetection::UnprojectScreenPosition(Vector3 position, float aspect, float fov, const PerspectiveCamera& c) {
	//Create our inverted matrix! Note how that to get a correct inverse matrix,
	//the order of matrices used to form it are inverted, too.
	Matrix4 invVP = GenerateInverseView(c) * GenerateInverseProjection(aspect, fov, c.GetNearPlane(), c.GetFarPlane());

	Vector2i screenSize = Window::GetWindow()->GetScreenSize();

	//Our mouse position x and y values are in 0 to screen dimensions range,
	//so we need to turn them into the -1 to 1 axis range of clip space.
	//We can do that by dividing the mouse values by the width and height of the
	//screen (giving us a range of 0.0 to 1.0), multiplying by 2 (0.0 to 2.0)
	//and then subtracting 1 (-1.0 to 1.0).
	Vector4 clipSpace = Vector4(
		(position.x / (float)screenSize.x) * 2.0f - 1.0f,
		(position.y / (float)screenSize.y) * 2.0f - 1.0f,
		(position.z) - 1.0f,
		1.0f
	);

	//Then, we multiply our clipspace coordinate by our inverted matrix
	Vector4 transformed = invVP * clipSpace;

	//our transformed w coordinate is now the 'inverse' perspective divide, so
	//we can reconstruct the final world space by dividing x,y,and z by w.
	return Vector3(transformed.x / transformed.w, transformed.y / transformed.w, transformed.z / transformed.w);
}

