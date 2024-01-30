#pragma once
#include "CollisionVolume.h"
#include "Matrix4.h"

namespace NCL {
	class OBBVolume : CollisionVolume
	{
	public:
		OBBVolume(const Maths::Vector3& halfDims, float pit = 0, float y = 0, float ro = 0) {
			type		= VolumeType::OBB;
			halfSizes	= halfDims;
			pitch       = pit;
			yaw         = y;
			roll        = ro;
		}
		~OBBVolume() {}

		Maths::Vector3 GetHalfDimensions() const {
			return halfSizes;
		}

		float GetPitch() const { return pitch; }
		float GetYaw()   const { return yaw; }
		float GetRoll()  const { return roll; }

		Maths::Quaternion GetQuaternion() const {
			return Maths::Quaternion::EulerAnglesToQuaternion(pitch, yaw, roll);
		}

	protected:
		Maths::Vector3 halfSizes;
		float pitch, yaw, roll;
	};
}

