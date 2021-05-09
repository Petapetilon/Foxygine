#pragma once
#include "Texture.h"
#include <vector>
#include <thread>


class CubeMap : public Texture
{
	class CubeMapTexture {
	private:
		unsigned char* texData;
		int texWidth;
		int texHeight;
		int colorChannels;
		std::string filePath;
		std::thread loadingThread;
		
		void LoadImage();

	public:
		void LoadThreaded(std::string);
		void FinishLoading(int index);
	};

protected:
	int colorChannels;
	CubeMapTexture* textures;
	bool loadingFinished;

	void FinishLoading();

public:
	void LoadCubeMapInline(std::vector<std::string> filePaths, std::string _name, Wrapping, Filtering);
	void LoadCubeMapOptimized(std::vector<std::string> filePaths, std::string _name, Wrapping, Filtering);
	void SetWrapping(Wrapping) override;
	void SetFiltering(Filtering) override;
	void FreeImageData() override;
	void GL_GetUniform(std::shared_ptr<Shader> shader, std::string uniformName) override;
	void GL_BindTexture(unsigned int) override;
};

