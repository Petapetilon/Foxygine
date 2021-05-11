#pragma once
#include <string>
#include <memory>
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
	unsigned char* texData;
	unsigned int GL_TextureID;
	unsigned int GL_UniformLocation;
	Wrapping setWrapping;
	Filtering setFiltering;

public:
	std::string name;

	virtual bool FinishLoading() { return true; }
	virtual void SetWrapping(Wrapping) {}
	virtual void SetFiltering(Filtering) {}
	virtual void FreeImageData() {}
	virtual void GL_GetUniform(std::shared_ptr<Shader> shader, std::string uniformName) {}
	virtual void GL_BindTexture(unsigned int) {}
};