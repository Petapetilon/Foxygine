#pragma once
#include "../GL.h"
#include <string>
#include <memory>


class Shader;

class Texture
{
protected :
	unsigned char* texData;
	unsigned int GL_TextureID;
	unsigned int GL_UniformLocation;

	int texWidth;
	int texHeight;
	int colorChannels;

public:
	enum class Wrapping {
		Repeat,
		Mirror,
		ClampEdge,
		ClampBorder
	};

	enum class Filtering {
		Nearest,
		Linear
	};



	std::string name;

	Texture();
	~Texture();
	void LoadTexture(std::string filePath, std::string _name, Wrapping, Filtering);
	void SetWrapping(Wrapping);
	void SetFiltering(Filtering);
	void FreeImageData();
	void GL_GetUniform(std::shared_ptr<Shader> shader, std::string uniformName);
	void GL_BindTexture(unsigned int);
};

