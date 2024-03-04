#include "GameUI.h"
#include "TextureLoader.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

std::vector<GameUI::UIen>	GameUI::UIEntries;
//std::map<std::string,Texture*> UImap;

GameUI::GameUI() {

healthShader = new OGLShader("health.vert", "health.frag");
healthMesh = new OGLMesh();

	//UImap["blood"]= renderer->LoadTexture("blood.png");
	//UImap["redbootle"]= renderer->LoadTexture("redbootle.png");
}

GameUI::~GameUI() {

}
void GameUI::setUI(std::vector<Vector3> UIpos, std::vector<Vector2> UItex, int widthI, int heightI, int channelsI, int flagsI, const std::string& nameI) {
UIen newentry;
newentry.UIpox = UIpos;
newentry.uitex = UItex;
newentry.width = widthI;
newentry.haight = heightI;
newentry.channels = channelsI;
newentry.flags = flagsI;
newentry.name = nameI;

//void GameUI::CreateGameUI(std::vector<Vector3> UIpos, std::vector<Vector2> UItex,const std::string& name, std::string tag) {
//
//	UIen newentry;
//	newentry.mesh = new OGLMesh();
//	newentry.tag = tag;
//	newentry.texture = UImap[name];
//	newentry.mesh->SetVertexPositions({ UIpos });
//	newentry.mesh->SetVertexTextureCoords({ UItex });
//	newentry.mesh->SetVertexIndices({ 0,1,2,2,3,0 });
//	newentry.mesh->UploadToGPU();
//
//	UIEntries.push_back(newentry);
//
//}

//
//void GameUI::UpdateUI(const std::string& tag, std::vector<NCL::Maths::Vector3> UIpox) {
//	
//	for (int i = 0; i < UIEntries.size(); i++) {
//		if (UIEntries[i].tag == tag) {
//			//UIEntries[i].mesh->DeteleBuffer();
//			//UIEntries[i].mesh->SetVertexPositions({ UIpox });
//			//UIEntries[i].mesh->UploadToGPU();
//		}
//		
//	} 
//
//	for (const auto& element:UIEntries) {
//		delete element.mesh;
//
//	}
//	//std::vector<UIen>().swap(UIEntries);
//	UIEntries.clear();	
//}
//
//const std::vector<GameUI::UIen>& GameUI::GetUIEntries() {
//	return UIEntries;
//}
