#include "Debug.h"
#include "Transform.h"
using namespace NCL;

std::vector<Debug::DebugStringEntry>	Debug::stringEntries;
std::vector<Debug::DebugLineEntry>		Debug::lineEntries;

SimpleFont* Debug::debugFont = nullptr;

const Vector4 Debug::RED = Vector4(1, 0, 0, 1);
const Vector4 Debug::GREEN = Vector4(0, 1, 0, 1);
const Vector4 Debug::BLUE = Vector4(0, 0, 1, 1);

const Vector4 Debug::BLACK = Vector4(0, 0, 0, 1);
const Vector4 Debug::WHITE = Vector4(1, 1, 1, 1);

const Vector4 Debug::YELLOW = Vector4(1, 1, 0, 1);
const Vector4 Debug::MAGENTA = Vector4(1, 0, 1, 1);
const Vector4 Debug::CYAN = Vector4(0, 1, 1, 1);

void Debug::Print(const std::string& text, const Vector2& pos, const Vector4& colour) {
	DebugStringEntry newEntry;

	newEntry.data = text;
	newEntry.position = pos;
	newEntry.colour = colour;

	stringEntries.emplace_back(newEntry);
}

void Debug::DrawLine(const Vector3& startpoint, const Vector3& endpoint, const Vector4& colour, float time) {
	DebugLineEntry newEntry;

	newEntry.start = startpoint;
	newEntry.end = endpoint;
	newEntry.colourA = colour;
	newEntry.colourB = colour;
	newEntry.time = time;

	lineEntries.emplace_back(newEntry);
}

void Debug::DrawAxisLines(const Matrix4& modelMatrix, float scaleBoost, float time) {
	Matrix4 local = modelMatrix;
	local.SetPositionVector({ 0, 0, 0 });

	Vector3 fwd = local * Vector4(0, 0, -1, 1.0f);
	Vector3 up = local * Vector4(0, 1, 0, 1.0f);
	Vector3 right = local * Vector4(1, 0, 0, 1.0f);

	Vector3 worldPos = modelMatrix.GetPositionVector();

	DrawLine(worldPos, worldPos + (right * scaleBoost), Debug::RED, time);
	DrawLine(worldPos, worldPos + (up * scaleBoost), Debug::GREEN, time);
	DrawLine(worldPos, worldPos + (fwd * scaleBoost), Debug::BLUE, time);
}

void Debug::UpdateRenderables(float dt) {
	int trim = 0;
	for (int i = 0; i < lineEntries.size(); ) {
		DebugLineEntry* e = &lineEntries[i];
		e->time -= dt;
		if (e->time < 0) {
			trim++;
			lineEntries[i] = lineEntries[lineEntries.size() - trim];
		}
		else {
			++i;
		}
		if (i + trim >= lineEntries.size()) {
			break;
		}
	}
	lineEntries.resize(lineEntries.size() - trim);
	stringEntries.clear();
}

SimpleFont* Debug::GetDebugFont() {
	return debugFont;
}

void Debug::CreateDebugFont(const std::string& dataFile, Texture& tex) {
	debugFont = new SimpleFont(dataFile, tex);
}

const std::vector<Debug::DebugStringEntry>& Debug::GetDebugStrings() {
	return stringEntries;
}

const std::vector<Debug::DebugLineEntry>& Debug::GetDebugLines() {
	return lineEntries;
}

const void Debug::DrawCollisionBox(NCL::CSC8503::GameObject* obj) {
	if (obj->GetBoundingVolume() == NULL) return;
	switch (obj->GetBoundingVolume()->type)
	{
	case VolumeType::AABB:
	{
		//return;
		Vector3 position = obj->GetTransform().GetPosition() + obj->GetTransform().GetCollisionOffset();
		Vector3 scale = Vector3();
		if (obj->GetTransform().GetCollisionDimensions() != Vector3()) {
			scale = obj->GetTransform().GetCollisionDimensions();
		}
		else {
			scale = obj->GetTransform().GetScale() / 2;
		}

		Vector3 a = Vector3(position.x + scale.x, position.y + scale.y, position.z + scale.z);
		Vector3 b = Vector3(position.x + scale.x, position.y + scale.y, position.z - scale.z);
		Vector3 c = Vector3(position.x + scale.x, position.y - scale.y, position.z + scale.z);
		Vector3 d = Vector3(position.x + scale.x, position.y - scale.y, position.z - scale.z);
		Vector3 e = Vector3(position.x - scale.x, position.y + scale.y, position.z + scale.z);
		Vector3 f = Vector3(position.x - scale.x, position.y + scale.y, position.z - scale.z);
		Vector3 g = Vector3(position.x - scale.x, position.y - scale.y, position.z + scale.z);
		Vector3 h = Vector3(position.x - scale.x, position.y - scale.y, position.z - scale.z);

		DrawLine(a, b, Debug::GREEN);
		DrawLine(a, c, Debug::GREEN);
		DrawLine(d, c, Debug::GREEN);
		DrawLine(d, b, Debug::GREEN);
		DrawLine(a, e, Debug::GREEN);
		DrawLine(b, f, Debug::GREEN);
		DrawLine(d, h, Debug::GREEN);
		DrawLine(c, g, Debug::GREEN);
		DrawLine(e, f, Debug::GREEN);
		DrawLine(e, g, Debug::GREEN);
		DrawLine(g, h, Debug::GREEN);
		DrawLine(f, h, Debug::GREEN);
		break;
	}

	case VolumeType::OBB:
	{
		//return;
		Vector3 position = obj->GetTransform().GetPosition() + obj->GetTransform().GetCollisionOffset();
		Vector3 scale = Vector3();
		if (obj->GetTransform().GetCollisionDimensions() != Vector3()) {
			scale = obj->GetTransform().GetCollisionDimensions();
		}
		else {
			scale = obj->GetTransform().GetScale() / 2;
		}
		Vector3 x = obj->GetTransform().GetOrientation() * Vector3(scale.x, 0, 0);
		Vector3 y = obj->GetTransform().GetOrientation() * Vector3(0, scale.y, 0);
		Vector3 z = obj->GetTransform().GetOrientation() * Vector3(0, 0, scale.z);

		Vector3 a = position + x + y + z;
		Vector3 b = position + x + y - z;
		Vector3 c = position + x - y + z;
		Vector3 d = position + x - y - z;
		Vector3 e = position - x + y + z;
		Vector3 f = position - x + y - z;
		Vector3 g = position - x - y + z;
		Vector3 h = position - x - y - z;

		DrawLine(a, b, Debug::GREEN);
		DrawLine(a, c, Debug::GREEN);
		DrawLine(d, c, Debug::GREEN);
		DrawLine(d, b, Debug::GREEN);
		DrawLine(a, e, Debug::GREEN);
		DrawLine(b, f, Debug::GREEN);
		DrawLine(d, h, Debug::GREEN);
		DrawLine(c, g, Debug::GREEN);
		DrawLine(e, f, Debug::GREEN);
		DrawLine(e, g, Debug::GREEN);
		DrawLine(g, h, Debug::GREEN);
		DrawLine(f, h, Debug::GREEN);
		break;
	}

	case VolumeType::Sphere:
		break;
	case VolumeType::Capsule:
		break;
	default:
		break;
	}


}