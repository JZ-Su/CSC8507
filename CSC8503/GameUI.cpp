#include "GameUI.h"
#include "TextureLoader.h"
using namespace NCL;
using namespace Rendering;
using namespace CSC8503;
GameUI::GameUI() {

}

GameUI::~GameUI() {

}

void GameUI::creatGameUI(std::vector<Vector3> UIpos, std::vector<Vector2> UItex,int width,int height,int channels,int flags, const std::string& name) {


healthShader = new OGLShader("health.vert", "health.frag");
healthMesh = new OGLMesh();

//Vector3(-15, 15, -1), Vector3(-15, -15, -1), Vector3(15, -15, -1), Vector3(15, 15, -1);
//Vector2(-1, 1), Vector2(-1, -1), Vector2(1, -1), Vector2(1, 1);

healthMesh->SetVertexPositions({ UIpos });
healthMesh->SetVertexTextureCoords({ UItex });
healthMesh->SetVertexIndices({ 0,1,2,2,3,0 });
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

void GameUI::setUIpos(NCL::Rendering::OGLMesh* Mesh)
{
	std::vector<Vector3>		positions;
	positions=Mesh->GetPositionData();
	Mesh->GetBindPose();
	//float scale = size;

	//positions.reserve(positions.size() + 6);
	//texCoords.reserve(texCoords.size() +6);

	//float charWidth = (30.0f / 512) * 20.0f;
	//float charHeight = 30.0f;

	//float xStart = ((charData.xOff + currentX) * texWidthRecip) * scale;
	//float yStart = startPos.y;
	//float yHeight = (charHeight * texHeightRecip) * scale;
	//float yOff = ((charHeight + charData.yOff) * texHeightRecip) * scale;

}