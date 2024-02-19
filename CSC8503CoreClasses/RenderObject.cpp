#include "RenderObject.h"
#include "Mesh.h"

using namespace NCL::CSC8503;
using namespace NCL;

RenderObject::RenderObject(Transform* parentTransform, Mesh* mesh, Texture* tex, Shader* shader) {
	if (!tex) {
		bool a = true;
	}
	for (int i = 0; i < 6; i++)
		this->texture[i] = nullptr;
	this->transform	= parentTransform;
	this->mesh		= mesh;
	this->texture[0] = tex;
	this->shader	= shader;
	this->colour	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	this->tag = "";
}

RenderObject::~RenderObject() {

}