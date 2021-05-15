#include "Texture2D.h"
#include "../Shaders/Shader.h"
#include <iostream>
#include "../Lights/ShadowMap.h"


#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // !STB_IMAGE_IMPLEMENTATION


void Texture2D::LoadImage()
{
	texData = stbi_load(filePath.c_str(), &texWidth, &texHeight, &colorChannels, 0);
}


void Texture2D::GL_RegisterImage()
{
	glBindTexture(GL_TEXTURE_2D, GL_TextureID);
	SetWrapping(setWrapping);
	SetFiltering(setFiltering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (texData) {
		if (colorChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		}
		else if (colorChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		}
	
		GL_Call(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else {
		std::cout << " - failed!" << std::endl;
		std::cout << "Texture could not be Loaded from: " << filePath << std::endl;
	}
}


Texture2D::Texture2D()
{
	GL_TextureID = -2;
	texData == nullptr;
	texWidth = 0;
	texHeight = 0;
	colorChannels = 0;
	name = "unnamedTexture";
}


Texture2D::~Texture2D()
{
	FreeImageData();
}


bool Texture2D::LoadTexture2DInline(std::string _filePath, std::string _name, Wrapping textureWrapping = Wrapping::Repeat, Filtering textureFiltering = Filtering::Linear)
{
	std::cout << "Loading Texture Resource: " << _filePath << std::endl;
	stbi_set_flip_vertically_on_load(true);
	name = _name;
	setWrapping = textureWrapping;
	setFiltering = textureFiltering;

	glGenTextures(1, &GL_TextureID);

	filePath = _filePath;
	loadingThread = std::thread(&Texture2D::LoadImage, this);
	loadingFinished = false;
	return FinishLoading();
}


void Texture2D::LoadTexture2D(std::string _filePath, std::string _name, Wrapping textureWrapping = Wrapping::Repeat, Filtering textureFiltering = Filtering::Linear)
{
	filePath = _filePath;
	if (loadingThread.joinable()) {
		std::cout << "Wait for Texture Resource to finish loading: " << _filePath << " - aborted" << std::endl;
		loadingThread.join();
	}

	std::cout << "Loading Texture Resource in background: " << _filePath << std::endl;
	stbi_set_flip_vertically_on_load(true);
	name = _name;

	glGenTextures(1, &GL_TextureID);
	loadingThread = std::thread(&Texture2D::LoadImage, this);
}


bool Texture2D::FinishLoading()
{
	if (loadingFinished) return true;

	std::cout << "Waiting for Texture Resource to finish loading: " << filePath;
	if (loadingThread.joinable()) {
		loadingThread.join();
	}
	else {
		std::cout << " - failed!" << std::endl;
		loadingFinished = true;
		return false;
	}

	if (texData) {
		GL_RegisterImage();
		std::cout << " - success!" << std::endl;
		stbi_image_free(texData);
	}
	else {
		std::cout << " - failed!" << std::endl;
		loadingFinished = true;
		return false;
	}

	loadingFinished = true;
	return true;
}


void Texture2D::SetWrapping(Wrapping wrappingMode)
{
	if (GL_TextureID == -2) {
		std::cout << "Texture was not inited!" << std::endl;
	}
	else if (GL_TextureID == -1) {
		std::cout << "Texture couldnt be bound to OpenGL!" << std::endl;
	}

	setWrapping = wrappingMode;
	glBindTexture(GL_TEXTURE_2D, GL_TextureID);

	switch (wrappingMode)
	{
	case Texture2D::Wrapping::Repeat:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;

	case Texture2D::Wrapping::Mirror:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;

	case Texture2D::Wrapping::ClampEdge:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;

	case Texture2D::Wrapping::ClampBorder:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		break;

	default:
		break;
	}
}


void Texture2D::SetFiltering(Filtering textureFiltering)
{
	if (GL_TextureID == -2) {
		std::cout << "Texture was not inited!" << std::endl;
	}
	else if (GL_TextureID == -1) {
		std::cout << "Texture couldnt be bound to OpenGL!" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, GL_TextureID);
	setFiltering = textureFiltering;

	if (textureFiltering == Filtering::Nearest) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}


void Texture2D::FreeImageData()
{
	if (texData) {
		stbi_image_free(texData);
	}

	glDeleteTextures(1, &GL_TextureID);
}


void Texture2D::GL_GetUniform(std::shared_ptr<Shader> shader, std::string uniformName)
{
	shader->GL_BindProgram();
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), uniformName.c_str());
}


void Texture2D::GL_BindTexture(unsigned int GL_TextureIndex)
{
	if(!FinishLoading()) return;

	if (GL_TextureIndex <= 31) {
		//glUniform1i(GL_UniformLocation, GL_TextureIndex);
		GL_Call(glActiveTexture(GL_TEXTURE0 + GL_TextureIndex));
		glBindTexture(GL_TEXTURE_2D, GL_TextureID);
		//glBindTexture(GL_TEXTURE_2D, ShadowMap::GL_ShadowDepthMap);
	}
	else {
		std::cout << "OpenGL Texture Index out of Bounds:" << GL_TextureIndex << std::endl;
	}
}

