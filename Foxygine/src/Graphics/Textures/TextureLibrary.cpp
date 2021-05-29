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
	return textures[file];
}
