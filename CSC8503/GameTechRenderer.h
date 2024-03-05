#pragma once
#include "OGLRenderer.h"
#include "OGLShader.h"
#include "OGLTexture.h"
#include "OGLMesh.h"
#include "GameWorld.h"

namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	namespace CSC8503 {
		class RenderObject;

		class GameTechRenderer : public OGLRenderer	{
		public:
			GameTechRenderer(GameWorld& world);
			~GameTechRenderer();
			Mesh*		LoadMesh(const std::string& name);
			Texture*	LoadTexture(const std::string& name);
			Shader*		LoadShader(const std::string& vertex, const std::string& fragment);

			vector<Vector4> LoadMap();

		protected:
			std::unordered_map<std::string, SharedOGLTexture> textureCache;

			void NewRenderLines();
			void NewRenderText();

			void RenderFrame()	override;

			OGLShader*		defaultShader;

			GameWorld&	gameWorld;

			void BuildObjectList();
			void SortObjectList();
			void RenderShadowMap();
			void RenderCamera(); 
			void RenderSkybox();
			void RenderLight();
			void RenderCombine();

			void LoadSkybox();

			void SetDebugStringBufferSizes(size_t newVertCount);
			void SetDebugLineBufferSizes(size_t newVertCount);

			float bias;
			float far_plane;

			vector<const RenderObject*> activeObjects;

			OGLShader*  debugShader;
			OGLShader* skyboxShader;
			OGLMesh*	skyboxMesh;
			Mesh* lightMesh;
			OGLMesh* quadMesh;
			GLuint		skyboxTex;

			//shadow mapping things
			OGLShader*	shadowShader;
			GLuint		shadowTex;
			GLuint		shadowFBO;
			Matrix4     shadowMatrix;

			//deferred render
			OGLShader* lightShader;
			OGLShader* combineShader;
			GLuint		bufferFBO;
			GLuint		lightFBO;
			GLuint		depthTex;
			GLuint		colorTex;
			GLuint		addTex;
			GLuint		indexTex;
			GLuint		normalTex;
			GLuint		dataTex;
			GLuint		lightDiffTex;
			GLuint		lightSpecTex;
			Texture* skinTex;

			Vector4		lightColour;
			float		lightRadius;
			//float		fov; 
			Vector3		lightPosition;
			Vector3		shadowPosition;

			//Debug data storage things
			vector<Vector3> debugLineData;

			vector<Vector3> debugTextPos;
			vector<Vector4> debugTextColours;
			vector<Vector2> debugTextUVs;

			GLuint lineVAO;
			GLuint lineVertVBO;
			size_t lineCount;

			GLuint textVAO;
			GLuint textVertVBO;
			GLuint textColourVBO;
			GLuint textTexVBO;
			size_t textCount;

		};
	}
}