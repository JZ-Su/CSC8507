#include "GameTechRenderer.h"
#include "GameObject.h"
#include "RenderObject.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "MshLoader.h"
#include "Frustum.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

#define SHADOWSIZE 4096

Matrix4 biasMatrix = Matrix4::Translation(Vector3(0.5f, 0.5f, 0.5f)) * Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f));
std::vector<GameTechRenderer::UIen>	GameTechRenderer::UIEntries;
std::map<std::string, char*> UImap;

GameTechRenderer::GameTechRenderer(GameWorld& world) : OGLRenderer(*Window::GetWindow()), gameWorld(world) {
	glEnable(GL_DEPTH_TEST);

	debugShader = new OGLShader("debug.vert", "debug.frag");
	shadowShader = new OGLShader("shadow.vert", "shadow.frag", "shadow.geom");
	lightShader = new OGLShader("pointlight.vert", "pointlight.frag");
	combineShader = new OGLShader("combine.vert", "combine.frag");
	healthShader = new OGLShader("health.vert", "health.frag");
	upSampleShader = new OGLShader("combine.vert", "upSample.frag");
	downSampleShader = new OGLShader("combine.vert", "downSample.frag");
	toneShader = new OGLShader("combine.vert", "tonemapping.frag");

	glGenTextures(1, &shadowTex);

	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowTex);
	for (int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, windowSize.x, windowSize.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &colorTex);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowSize.x, windowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &normalTex);
	glBindTexture(GL_TEXTURE_2D, normalTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowSize.x, windowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &dataTex);
	glBindTexture(GL_TEXTURE_2D, dataTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowSize.x, windowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &addTex);
	glBindTexture(GL_TEXTURE_2D, addTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowSize.x, windowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &indexTex);
	glBindTexture(GL_TEXTURE_2D, indexTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, windowSize.x, windowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);

	GLenum buffers[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	glGenFramebuffers(1, &bufferFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, dataTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, addTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, indexTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
	glDrawBuffers(5, buffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return;
	}

	glGenTextures(1, &lightDiffTex);
	glBindTexture(GL_TEXTURE_2D, lightDiffTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowSize.x, windowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &lightSpecTex);
	glBindTexture(GL_TEXTURE_2D, lightSpecTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowSize.x, windowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenFramebuffers(1, &lightFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightDiffTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, lightSpecTex, 0);
	glDrawBuffers(2, buffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return;
	}

	for (int i = 0; i < 6; i++) {
		glGenTextures(1, &processTex[i]);
		glBindTexture(GL_TEXTURE_2D, processTex[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int sqrt = pow(2, i);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowSize.x / sqrt, windowSize.y / sqrt, 0, GL_RGB, GL_FLOAT, NULL);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &postFBO);
	glGenFramebuffers(1, &processFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, postFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, indexTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, processTex[0], 0);
	glDrawBuffers(2, buffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 1, 1, 1);

	//Set up the light properties
	//lightColour = Vector4(1.0f, 0.8f, 0.3f, 1.0f);
	//lightRadius = 130.0f;
	//lightPosition = Vector3(0.0f, 50.0f, -30.0f);
	//shadowPosition = Vector3(0.0f, 50.0f, -30.0f);

	lightMesh = LoadMesh("sphere.msh");
	quadMesh = new OGLMesh();
	quadMesh->SetVertexPositions({ Vector3(-1, 1,0), Vector3(-1,-1,0) , Vector3(1,-1,0) , Vector3(1,1,0) });
	quadMesh->SetVertexIndices({ 0,1,2,2,3,0 });
	quadMesh->SetVertexTextureCoords({ Vector2(0.0, 1.0), Vector2(0.0, 0.0) , Vector2(1.0, 0.0) , Vector2(1.0, 1.0) });
	quadMesh->UploadToGPU();
	skinTex = LoadTexture("Player/preintegrated_falloff_2D.png");

	//Skybox!
	skyboxShader = new OGLShader("skybox.vert", "skybox.frag");
	skyboxMesh = new OGLMesh();
	skyboxMesh->SetVertexPositions({ Vector3(-1, 1,-1), Vector3(-1,-1,-1) , Vector3(1,-1,-1) , Vector3(1,1,-1) });
	skyboxMesh->SetVertexIndices({ 0,1,2,2,3,0 });
	skyboxMesh->UploadToGPU();

	LoadSkybox();

	glGenVertexArrays(1, &lineVAO);
	glGenVertexArrays(1, &textVAO);

	glGenBuffers(1, &lineVertVBO);
	glGenBuffers(1, &textVertVBO);
	glGenBuffers(1, &textColourVBO);
	glGenBuffers(1, &textTexVBO);

	far_plane = 200.0;

	Debug::CreateDebugFont("PressStart2P.fnt", *LoadTexture("PressStart2P.png"));

	SetDebugStringBufferSizes(10000);
	SetDebugLineBufferSizes(1000);

	vector<char*> data(6, nullptr);
	int width;
	int height;
	int channel;
	int flag;

	TextureLoader::LoadTexture("blood.png", data[0], width, height, channel, flag);
	UImap["blood"] = data[0];
	TextureLoader::LoadTexture("greenbottle.png", data[1], width, height, channel, flag);
	UImap["greenbottle"] = data[1];
	TextureLoader::LoadTexture("redbottle.png", data[2], width, height, channel, flag);
	UImap["redbottle"] = data[2];
	TextureLoader::LoadTexture("inventory.png", data[3], width, height, channel, flag);
	UImap["inventory"] = data[3];

}

GameTechRenderer::~GameTechRenderer() {
	glDeleteTextures(1, &shadowTex);
	glDeleteTextures(1, &depthTex);
	glDeleteTextures(1, &colorTex);
	glDeleteTextures(1, &normalTex);
	glDeleteTextures(1, &dataTex);
	glDeleteTextures(1, &addTex);
	glDeleteTextures(1, &indexTex);
	glDeleteTextures(1, &lightDiffTex);
	glDeleteTextures(1, &lightSpecTex);
	glDeleteTextures(1, &processTex[0]);
	glDeleteTextures(1, &processTex[1]);
	glDeleteTextures(1, &processTex[2]);
	glDeleteFramebuffers(1, &shadowFBO);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &lightFBO);
	glDeleteFramebuffers(1, &postFBO);
	glDeleteFramebuffers(1, &processFBO);
	textureCache.clear();
}

void GameTechRenderer::LoadSkybox() {
	std::string filenames[6] = {
		"/Cubemap/skyrender0004.png",
		"/Cubemap/skyrender0001.png",
		"/Cubemap/skyrender0003.png",
		"/Cubemap/skyrender0006.png",
		"/Cubemap/skyrender0002.png",
		"/Cubemap/skyrender0005.png"
	};

	int width[6] = { 0 };
	int height[6] = { 0 };
	int channels[6] = { 0 };
	int flags[6] = { 0 };

	vector<char*> texData(6, nullptr);

	for (int i = 0; i < 6; ++i) {
		TextureLoader::LoadTexture(filenames[i], texData[i], width[i], height[i], channels[i], flags[i]);
		if (i > 0 && (width[i] != width[0] || height[0] != height[0])) {
			std::cout << __FUNCTION__ << " cubemap input textures don't match in size?\n";
			return;
		}
	}
	glGenTextures(1, &skyboxTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	GLenum type = channels[0] == 4 ? GL_RGBA : GL_RGB;

	for (int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width[i], height[i], 0, type, GL_UNSIGNED_BYTE, texData[i]);
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void GameTechRenderer::RenderFrame() {
	glEnable(GL_CULL_FACE);
	glClearColor(1, 1, 1, 1);
	BuildObjectList();
	SortObjectList();
	RenderShadowMap();
	RenderCamera();
	RenderLight();
	RenderSkybox();
	RenderCombine();
	RenderProcess();
	RenderTone();
	glDisable(GL_CULL_FACE); //Todo - text indices are going the wrong way...
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	NewRenderLines();
	NewRenderText();
	Loadhealth();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameTechRenderer::BuildObjectList() {
	activeObjects.clear();

	gameWorld.OperateOnContents(
		[&](GameObject* o) {
			if (o->IsActive()) {
				const RenderObject* g = o->GetRenderObject();
				if (g) {
					activeObjects.emplace_back(g);
				}
			}
		}
	);
}

void GameTechRenderer::SortObjectList() {
	vector<const RenderObject*> transparentObject;
	for (int i = 0; i < activeObjects.size(); i++) {
		if (activeObjects[i]->GetColour().w != 1) {
			const RenderObject* g = activeObjects[i];
			activeObjects.erase(activeObjects.begin() + i);
			transparentObject.push_back(g);
		}
	}
	activeObjects.insert(activeObjects.end(), transparentObject.begin(), transparentObject.end());
}

void GameTechRenderer::RenderShadowMap() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	//glCullFace(GL_FRONT);

	BindShader(*shadowShader);
	int mvpLocation = glGetUniformLocation(shadowShader->GetProgramID(), "mvpMatrix");
	int shadowLocation = glGetUniformLocation(shadowShader->GetProgramID(), "shadowTex");
	int lightPosLocation = glGetUniformLocation(shadowShader->GetProgramID(), "lightPos");
	
	int projLocation = glGetUniformLocation(shadowShader->GetProgramID(), "shadProjMatrix");
	int viewLocation = glGetUniformLocation(shadowShader->GetProgramID(), "viewMatrix");
	int isAnim = glGetUniformLocation(shadowShader->GetProgramID(), "isAnimation");

	for (const auto& i : activeObjects) {
		if ((*i).isShadow) {
			shadowPosition = (*i).GetTransform()->GetPosition();
		}
	}	
	
	glUniform3fv(lightPosLocation, 1, (float*)&shadowPosition);

	Matrix4 shadowProjMatrix = Matrix4::Perspective(1.0f, far_plane, 1, 90.0f);
	vector<Matrix4> shadowViewMatrix;
	shadowViewMatrix.push_back(shadowProjMatrix * Matrix4::lookAt(180, 90, shadowPosition));
	shadowViewMatrix.push_back(shadowProjMatrix * Matrix4::lookAt(180, -90, shadowPosition));

	shadowViewMatrix.push_back(shadowProjMatrix * Matrix4::lookAt(90, 0, shadowPosition));
	shadowViewMatrix.push_back(shadowProjMatrix * Matrix4::lookAt(-90, 0, shadowPosition));

	shadowViewMatrix.push_back(shadowProjMatrix * Matrix4::lookAt(180, 0, shadowPosition));
	shadowViewMatrix.push_back(shadowProjMatrix * Matrix4::lookAt(180, 180, shadowPosition));

	int shadowMatLocation = glGetUniformLocation(shadowShader->GetProgramID(), "shadowMatrices");
	glUniformMatrix4fv(shadowMatLocation, 6, false, (float*)shadowViewMatrix.data());

	glUniform1i(shadowLocation, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowTex);
	Matrix4 mvpMatrix;
	for (const auto& i : activeObjects) {
		glUniform1i(isAnim, i->isAnimated);

		if (i->isAnimated == true) {
			int j = glGetUniformLocation(shadowShader->GetProgramID(), "joints");
			glUniformMatrix4fv(j, i->GetFrameMatrices().size(), false, (float*)i->GetFrameMatrices().data());
		}

		Matrix4 modelMatrix = (*i).GetTransform()->GetMatrix();
		mvpMatrix = modelMatrix;
		glUniformMatrix4fv(mvpLocation, 1, false, (float*)&mvpMatrix);
		glActiveTexture(GL_TEXTURE1);
		if (!(*i).isLight) {
			Matrix4 modelMatrix = (*i).GetTransform()->GetMatrix();
			mvpMatrix = modelMatrix;
			glUniformMatrix4fv(mvpLocation, 1, false, (float*)&mvpMatrix);
			glActiveTexture(GL_TEXTURE1);

			BindMesh((OGLMesh&)*(*i).GetMesh());
			size_t layerCount = (*i).GetMesh()->GetSubMeshCount();
			for (size_t i = 0; i < layerCount; ++i) {
				DrawBoundMesh((uint32_t)i);
			}
		}
	}


	glViewport(0, 0, windowSize.x, windowSize.y);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_BACK);
}

void GameTechRenderer::RenderSkybox() {
	glBindFramebuffer(GL_FRAMEBUFFER, postFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	Matrix4 viewMatrix = gameWorld.GetMainCamera().BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera().BuildProjectionMatrix(hostWindow.GetScreenAspect());

	BindShader(*skyboxShader);

	int projLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "projMatrix");
	int viewLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "viewMatrix");
	int texLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "cubeTex");

	glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

	glUniform1i(texLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	BindMesh(*skyboxMesh);
	DrawBoundMesh();

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameTechRenderer::RenderCamera() {
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	Matrix4 viewMatrix = gameWorld.GetMainCamera().BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera().BuildProjectionMatrix(hostWindow.GetScreenAspect());

	OGLShader* activeShader = nullptr;
	int projLocation = 0;
	int viewLocation = 0;
	int modelLocation = 0;
	int colourLocation = 0;
	int hasVColLocation = 0;
	int hasTexLocation = 0;

	int cameraLocation = 0;

	for (const auto& i : activeObjects) {
		if (!(*i).isLight) {
			OGLShader* shader = (OGLShader*)(*i).GetShader();
			BindShader(*shader);

			if ((*i).GetDefaultTexture(0)) {
				BindTextureToShader(*(OGLTexture*)(*i).GetDefaultTexture(0), "mainTex", 0);
			}
			else {
				BindTextureToShader(0, "mainTex", 0);
			}
			if ((*i).GetDefaultTexture(1)) {
				BindTextureToShader(*(OGLTexture*)(*i).GetDefaultTexture(1), "normalTex", 2);
			}
			else {
				BindTextureToShader(0, "normalTex", 2);
			}
			if ((*i).GetDefaultTexture(2)) {
				BindTextureToShader(*(OGLTexture*)(*i).GetDefaultTexture(2), "metalTex", 3);
			}
			else {
				BindTextureToShader(0, "metalTex", 3);
			}
			if ((*i).GetDefaultTexture(3)) {
				BindTextureToShader(*(OGLTexture*)(*i).GetDefaultTexture(3), "roughTex", 4);
			}
			else {
				BindTextureToShader(0, "roughTex", 4);
			}
			if ((*i).GetDefaultTexture(4)) {
				BindTextureToShader(*(OGLTexture*)(*i).GetDefaultTexture(4), "aoTex", 5);
			}
			else {
				BindTextureToShader(0, "aoTex", 5);
			}
			if ((*i).GetDefaultTexture(5)) {
				BindTextureToShader(*(OGLTexture*)(*i).GetDefaultTexture(5), "heightTex", 6);
			}
			else {
				BindTextureToShader(0, "heightTex", 6);
			}

			if (activeShader != shader) {
				projLocation = glGetUniformLocation(shader->GetProgramID(), "projMatrix");
				viewLocation = glGetUniformLocation(shader->GetProgramID(), "viewMatrix");
				modelLocation = glGetUniformLocation(shader->GetProgramID(), "modelMatrix");
				colourLocation = glGetUniformLocation(shader->GetProgramID(), "objectColour");
				hasVColLocation = glGetUniformLocation(shader->GetProgramID(), "hasVertexColours");
				hasTexLocation = glGetUniformLocation(shader->GetProgramID(), "hasTexture");

				cameraLocation = glGetUniformLocation(shader->GetProgramID(), "cameraPos");

				if (i->isAnimation) {
					int j = glGetUniformLocation(shader->GetProgramID(), "joints");
					glUniformMatrix4fv(j, i->GetFrameMatrices().size(), false, (float*)i->GetFrameMatrices().data());
				}

				Vector3 camPos = gameWorld.GetMainCamera().GetPosition();
				glUniform3fv(cameraLocation, 1, &camPos.x);

				glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
				glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

				activeShader = shader;
			}

			Matrix4 modelMatrix = (*i).GetTransform()->GetMatrix();
			glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

			Vector4 colour = i->GetColour();
			glUniform4fv(colourLocation, 1, &colour.x);

			glUniform1i(hasVColLocation, !(*i).GetMesh()->GetColourData().empty());

			glUniform1i(hasTexLocation, (OGLTexture*)(*i).GetDefaultTexture(0) ? 1 : 0);

			BindMesh((OGLMesh&)*(*i).GetMesh());
			size_t layerCount = (*i).GetMesh()->GetSubMeshCount();
			for (size_t x = 0; x < layerCount; ++x) {
				if ((*i).isAnimation) {
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, ((OGLTexture*)i->matDiffuseTextures[x])->GetObjectID());
					glUniform1i(glGetUniformLocation(shader->GetProgramID(), "mainTex"), 0);

					if (!i->matNormalTextures.empty() && x < i->matNormalTextures.size() && i->matNormalTextures[x] != nullptr) {
						glActiveTexture(GL_TEXTURE2);
						glBindTexture(GL_TEXTURE_2D, ((OGLTexture*)i->matNormalTextures[x])->GetObjectID());
						glUniform1i(glGetUniformLocation(shader->GetProgramID(), "normalTex"), 2);
					}
					else {
						glUniform1i(glGetUniformLocation(shader->GetProgramID(), "normalTex"), 7); // set defalut value
					}

					if (!i->matMetalTextures.empty() && x < i->matMetalTextures.size() && i->matMetalTextures[x] != nullptr) {
						glActiveTexture(GL_TEXTURE3);
						glBindTexture(GL_TEXTURE_2D, ((OGLTexture*)i->matMetalTextures[x])->GetObjectID());
						glUniform1i(glGetUniformLocation(shader->GetProgramID(), "metalTex"), 3);
					}
					else {
						glUniform1i(glGetUniformLocation(shader->GetProgramID(), "metalTex"), 7); // set defalut value
					}

					if (!i->matRoughnessTextures.empty() && x < i->matRoughnessTextures.size() && i->matRoughnessTextures[x] != nullptr) {
						glActiveTexture(GL_TEXTURE4);
						glBindTexture(GL_TEXTURE_2D, ((OGLTexture*)i->matRoughnessTextures[x])->GetObjectID());
						glUniform1i(glGetUniformLocation(shader->GetProgramID(), "roughTex"), 4);
					}
					else {
						glUniform1i(glGetUniformLocation(shader->GetProgramID(), "roughTex"), 7); // set defalut value
					}

					if (!i->matAoTextures.empty() && x < i->matAoTextures.size() && i->matAoTextures[x] != nullptr) {
						glActiveTexture(GL_TEXTURE5);
						glBindTexture(GL_TEXTURE_2D, ((OGLTexture*)i->matAoTextures[x])->GetObjectID());
						glUniform1i(glGetUniformLocation(shader->GetProgramID(), "aoTex"), 5);
					}
					else {
						glUniform1i(glGetUniformLocation(shader->GetProgramID(), "aoTex"), 7); // set defalut value
					}

					if (!i->matHeightTextures.empty() && x < i->matHeightTextures.size() && i->matHeightTextures[x] != nullptr) {
						glActiveTexture(GL_TEXTURE6);
						glBindTexture(GL_TEXTURE_2D, ((OGLTexture*)i->matHeightTextures[x])->GetObjectID());
						glUniform1i(glGetUniformLocation(shader->GetProgramID(), "heightTex"), 6);
					}
					else {
						glUniform1i(glGetUniformLocation(shader->GetProgramID(), "heightTex"), 7); // set defalut value
					}
				}
				DrawBoundMesh((uint32_t)x);
			}
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameTechRenderer::RenderLight() {
	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	BindShader(*lightShader);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_ONE, GL_ONE);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_FALSE);

	Matrix4 viewMatrix = gameWorld.GetMainCamera().BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera().BuildProjectionMatrix(hostWindow.GetScreenAspect());
	Vector2 pixel = Vector2(1.0f / windowSize.x, 1.0f / windowSize.y );

	int projLocation = 0;
	int viewLocation = 0;
	int modelLocation = 0;
	int lightPosLocation	= 0;
	int lightColourLocation = 0;
	int lightRadiusLocation = 0;

	int cameraLocation = 0;
	int pixelLocation = 0;

	//TODO - PUT IN FUNCTION
	glActiveTexture(GL_TEXTURE0 + 8);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, normalTex);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, dataTex);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, addTex);
	glActiveTexture(GL_TEXTURE0 + 5);
	glBindTexture(GL_TEXTURE_2D, indexTex);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowTex);

	Matrix4 invViewProj = (projMatrix * viewMatrix).Inverse();
	int invLocation = glGetUniformLocation(lightShader->GetProgramID(), "inverseProjView");
	projLocation = glGetUniformLocation(lightShader->GetProgramID(), "projMatrix");
	viewLocation = glGetUniformLocation(lightShader->GetProgramID(), "viewMatrix");
	lightPosLocation = glGetUniformLocation(lightShader->GetProgramID(), "lightPos");
	lightColourLocation = glGetUniformLocation(lightShader->GetProgramID(), "lightColour");
	lightRadiusLocation = glGetUniformLocation(lightShader->GetProgramID(), "lightRadius");
	int shadowPosLocation = glGetUniformLocation(lightShader->GetProgramID(), "shadowPos");
	cameraLocation = glGetUniformLocation(lightShader->GetProgramID(), "cameraPos");
	pixelLocation = glGetUniformLocation(lightShader->GetProgramID(), "pixelSize");
	glUniformMatrix4fv(invLocation, 1, false, (float*)&invViewProj);
	Vector3 camPos = gameWorld.GetMainCamera().GetPosition();
	glUniform3fv(cameraLocation, 1, &camPos.x);
	glUniform2fv(pixelLocation, 1, &pixel.x);
	glUniform3fv(shadowPosLocation, 1, (float*)&shadowPosition);

	glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

	int depthTexLocation = glGetUniformLocation(lightShader->GetProgramID(), "depthTex");
	glUniform1i(depthTexLocation, 8);
	int colorTexLocation = glGetUniformLocation(lightShader->GetProgramID(), "colorTex");
	glUniform1i(colorTexLocation, 0);
	int normalTexLocation = glGetUniformLocation(lightShader->GetProgramID(), "normalTex");
	glUniform1i(normalTexLocation, 2);
	int dataTexLocation = glGetUniformLocation(lightShader->GetProgramID(), "dataTex");
	glUniform1i(dataTexLocation, 3);
	int addTexLocation = glGetUniformLocation(lightShader->GetProgramID(), "addTex");
	glUniform1i(addTexLocation, 4);
	int indexTexLocation = glGetUniformLocation(lightShader->GetProgramID(), "indexTex");
	glUniform1i(indexTexLocation, 5);
	BindTextureToShader(*(OGLTexture*)skinTex, "skinTex", 6);
	int shadowTexLocation = glGetUniformLocation(lightShader->GetProgramID(), "shadowTex");
	glUniform1i(shadowTexLocation, 1);

	for (const auto& i : activeObjects) {
		if ((*i).onLight) {
			lightPosition = (*i).GetTransform()->GetPosition();
			lightColour = (*i).GetColour();
			lightRadius = (*i).GetTransform()->GetScale().x;
			glUniform3fv(lightPosLocation, 1, (float*)&lightPosition);
			glUniform4fv(lightColourLocation, 1, (float*)&lightColour);
			glUniform1f(lightRadiusLocation, lightRadius);
			BindMesh((OGLMesh&)*lightMesh);
			DrawBoundMesh();
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glClearColor(0.2f, 0.2f, 0.2f, 1);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameTechRenderer::RenderCombine() {
	glBindFramebuffer(GL_FRAMEBUFFER, postFBO);
	BindShader(*combineShader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lightDiffTex);
	int lightDiffTexLocation = glGetUniformLocation(combineShader->GetProgramID(), "lightDiffTex");
	glUniform1i(lightDiffTexLocation, 0);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	int depthTexLocation = glGetUniformLocation(combineShader->GetProgramID(), "depthTex");
	glUniform1i(depthTexLocation, 1);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, lightSpecTex);
	int lightSpecTexLocation = glGetUniformLocation(combineShader->GetProgramID(), "lightSpecTex");
	glUniform1i(lightSpecTexLocation, 2);

	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	int colorTexLocation = glGetUniformLocation(combineShader->GetProgramID(), "colorTex");
	glUniform1i(colorTexLocation, 3);

	BindMesh(*quadMesh);
	DrawBoundMesh();
	glDepthFunc(GL_LEQUAL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameTechRenderer::RenderProcess() {
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	for (int i = 1; i < 5; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, processTex[i], 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	}
	glDisable(GL_DEPTH_TEST);

	BindShader(*downSampleShader);
	BindMesh(*quadMesh);
	
	glActiveTexture(GL_TEXTURE0);
	int sizeLocation = glGetUniformLocation(downSampleShader->GetProgramID(), "mapSize");
	int colorTexLocation = glGetUniformLocation(downSampleShader->GetProgramID(), "colorTex");
	Vector2 size = Vector2(0.0, 0.0);
	int sqrt = 0;

	for (int i = 0; i < 5; ++i) {
		sqrt = pow(2, i);
		size = Vector2(windowSize.x / sqrt, windowSize.y / sqrt);
		glUniform2fv(sizeLocation, 1, &size.x);
		glBindTexture(GL_TEXTURE_2D, processTex[i]);
		glUniform1i(colorTexLocation, 0);
		int sqrtN = pow(2, i + 1);
		glViewport(0, 0, windowSize.x / sqrtN, windowSize.y / sqrtN);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, processTex[i + 1], 0);
		DrawBoundMesh();
	}
	BindShader(*upSampleShader);
	sizeLocation = glGetUniformLocation(upSampleShader->GetProgramID(), "mapSize");
	colorTexLocation = glGetUniformLocation(upSampleShader->GetProgramID(), "colorTex");
	for (int i = 5; i > 0; i--) {
		sqrt = pow(2, i);
		size = Vector2(windowSize.x / sqrt, windowSize.y / sqrt);
		glUniform2fv(sizeLocation, 1, &size.x);
		glBindTexture(GL_TEXTURE_2D, processTex[i]);
		glUniform1i(colorTexLocation, 0);
		int sqrtN = pow(2, i - 1);
		glViewport(0, 0, windowSize.x / sqrtN, windowSize.y / sqrtN);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, processTex[i - 1], 0);
		DrawBoundMesh();
	}
	glViewport(0, 0, windowSize.x, windowSize.y);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameTechRenderer::RenderTone() {
	BindShader(*toneShader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, indexTex);
	int colorTexLocation = glGetUniformLocation(toneShader->GetProgramID(), "colorTex");
	glUniform1i(colorTexLocation, 0);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, processTex[1]);
	int bloomLocation = glGetUniformLocation(toneShader->GetProgramID(), "bloomTex");
	glUniform1i(bloomLocation, 2);

	BindMesh(*quadMesh);
	DrawBoundMesh();
}

Mesh* GameTechRenderer::LoadMesh(const std::string& name) {
	OGLMesh* mesh = new OGLMesh();
	MshLoader::LoadMesh(name, *mesh);
	mesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	mesh->UploadToGPU();
	return mesh;
	//Matrix4 viewProj = projMatrix * viewMatrix;
}

void GameTechRenderer::NewRenderLines() {
	const std::vector<Debug::DebugLineEntry>& lines = Debug::GetDebugLines();
	if (lines.empty()) {
		return;
	}

	Matrix4 viewMatrix = gameWorld.GetMainCamera().BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera().BuildProjectionMatrix(hostWindow.GetScreenAspect());

	Matrix4 viewProj = projMatrix * viewMatrix;

	BindShader(*debugShader);
	int matSlot = glGetUniformLocation(debugShader->GetProgramID(), "viewProjMatrix");
	GLuint texSlot = glGetUniformLocation(debugShader->GetProgramID(), "useTexture");


	glUniformMatrix4fv(matSlot, 1, false, (float*)viewProj.array);

	debugLineData.clear();

	size_t frameLineCount = lines.size() * 2;

	SetDebugLineBufferSizes(frameLineCount);

	glBindBuffer(GL_ARRAY_BUFFER, lineVertVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, lines.size() * sizeof(Debug::DebugLineEntry), lines.data());


	glBindVertexArray(lineVAO);
	glDrawArrays(GL_LINES, 0, (GLsizei)frameLineCount);
	glBindVertexArray(0);
}

void GameTechRenderer::NewRenderText() {
	const std::vector<Debug::DebugStringEntry>& strings = Debug::GetDebugStrings();
	if (strings.empty()) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	BindShader(*debugShader);

	OGLTexture* t = (OGLTexture*)Debug::GetDebugFont()->GetTexture();

	if (t) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t->GetObjectID());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		BindTextureToShader(*t, "mainTex", 0);
	}
	Matrix4 proj = Matrix4::Orthographic(0.0, 100.0f, 100, 0, -1.0f, 1.0f);

	int matSlot = glGetUniformLocation(debugShader->GetProgramID(), "viewProjMatrix");
	glUniformMatrix4fv(matSlot, 1, false, (float*)proj.array);

	GLuint texSlot = glGetUniformLocation(debugShader->GetProgramID(), "useTexture");
	glUniform1i(texSlot, 1);

	debugTextPos.clear();
	debugTextColours.clear();
	debugTextUVs.clear();

	int frameVertCount = 0;
	for (const auto& s : strings) {
		frameVertCount += Debug::GetDebugFont()->GetVertexCountForString(s.data);
	}
	SetDebugStringBufferSizes(frameVertCount);

	for (const auto& s : strings) {
		float size = 20.0f;
		Debug::GetDebugFont()->BuildVerticesForString(s.data, s.position, s.colour, size, debugTextPos, debugTextUVs, debugTextColours);
	}

	glBindBuffer(GL_ARRAY_BUFFER, textVertVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, frameVertCount * sizeof(Vector3), debugTextPos.data());
	glBindBuffer(GL_ARRAY_BUFFER, textColourVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, frameVertCount * sizeof(Vector4), debugTextColours.data());
	glBindBuffer(GL_ARRAY_BUFFER, textTexVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, frameVertCount * sizeof(Vector2), debugTextUVs.data());

	glBindVertexArray(textVAO);
	glDrawArrays(GL_TRIANGLES, 0, frameVertCount);
	glBindVertexArray(0);
}

