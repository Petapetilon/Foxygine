#pragma once
#include "Texture.h"
#include <thread>
#include <future>
#include <functional>


class Shader;

class Texture2D : public Texture
{
protected :
	int texWidth;
	int texHeight;
	int colorChannels;
	std::string filePath;
	std::thread loadingThread;
	bool loadingFinished;

	void LoadImage();
	void GL_RegisterImage();

public:
	Texture2D();
	~Texture2D();
	bool LoadTexture2DInline(std::string filePath, std::string _name, Wrapping, Filtering);
	void LoadTexture2D(std::string filePath, std::string _name, Wrapping, Filtering);
	bool FinishLoading() override;
	void SetWrapping(Wrapping) override;
	void SetFiltering(Filtering) override;
	void FreeImageData() override;
	void GL_GetUniform(std::shared_ptr<Shader> shader, std::string uniformName) override;
	void GL_BindTexture(unsigned int) override;
};

