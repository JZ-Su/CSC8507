#pragma once
#include "GameTechRenderer.h"
#ifdef USEVULKAN
#include "GameTechVulkanRenderer.h"
#endif
#include "PhysicsSystem.h"

#include "StateGameObject.h"

#include "NavigationGrid.h"

namespace NCL {
	class BasicExamples
	{
	public:
		BasicExamples(GameTechRenderer* render);
		~BasicExamples();
		GameObject* CreateCube(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateSphere(const Vector3& position, float radius, float inverseMass = 10.0f);
		GameObject* CreateChar(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateGoat(const Vector3& position, const Vector3& dimensions, float inverseMass = 10.0f);
		GameObject* CreateCapsule(const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f);

	protected:
		Mesh* cubeMesh = nullptr;
		Mesh* sphereMesh = nullptr;
		Mesh* charMesh = nullptr;
		Mesh* goatMesh = nullptr;
		Mesh* capsuleMesh = nullptr;

		Texture* basicTexture = nullptr;
		Shader* basicShader = nullptr;
	};
}
