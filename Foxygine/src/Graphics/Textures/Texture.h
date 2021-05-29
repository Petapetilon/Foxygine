#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../GL.h"



class Shader;

class Texture {
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

protected:
	std::string textureFile;
	unsigned char* texData;
	unsigned int GL_TextureID;
	unsigned int GL_UniformLocation;
	Wrapping setWrapping;
	Filtering setFiltering;

	void GL_CopyData(Texture* sourceTexture) {
		name = sourceTexture->name;
		texData = sourceTexture->texData;
		GL_TextureID = sourceTexture->GL_TextureID;
		GL_UniformLocation = sourceTexture->GL_UniformLocation;
		setWrapping = sourceTexture->setWrapping;
		setFiltering = sourceTexture->setFiltering;
	}

public:
	std::string name;

	std::string GetFileName() { return textureFile; }

	virtual bool FinishLoading() = 0;
	virtual void SetWrapping(Wrapping) = 0;
	virtual void SetFiltering(Filtering) = 0;
	virtual void FreeImageData() = 0;
	virtual void GL_GetUniform(std::shared_ptr<Shader> shader, std::string uniformName) = 0;
	virtual void GL_BindTexture(unsigned int) = 0;
};