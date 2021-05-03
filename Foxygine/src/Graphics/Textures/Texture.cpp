#include "Texture.h"
#include "../Shaders/Shader.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



Texture::Texture()
{
	GL_TextureID = -2;
	texData == nullptr;
	texWidth = 0;
	texHeight = 0;
	colorChannels = 0;
	name = "unnamedTexture";
}

Texture::~Texture()
{
	FreeImageData();
}


void Texture::LoadTexture(std::string filePath, std::string _name, Wrapping textureWrapping = Wrapping::Repeat, Filtering textureFiltering = Filtering::Linear)
{
	stbi_set_flip_vertically_on_load(true);
	name = _name;

	glGenTextures(1, &GL_TextureID);
	glBindTexture(GL_TEXTURE_2D, GL_TextureID);
	SetWrapping(textureWrapping);
	SetFiltering(textureFiltering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texData = stbi_load(filePath.c_str(), &texWidth, &texHeight, &colorChannels, 0);
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


void Texture::SetWrapping(Wrapping wrappingMode)
{
	if (GL_TextureID == -2) {
		std::cout << "Texture was not inited!" << std::endl;
	}
	else if (GL_TextureID == -1) {
		std::cout << "Texture couldnt be bound to OpenGL!" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, GL_TextureID);

	switch (wrappingMode)
	{
	case Texture::Wrapping::Repeat:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;

	case Texture::Wrapping::Mirror:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;

	case Texture::Wrapping::ClampEdge:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;

	case Texture::Wrapping::ClampBorder:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		break;

	default:
		break;
	}
}

void Texture::SetFiltering(Filtering textureFiltering)
{
	if (GL_TextureID == -2) {
		std::cout << "Texture was not inited!" << std::endl;
	}
	else if (GL_TextureID == -1) {
		std::cout << "Texture couldnt be bound to OpenGL!" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, GL_TextureID);

	if (textureFiltering == Texture::Filtering::Nearest) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

void Texture::FreeImageData()
{
	if (texData) {
		stbi_image_free(texData);
	}
}


void Texture::GL_GetUniform(std::shared_ptr<Shader> shader, std::string uniformName)
{
	shader->GL_BindProgram();
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), uniformName.c_str());
}


void Texture::GL_BindTexture(unsigned int GL_TextureIndex)
{
	if (GL_TextureIndex <= 15) {
		glUniform1i(GL_UniformLocation, GL_TextureIndex);
		glActiveTexture(GL_TEXTURE0 + GL_TextureIndex);
		glBindTexture(GL_TEXTURE_2D, GL_TextureID);
	}
	else {
		std::cout << "OpenGL Texture Index out of Bounds:" << GL_TextureIndex << std::endl;
	}
}

