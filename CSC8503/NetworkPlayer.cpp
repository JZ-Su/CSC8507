#include "NetworkPlayer.h"
#include "NetworkedGame.h"

using namespace NCL;
using namespace CSC8503;

NetworkPlayer::NetworkPlayer(NetworkedGame* game, int num)	{
	this->game = game;
	playerNum  = num;
}

NetworkPlayer::~NetworkPlayer()	{

}

void NetworkPlayer::OnCollisionBegin(GameObject* otherObject) {
	if (game) {
		if (dynamic_cast<NetworkPlayer*>(otherObject))
		{
			game->OnPlayerCollision(this, (NetworkPlayer*)otherObject);
		}
	}
}

Quaternion GenerateOrientation(const Vector3& axis, float angle)
{
	float halfAngle = angle / 2;
	float s = std::sin(halfAngle);
	float w = std::cos(halfAngle);
	float x = axis.x * s;
	float y = axis.y * s;
	float z = axis.z * s;
	return Quaternion(x, y, z, w);
}

void NetworkPlayer::SetPlayerYaw(const Vector3& pointPos)
{
	Quaternion orientation;
	Vector3 pos = transform.GetPosition();
	Vector3 targetForwardVec = (pointPos - pos);
	targetForwardVec.y = 0;
	targetForwardVec = targetForwardVec.Normalised();

	Vector3 forward = Vector3(0, 0, -1);

	float cosTheta = Vector3::Dot(forward, targetForwardVec);
	Vector3 rotationAxis;
	float angle;
	if (cosTheta < -1 + 0.001f)
	{
		rotationAxis = Vector3::Cross(Vector3(0, 0, 1), forward);
		if (rotationAxis.Length() < 0.01)
		{
			rotationAxis = Vector3::Cross(Vector3(1, 0, 0), forward);
		}
		rotationAxis = rotationAxis.Normalised();
		angle = 3.1415926f;
	}
	else
	{
		rotationAxis = Vector3::Cross(forward, targetForwardVec);
		rotationAxis = rotationAxis.Normalised();
		angle = std::acos(cosTheta);
	}
	orientation = GenerateOrientation(rotationAxis, angle);

	transform.SetOrientation(orientation);

	//Debug::DrawLine(pos, pos + forward * 30, Debug::MAGENTA);
}