#include "Transform.h"

using namespace NCL::CSC8503;

Transform::Transform()	{
	scale = Vector3(1, 1, 1);
	collisionOffset		= Vector3();
	collisionDimensions	= Vector3();
	rotationCenter		= Vector3();
}

Transform::~Transform()	{

}

void Transform::UpdateMatrix() {
	matrix =
		Matrix4::Translation(position) *
		Matrix4::Translation(rotationCenter) *	// then reset the offset
		Matrix4(orientation) *
		Matrix4::Translation(-rotationCenter) *	// move it to the origin first
		Matrix4::Scale(scale);
}

Transform& Transform::SetPosition(const Vector3& worldPos) {
	position = worldPos;
	UpdateMatrix();
	return *this;
}

Transform& Transform::SetScale(const Vector3& worldScale) {
	scale = worldScale;
	UpdateMatrix();
	return *this;
}

Transform& Transform::SetOrientation(const Quaternion& worldOrientation) {
	orientation = worldOrientation;
	UpdateMatrix();
	return *this;
}

Transform& Transform::SetCollisionOffset(const Vector3& newOff) {
	collisionOffset = newOff;
	return *this;
}

Transform& Transform::SetCollisionDimensions(const Vector3& newDi) {
	collisionDimensions = newDi;
	return *this;
}

Transform& Transform::SetRotationCenter(const Vector3& newOr) {
	rotationCenter = newOr;
	UpdateMatrix();
	return *this;
}

Matrix4 Transform::GetOffsetMatrix() const {
	Matrix4 mat =
		Matrix4::Translation(position + collisionOffset) *
		Matrix4(orientation) *
		Matrix4::Scale(scale);

	return mat;
}