#pragma once
#include "Texture.h"


class Shader;

class Texture2D : public Texture
{
protected :
	int texWidth;
	int texHeight;
	int colorChannels;

public:
	Texture2D();
	~Texture2D();
	void LoadTexture2D(std::string filePath, std::string _name, Wrapping, Filtering);
	void SetWrapping(Wrapping) override;
	void SetFiltering(Filtering) override;
	void FreeImageData() override;
	void GL_GetUniform(std::shared_ptr<Shader> shader, std::string uniformName) override;
	void GL_BindTexture(unsigned int) override;
};

