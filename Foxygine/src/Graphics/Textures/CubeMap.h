#pragma once
#include "Texture.h"
#include <vector>


class CubeMap : public Texture
{
protected:
	int colorChannels;

public:
	void LoadCubeMap(std::vector<std::string> filePaths, std::string _name, Wrapping, Filtering);
	void SetWrapping(Wrapping) override;
	void SetFiltering(Filtering) override;
	void FreeImageData() override;
	void GL_GetUniform(std::shared_ptr<Shader> shader, std::string uniformName) override;
	void GL_BindTexture(unsigned int) override;
	//static void LoadSingleImage(unsigned char* data, std::string file, int*, int* ,int*);
};

