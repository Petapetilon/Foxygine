#include "TextureLibrary.h"
#include "Texture.h"



std::map<std::string, Texture*> TextureLibrary::textures;


bool TextureLibrary::RegisterTexture(Texture* tex)
{
	if (textures.find(tex->GetFileName()) == textures.end()) {
		std::cout << "Registering Texture" << tex->GetFileName() << std::endl;
		textures[tex->GetFileName()] = tex;
		return true;
	}
	
	return false;
}


void TextureLibrary::UnregisterTexture(Texture* tex)
{
}


Texture* TextureLibrary::FindTexture(std::string file)
{
	if(textures.find(file) != textures.end())
		return textures[file];
	return nullptr;
}


bool TextureLibrary::TryFindTexture(std::string file, Texture* result)
{
	if (textures.find(file) != textures.end()) {
		result = textures[file];
		return true;
	}

	result = nullptr;
	return false;
}


Texture* TextureLibrary::FindTextureByName(std::string textureName)
{
	for (auto tex : textures) {
		if (tex.first == textureName) {
			return tex.second;
		}
	}

	return nullptr;
}


bool TextureLibrary::TryFindTextureByName(std::string textureName, Texture* result)
{
	for (auto tex : textures) {
		if (tex.first == textureName) {
			result = tex.second;
			return true;
		}
	}

	result = nullptr;
	return false;
}
