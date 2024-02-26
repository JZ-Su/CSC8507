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
			
			struct UIen {	
				NCL::Rendering::OGLMesh* mesh;
				int width;
				int height;
				int channels;
				int flags;
				char* texture;
				float healthpoint;
				std::string tag;
			};

			GameUI();
			~GameUI();

            NCL::Rendering::OGLShader* getshader() { return healthShader; };
	
			void CreateGameUI(std::vector<NCL::Maths::Vector3> UIpox, std::vector<NCL::Maths::Vector2> uitex, 
				int width, int haight, int channels, int flags, const std::string& name,std::string type);

			std::vector<UIen> GetUIEntries() { return UIEntries; }
			
			void UpdateUI(const std::string& tag, std::vector<NCL::Maths::Vector3> UIpox);
		protected:
			
			NCL::Rendering::OGLShader* healthShader;
		
			GLuint		healthTex;
		
			std::vector<NCL::Maths::Vector3> UIpositions;
			std::vector<NCL::Maths::Vector2> UItexCoords;
			static std::vector<UIen>	UIEntries;
		    UIen UiElementl;
		};
	}
}
