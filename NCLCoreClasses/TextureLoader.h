/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include <map>
#include <functional>
#include "./stb/stb_image.h"

namespace NCL {
	namespace Rendering {
		class Texture;
	}

	typedef std::function<bool(const std::string& filename, char*& outData, int& width, int &height, int &channels, int&flags)> TextureLoadFunction;

	typedef std::function<Rendering::Texture*(const std::string& filename)> APILoadFunction;

	class TextureLoader	{
	public:
		TextureLoader() {}

		~TextureLoader() {
			ClearCache();
		}

		static bool LoadTexture(const std::string& filename, char*& outData, int& width, int &height, int &channels, int&flags);

		static void RegisterTextureLoadFunction(TextureLoadFunction f, const std::string&fileExtension);

		static void DeleteTextureData(char* data);

	protected:

		static char* GetTextureFromCache(const std::string& filename) {
			auto it = textureCache.find(filename);
			return it != textureCache.end() ? it->second : nullptr;
		}

		static void AddTextureToCache(const std::string& filename, char* data) {
			textureCache[filename] = data;
		}

		static void ClearCache() {
			for (auto& pair : textureCache) {
				stbi_image_free(pair.second); // 释放纹理内存
			}
			textureCache.clear();
		}

		static std::string GetFileExtension(const std::string& fileExtension);

		static std::map<std::string, TextureLoadFunction> fileHandlers;

		static std::unordered_map<std::string, char*> textureCache;
	};
}

