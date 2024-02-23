#pragma once
//#include "GameTechRenderer.h"
#include "OGLTexture.h"
#include "OGLShader.h"
#include "OGLMesh.h"

#include "../NCLCoreClasses/Vector2.h"

namespace NCL {
	namespace CSC8503 {
		class GameUI 
		{

		public:
			struct UI {

			};
			GameUI();
			~GameUI();
            NCL::Rendering::OGLShader* getshader() { return healthShader; };
			NCL::Rendering::OGLMesh* getMesh() { return healthMesh; };
			GLuint   gettex() { return healthTex; };
            void creatGameUI(std::vector<NCL::Maths::Vector3> UIpox, std::vector<NCL::Maths::Vector2> uitex,int width,int haight,int channels,int flags, const std::string& name);
			void healthline();
			void setUIpos(NCL::Rendering::OGLMesh* Mesh);

		protected:
			
			
			NCL::Rendering::OGLShader* healthShader;
			NCL::Rendering::OGLMesh* healthMesh;
			GLuint		healthTex;
			
			std::vector<NCL::Maths::Vector3> UIpositions;
			std::vector<NCL::Maths::Vector2> UItexCoords;
			//static
			//OGLShader* healthShader;
			//OGLMesh* healthMesh;
			//GLuint	healthTex;

		};




	}



}
