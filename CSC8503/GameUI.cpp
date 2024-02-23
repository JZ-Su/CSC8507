#include "GameUI.h"
#include "TextureLoader.h"
using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

std::vector<GameUI::UIen>	GameUI::UIEntries;

GameUI::GameUI() {

healthShader = new OGLShader("health.vert", "health.frag");
healthMesh = new OGLMesh();

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

UIEntries.emplace_back(newentry);
}

void GameUI::creatGameUI(std::vector<Vector3> UIpos, std::vector<Vector2> UItex,int width,int height,int channels,int flags, const std::string& name) {
	

//Vector3(-15, 15, -1), Vector3(-15, -15, -1), Vector3(15, -15, -1), Vector3(15, 15, -1);
//Vector2(-1, 1), Vector2(-1, -1), Vector2(1, -1), Vector2(1, 1);

healthMesh->SetVertexPositions({ UIEntries[0].UIpox});
healthMesh->SetVertexTextureCoords({ UItex });
healthMesh->SetVertexIndices({ 0,1,2,2,3,0 });
//healthMesh->SetVertexIndices({ 0,1,3,1,2,3 });
healthMesh->UploadToGPU();

//int width = 512;
//int height = 512;
//int channels = 3;
//int flags = 0;

std::vector<char*> texData(1, nullptr);

TextureLoader::LoadTexture(name, texData[0], width, height, channels, flags);

glGenTextures(1, &healthTex);
glBindTexture(GL_TEXTURE_2D, healthTex);

glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData[0]);


glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

glBindTexture(GL_TEXTURE_2D, 0);

}

void GameUI::Drawhealthline() {

creatGameUI({ Vector3(-0.9f, 0.9f,-1.0f), Vector3(-0.9f, 0.8f, -1.0f) , Vector3(-0.4f, 0.8f, -1.0f) , Vector3(-0.4f, 0.9f, -1.0f) },
	{ Vector2(0.0f,1.0f), Vector2(0.0f,0.0f), Vector2(1.0f,0.0f), Vector2(1.0f,1.0f) }, 512, 512, 4, 0, "blood.png");

creatGameUI({ Vector3(-0.9f, 0.9f,-1.0f), Vector3(-0.9f, 0.8f, -1.0f) , Vector3(-0.4f, 0.8f, -1.0f) , Vector3(-0.4f, 0.9f, -1.0f) },
	{ Vector2(0.0f,1.0f), Vector2(0.0f,0.0f), Vector2(1.0f,0.0f), Vector2(1.0f,1.0f) }, 1980, 360, 4, 0, "transparent.png");

}