Texture* GameTechRenderer::LoadTexture(const std::string& name) {
	auto it = textureCache.find(name);
	if (it != textureCache.end()) {
		std::cout << "Texture '" << name << "' found in cache!" << std::endl;
		return it->second.get(); // Return the texture in cache
	}
	else {
		std::cout << "Loading texture '" << name << "'..." << std::endl;
		// If the texture can't be found in cache, load it
		UniqueOGLTexture texture = OGLTexture::TextureFromFile(name);
		if (texture) {
			SharedOGLTexture sharedTexture(std::move(texture));
			textureCache[name] = sharedTexture; // Add the texture to cache
			return sharedTexture.get();
		}
		else {
			std::cerr << "Failed to load texture '" << name << "'!" << std::endl;
			return nullptr;
		}
	}
}

Shader* GameTechRenderer::LoadShader(const std::string& vertex, const std::string& fragment) {
	return new OGLShader(vertex, fragment);
}

void GameTechRenderer::SetDebugStringBufferSizes(size_t newVertCount) {
	if (newVertCount > textCount) {
		textCount = newVertCount;

		glBindBuffer(GL_ARRAY_BUFFER, textVertVBO);
		glBufferData(GL_ARRAY_BUFFER, textCount * sizeof(Vector3), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, textColourVBO);
		glBufferData(GL_ARRAY_BUFFER, textCount * sizeof(Vector4), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, textTexVBO);
		glBufferData(GL_ARRAY_BUFFER, textCount * sizeof(Vector2), nullptr, GL_DYNAMIC_DRAW);

		debugTextPos.reserve(textCount);
		debugTextColours.reserve(textCount);
		debugTextUVs.reserve(textCount);

		glBindVertexArray(textVAO);

		glVertexAttribFormat(0, 3, GL_FLOAT, false, 0);
		glVertexAttribBinding(0, 0);
		glBindVertexBuffer(0, textVertVBO, 0, sizeof(Vector3));

		glVertexAttribFormat(1, 4, GL_FLOAT, false, 0);
		glVertexAttribBinding(1, 1);
		glBindVertexBuffer(1, textColourVBO, 0, sizeof(Vector4));

		glVertexAttribFormat(2, 2, GL_FLOAT, false, 0);
		glVertexAttribBinding(2, 2);
		glBindVertexBuffer(2, textTexVBO, 0, sizeof(Vector2));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}
}

