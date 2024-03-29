﻿/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION

#include "./stb/stb_image.h"

#include "Assets.h"
#include "TextureWriter.cpp"

using namespace NCL;
using namespace Rendering;

std::map<std::string, TextureLoadFunction> TextureLoader::fileHandlers;

std::unordered_map<std::string, char*> NCL::TextureLoader::textureCache;

bool TextureLoader::LoadTexture(const std::string& filename, char*& outData, int& width, int& height, int& channels, int& flags) {
	if (filename.empty()) {
		return false;
	}

	std::filesystem::path path(filename);

	std::string extension = path.extension().string();

	bool isAbsolute = path.is_absolute();

	auto it = fileHandlers.find(extension);

	std::string realPath = isAbsolute ? filename : Assets::TEXTUREDIR + filename;

	if (it != fileHandlers.end()) {
		// use cache to load textures
		if (it->second(realPath, outData, width, height, channels, flags)) {
			AddTextureToCache(realPath, outData);
			return true;
		}
	}
	else {
		//  stb_image 
		stbi_uc* texData = stbi_load(realPath.c_str(), &width, &height, &channels, 4);

		channels = 4;

		if (texData) {
			outData = reinterpret_cast<char*>(texData);
			return true;
		}
	}

	return false;
}

void TextureLoader::RegisterTextureLoadFunction(TextureLoadFunction f, const std::string&fileExtension) {
	fileHandlers.insert(std::make_pair(fileExtension, f));
}

std::string TextureLoader::GetFileExtension(const std::string& fileExtension) {
	std::filesystem::path p = std::filesystem::path(fileExtension);

	std::filesystem::path ext = p.extension();

	return ext.string();
}

void TextureLoader::DeleteTextureData(char* data) {
	free(data);
}
