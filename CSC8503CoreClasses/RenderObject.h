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

			void SetFrameMatrices(const std::vector <Matrix4>& fmatrices) {
				frameMatrices = fmatrices;
			}

			std::vector<Matrix4> GetFrameMatrices() const {
				return frameMatrices;
			}

			std::vector<Texture*> matDiffuseTextures;
			std::vector<Texture*> matNormalTextures;
			std::vector<Texture*> matMetalTextures;
			std::vector<Texture*> matRoughnessTextures;
			std::vector<Texture*> matAoTextures;
			std::vector<Texture*> matHeightTextures;
			bool isAnimation = false;			
			bool isAnimated = false;
			bool isWall = false;
			bool onLight = false;
			bool isLight = false;
			bool isShadow = false;
			float radius = 0.0f;

			int			currentFrame;
			float		frameTime;
			float		scale;
		protected:
			Mesh*		mesh;
			Texture*	texture[6];
			Shader*		shader;
			Transform*	transform;
			Vector4		colour;



			std::vector <Matrix4 > frameMatrices;

		};
	}
}