void GameTechRenderer::SetDebugLineBufferSizes(size_t newVertCount) {
	if (newVertCount > lineCount) {
		lineCount = newVertCount;

		glBindBuffer(GL_ARRAY_BUFFER, lineVertVBO);
		glBufferData(GL_ARRAY_BUFFER, lineCount * sizeof(Debug::DebugLineEntry), nullptr, GL_DYNAMIC_DRAW);

		debugLineData.reserve(lineCount);

		glBindVertexArray(lineVAO);

		int realStride = sizeof(Debug::DebugLineEntry) / 2;

		glVertexAttribFormat(0, 3, GL_FLOAT, false, offsetof(Debug::DebugLineEntry, start));
		glVertexAttribBinding(0, 0);
		glBindVertexBuffer(0, lineVertVBO, 0, realStride);

		glVertexAttribFormat(1, 4, GL_FLOAT, false, offsetof(Debug::DebugLineEntry, colourA));
		glVertexAttribBinding(1, 0);
		glBindVertexBuffer(1, lineVertVBO, sizeof(Vector4), realStride);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}
}

vector<Vector4> GameTechRenderer::LoadMap() {
	char* texture;
	int width, height, flag, channel;
	TextureLoader::LoadTexture("map.png", texture, width, height, channel, flag);

	vector<Vector4> pixelData;
	for (int i = 0; i < width * height; i++) {
		Vector4 vec = Vector4(-(int)*(texture + 4 * i), -(int)*(texture + 4 * i + 1), -(int)*(texture + 4 * i + 2), -(int)*(texture + 4 * i + 3));
		pixelData.push_back(vec);
	}
	return pixelData;
}

