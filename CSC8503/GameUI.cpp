#include "GameUI.h"
#include "TextureLoader.h"
using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

std::vector<GameUI::UIen>	GameUI::UIEntries;

GameUI::GameUI() {

healthShader = new OGLShader("health.vert", "health.frag");

}

GameUI::~GameUI() {

}


void GameUI::CreateGameUI(std::vector<Vector3> UIpos, std::vector<Vector2> UItex,int width,int height,int channels,int flags, const std::string& name,std::string tag) {
	
	UIen newentry;
	newentry.mesh = new OGLMesh();

	newentry.width = width;
	newentry.height = height;
	newentry.channels = channels;
	newentry.flags = flags;
	newentry.tag = tag;

	TextureLoader::LoadTexture(name, newentry.texture, width, height, channels, flags);

	newentry.mesh->SetVertexPositions({UIpos});
	newentry.mesh->SetVertexTextureCoords({ UItex });
	newentry.mesh->SetVertexIndices({ 0,1,2,2,3,0 });
	newentry.mesh->UploadToGPU();

	UIEntries.emplace_back(newentry);
	
}


void GameUI::UpdateUI(const std::string& tag, std::vector<NCL::Maths::Vector3> UIpox) {
	for (int i = 0; i < UIEntries.size(); i++) {
		if (UIEntries[i].tag == tag) {
			UIEntries[i].mesh->SetVertexPositions({ UIpox });
			UIEntries[i].mesh->UploadToGPU();
		}
	}
}