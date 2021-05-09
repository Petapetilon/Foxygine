#include "CubeMap.h"
#include <iostream>
#include "../Shaders/ShaderPass.h"
#include "../Shaders/Shader.h"
#include <thread>
#include "stb_image.h"


void CubeMap::FinishLoading()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, GL_TextureID);

	for (int i = 0; i < 6; i++) {
		textures[i].FinishLoading(i);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	loadingFinished = true;
}


/// <summary>
/// right, left, top, bottom, front, back
/// </summary>
void CubeMap::LoadCubeMapInline(std::vector<std::string> filePaths, std::string _name, Wrapping textureWrapping = Wrapping::Repeat, Filtering textureFiltering = Filtering::Linear)
{
	if (filePaths.size() != 6) {
		std::cout << "Failed to load CubeMap: Wrong number of File Paths, there need to be 6!" << std::endl;
		return;
	}

	loadingFinished = false;
	stbi_set_flip_vertically_on_load(true);
	name = _name;

	glGenTextures(1, &GL_TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, GL_TextureID);


	textures = new CubeMapTexture[6];
	int texWidth = 0, texHeight = 0;

	for (int i = 0; i < 6; i++) {
		std::cout << "Loading Texture Resource: " << filePaths[i] << std::endl;
		textures[i].LoadThreaded(filePaths[i]);
		textures[i].FinishLoading(i);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	loadingFinished = true;
}


/// <summary>
/// right, left, top, bottom, front, back
/// </summary>
void CubeMap::LoadCubeMapOptimized(std::vector<std::string> filePaths, std::string _name, Wrapping textureWrapping = Wrapping::Repeat, Filtering textureFiltering = Filtering::Linear)
{
	if (filePaths.size() != 6) {
		std::cout << "Failed to load CubeMap: Wrong number of File Paths, there need to be 6!" << std::endl;
		return;
	}

	loadingFinished = false;
	stbi_set_flip_vertically_on_load(true);
	name = _name;

	glGenTextures(1, &GL_TextureID);

	textures = new CubeMapTexture[6];
	int texWidth = 0, texHeight = 0;

	for (int i = 0; i < 6; i++) {
		std::cout << "Loading Texture Resource: " << filePaths[i] << std::endl;
		textures[i].LoadThreaded(filePaths[i]);
	}
}


void CubeMap::SetWrapping(Wrapping wrappingMode)
{
	if (GL_TextureID == -2) {
		std::cout << "Texture was not inited!" << std::endl;
	}
	else if (GL_TextureID == -1) {
		std::cout << "Texture couldnt be bound to OpenGL!" << std::endl;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, GL_TextureID);

	switch (wrappingMode)
	{
	case Wrapping::Repeat:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
		break;

	case Wrapping::Mirror:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
		break;

	case Wrapping::ClampEdge:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		break;

	case Wrapping::ClampBorder:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
		break;

	default:
		break;
	}
}


void CubeMap::SetFiltering(Filtering textureFiltering)
{
	if (GL_TextureID == -2) {
		std::cout << "Texture was not inited!" << std::endl;
	}
	else if (GL_TextureID == -1) {
		std::cout << "Texture couldnt be bound to OpenGL!" << std::endl;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, GL_TextureID);

	if (textureFiltering == Filtering::Nearest) {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}


void CubeMap::FreeImageData()
{
	if (texData) {
		stbi_image_free(texData);
	}

	glDeleteTextures(1, &GL_TextureID);
}


void CubeMap::GL_GetUniform(std::shared_ptr<Shader> shader, std::string uniformName)
{
	shader->GL_BindProgram();
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), uniformName.c_str());
}


void CubeMap::GL_BindTexture(unsigned int GL_TextureIndex)
{
	if (!loadingFinished) {
		FinishLoading();
	}

	if (GL_TextureIndex <= 31) {
		glUniform1i(GL_UniformLocation, GL_TextureIndex);
		glActiveTexture(GL_TEXTURE0 + GL_TextureIndex);
		glBindTexture(GL_TEXTURE_CUBE_MAP, GL_TextureID);
	}
	else {
		std::cout << "OpenGL Texture Index out of Bounds:" << GL_TextureIndex << std::endl;
	}
}


void CubeMap::CubeMapTexture::LoadImage()
{
	texData = stbi_load(filePath.c_str(), &texWidth, &texHeight, &colorChannels, 0);
}


void CubeMap::CubeMapTexture::LoadThreaded(std::string file)
{
	filePath = file;
	loadingThread = std::thread(&CubeMapTexture::LoadImage, this);
}


void CubeMap::CubeMapTexture::FinishLoading(int index)
{
	std::cout << "Waiting for Texture Resource to finish loading: " << filePath << std::endl;

	if (loadingThread.joinable()) {
		loadingThread.join();
	}

	if (texData) {
		if (colorChannels == 3) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		}
		else if (colorChannels == 4) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		}
	
		stbi_image_free(texData);
	}
	else {
		std::cout << "Texture could not be Loaded from: " << filePath << std::endl;
	}
}