void GameTechRenderer::Loadhealth() {
	//Matrix4 viewMatrix = gameWorld.GetMainCamera().BuildViewMatrix();
	const std::vector<GameTechRenderer::UIen>& uii = GameTechRenderer::GetUIEntries();
	if (uii.empty()) {
		return;
	}
	Matrix4 proj = Matrix4::Orthographic(0.0, 100.0f, 100, 0, -1.0f, 1.0f);

	BindShader(*healthShader);

	for (const auto& element : uii) {

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, element.texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);


		int matSlot = glGetUniformLocation(healthShader->GetProgramID(), "viewProjMatrix");
		glUniformMatrix4fv(matSlot, 1, false, (float*)proj.array);
		
		GLuint texSlot = glGetUniformLocation(healthShader->GetProgramID(), "useTexture");
		glUniform1i(texSlot, 1);


		GLuint mainTexLocation = glGetUniformLocation(healthShader->GetProgramID(), "mainTex");
		glUniform1i(mainTexLocation, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		BindMesh(*element.mesh);
		DrawBoundMesh();
		//element.mesh->DeteleBuffer();

		glDeleteTextures(1, &texture);
	}

}

void GameTechRenderer::CreateGameUI(std::vector<Vector3> UIpos, const std::string& name, std::string tag) {

	UIen newentry;
	newentry.mesh = new OGLMesh();
	newentry.tag = tag;
	newentry.texture = UImap[name];
	newentry.mesh->SetVertexPositions({ UIpos });
	newentry.mesh->SetVertexTextureCoords({ Vector2(0.0f,1.0f), Vector2(0.0f,0.0f), Vector2(1.0f,0.0f), Vector2(1.0f,1.0f) });
	newentry.mesh->SetVertexIndices({ 0,1,2,2,3,0 });
	newentry.mesh->UploadToGPU();

	UIEntries.push_back(newentry);

}
void GameTechRenderer::UpdateUI() {

	for (const auto& element : UIEntries) {
		delete element.mesh;
	}
	UIEntries.clear();

}

const std::vector<GameTechRenderer::UIen>& GameTechRenderer::GetUIEntries() {
	return UIEntries;
}

