#include "Texture2D.h"
#include "../Shaders/Shader.h"
#include <iostream>
#include "TextureLibrary.h"


#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // !STB_IMAGE_IMPLEMENTATION


void Texture2D::LoadImage()
{
	texData = stbi_load(textureFile.c_str(), &texWidth, &texHeight, &colorChannels, 0);
}


void Texture2D::GL_RegisterImage()
{
	glBindTexture(GL_TEXTURE_2D, GL_TextureID);
	SetWrapping(setWrapping);
	SetFiltering(setFiltering);

	if (texData) {
		if (colorChannels == 3) {
			GL_Call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData));
			//GL_Call(gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texWidth, texHeight, GL_RGB, GL_UNSIGNED_BYTE, texData));
		}
		else if (colorChannels == 4) {
			GL_Call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData));
			//GL_Call(gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texWidth, texHeight, GL_RGBA, GL_UNSIGNED_BYTE, texData));
		}
	
		GL_Call(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else {
		std::cout << " - failed!" << std::endl;
		std::cout << "Texture could not be Loaded from: " << textureFile << std::endl;
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
	name = _name;
	textureFile = _filePath;
	setWrapping = textureWrapping;
	setFiltering = textureFiltering;

	//Check wether this texture was already loaded
	if (!TextureLibrary::RegisterTexture(this)) {
		std::cout << "Copying Data from existing Texture" << std::endl;
		GL_CopyData(TextureLibrary::FindTexture(textureFile));
		loadingFinished = true;
		return true;
	}

	TextureLibrary::RegisterTexture(this);
	glGenTextures(1, &GL_TextureID);

	std::cout << "Loading Texture Resource: " << _filePath << std::endl;
	stbi_set_flip_vertically_on_load(true);
	loadingThread = std::thread(&Texture2D::LoadImage, this);
	loadingFinished = false;
	return FinishLoading();
}


bool Texture2D::LoadTexture2D(std::string _filePath, std::string _name, Wrapping textureWrapping = Wrapping::Repeat, Filtering textureFiltering = Filtering::Linear)
{
	name = _name;
	textureFile = _filePath;
	setWrapping = textureWrapping;
	setFiltering = textureFiltering;
	
	//Check wether this texture was already loaded
	if (!TextureLibrary::RegisterTexture(this)) {
		std::cout << "Copying Data from existing Texture" << std::endl;
		GL_CopyData(TextureLibrary::FindTexture(textureFile));
		loadingFinished = true;
		return true;
	}

	if (loadingThread.joinable()) {
		std::cout << "Wait for Texture Resource to finish loading: " << _filePath << " - aborted" << std::endl;
		loadingThread.join();
	}

	std::cout << "Loading Texture Resource in background: " << _filePath << std::endl;
	stbi_set_flip_vertically_on_load(true);

	TextureLibrary::RegisterTexture(this);
	glGenTextures(1, &GL_TextureID);
	loadingThread = std::thread(&Texture2D::LoadImage, this);
	return true;
}


bool Texture2D::FinishLoading()
{
	if (loadingFinished) return true;

	std::cout << "Waiting for Texture Resource to finish loading: " << textureFile;
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
		GL_Call(glActiveTexture(GL_TEXTURE0 + GL_TextureIndex));
		glBindTexture(GL_TEXTURE_2D, GL_TextureID);
	}
	else {
		std::cout << "OpenGL Texture Index out of Bounds:" << GL_TextureIndex << std::endl;
	}
}

