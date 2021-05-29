#pragma once
#include <map>
#include <string>
#include <memory>


class Texture;


class TextureLibrary
{
private:
	static std::map<std::string, Texture*> textures;

public:
	static bool RegisterTexture(Texture*);
	static void UnregisterTexture(Texture*);
	static Texture* FindTexture(std::string);
};

