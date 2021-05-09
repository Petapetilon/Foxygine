#include "Texture2D.h"
#include "../Shaders/Shader.h"
#include <iostream>


#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
//#define STBI_NO_THREAD_LOCALS
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
	
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
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


void Texture2D::LoadTexture2DInline(std::string _filePath, std::string _name, Wrapping textureWrapping = Wrapping::Repeat, Filtering textureFiltering = Filtering::Linear)
{
	std::cout << "Loading Texture Resource: " << filePath << std::endl;
	stbi_set_flip_vertically_on_load(true);
	name = _name;
	setWrapping = textureWrapping;
	setFiltering = textureFiltering;

	glGenTextures(1, &GL_TextureID);

	filePath = _filePath;
	texData = stbi_load(filePath.c_str(), &texWidth, &texHeight, &colorChannels, 0);

	GL_RegisterImage();
}

void Texture2D::LoadTexture2DOptimized(std::string _filePath, std::string _name, Wrapping textureWrapping = Wrapping::Repeat, Filtering textureFiltering = Filtering::Linear)
{
	std::cout << "Loading Texture Resource: " << _filePath << std::endl;
	stbi_set_flip_vertically_on_load(true);
	name = _name;

	glGenTextures(1, &GL_TextureID);

	filePath = _filePath;
	loadingThread = std::thread(&Texture2D::LoadImage, this);
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
	if (loadingThread.joinable()) {
		std::cout << "Waiting for Texture Resource to finish loading: " << filePath << std::endl;
		loadingThread.join();

		glBindTexture(GL_TEXTURE_2D, GL_TextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GL_RegisterImage();
	}

	if (GL_TextureIndex <= 31) {
		glUniform1i(GL_UniformLocation, GL_TextureIndex);
		glActiveTexture(GL_TEXTURE0 + GL_TextureIndex);
		glBindTexture(GL_TEXTURE_2D, GL_TextureID);
	}
	else {
		std::cout << "OpenGL Texture Index out of Bounds:" << GL_TextureIndex << std::endl;
	}
}

