#pragma once
#include "GameTechRenderer.h"
//#include "../OpenGLRendering/OGLTexture.h"
#include "../OpenGLRendering/OGLShader.h"
#include "../OpenGLRendering/OGLMesh.h"
#include "../NCLCoreClasses/Vector2.h"
#include "Texture.h"
#include <map>

namespace NCL {

	namespace CSC8503 {

		class GameUI
	{
		public:	
			
			struct UIen {	
				NCL::Rendering::OGLMesh* mesh;
				int width;
				int haight;
				int channels;
				int flags;
				Texture* texture;
				std::string tag;
			};

			
            NCL::Rendering::OGLShader* getshader() { return healthShader; };
			NCL::Rendering::OGLMesh* getMesh() { return healthMesh; };
			GLuint   gettex() { return healthTex; };
            void creatGameUI(std::vector<NCL::Maths::Vector3> UIpox, std::vector<NCL::Maths::Vector2> uitex,int width,int haight,int channels,int flags, const std::string& name);
			void Drawhealthline();

			void setUI(std::vector<Vector3> UIpos, std::vector<Vector2> UItex, int width, int height, int channels, int flags, const std::string& name);

		protected:
			GameUI();
			~GameUI();
	
			static void CreateGameUI(std::vector<NCL::Maths::Vector3> UIpox, std::vector<NCL::Maths::Vector2> uitex,
		    const std::string& name,std::string type);

			static const std::vector<UIen>& GetUIEntries();
			
			static void UpdateUI(const std::string& tag, std::vector<NCL::Maths::Vector3> UIpox);
		    static  GameTechRenderer *renderer;
		protected:

			static std::vector<UIen>	UIEntries;
		    UIen UiElementl;
			Texture* texture;

		};




	}



}
