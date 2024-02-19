#pragma once
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"

namespace NCL {
	using namespace NCL::Rendering;

	namespace CSC8503 {
		class Transform;
		using namespace Maths;

		class RenderObject
		{
		public:
			RenderObject(Transform* parentTransform, Mesh* mesh, Texture* tex, Shader* shader);
			~RenderObject();

			void SetDefaultTexture(Texture* t, int i) {
				texture[i] = t;
			}

			Texture* GetDefaultTexture(int i) const {
				return texture[i];
			}

			Mesh*	GetMesh() const {
				return mesh;
			}

			Transform*		GetTransform() const {
				return transform;
			}

			Shader*		GetShader() const {
				return shader;
			}

			void SetColour(const Vector4& c) {
				colour = c;
			}

			Vector4 GetColour() const {
				return colour;
			}

			void SetTag(const std::string& str) { tag = str; }

			std::string GetTag() const { return tag; }
			std::string tag;

		protected:
			Mesh*		mesh;
			Texture*	texture[6];
			Shader*		shader;
			Transform*	transform;
			Vector4		colour;

		};
	}
}
