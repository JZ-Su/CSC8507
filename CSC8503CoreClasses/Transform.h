#pragma once

using std::vector;

using namespace NCL::Maths;

namespace NCL {
	namespace CSC8503 {
		class Transform
		{
		public:
			Transform();
			~Transform();

			Transform& SetPosition(const Vector3& worldPos);
			Transform& SetScale(const Vector3& worldScale);
			Transform& SetOrientation(const Quaternion& newOr);

			// set the offset of the collision volume, when the model and it cannot overlap
			Transform& SetCollisionOffset(const Vector3& newOff);

			// set the collision volume, when it is different from the scale
			Transform& SetCollisionDimensions(const Vector3& newDi);

			// set the center of rotation
			Transform& SetRotationCenter(const Vector3& newCen); 

			Vector3 GetPosition() const {
				return position;
			}

			Vector3 GetScale() const {
				return scale;
			}

			Quaternion GetOrientation() const {
				return orientation;
			}

			Vector3 GetRotationCenter() const {
				return rotationCenter;
			}

			Vector3 GetCollisionOffset() const {
				return collisionOffset;
			}

			Vector3 GetCollisionDimensions() const {
				return collisionDimensions;
			}

			// return a matrix without the collision offset
			Matrix4 GetMatrix() const {
				return matrix;
			}

			// return a matrix which is calculated with the collision offset already
			Matrix4 GetOffsetMatrix() const;

			void UpdateMatrix();

		protected:
			Matrix4		matrix;
			Quaternion	orientation;
			Vector3		position;

			Vector3		scale;

			Vector3		collisionOffset;
			Vector3		collisionDimensions;

			Vector3		rotationCenter;
		};
	}
}

