#include "CubeMap.h"
#include <iostream>
#include "../Shaders/ShaderPass.h"
#include "../Shaders/Shader.h"
#include <thread>


//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#endif // !STB_IMAGE_IMPLEMENTATION


/// <summary>
/// right, left, top, bottom, front, back
/// </summary>
void CubeMap::LoadCubeMap(std::vector<std::string> filePaths, std::string _name, Wrapping textureWrapping = Wrapping::Repeat, Filtering textureFiltering = Filtering::Linear)
{
	stbi_set_flip_vertically_on_load(true);
	name = _name;

	glGenTextures(1, &GL_TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, GL_TextureID);

	int texWidth = 0, texHeight = 0;
	//unsigned char* data0;
	//unsigned char* data1;
	//unsigned char* data2;
	//unsigned char* data3;
	//unsigned char* data4;
	//unsigned char* data5;
	//int* width0, *height0, *colChan0;
	//int* width1, *height1, *colChan1;
	//int* width2, *height2, *colChan2;
	//int* width3, *height3, *colChan3;
	//int* width4, *height4, *colChan4;
	//int* width5, *height5, *colChan5;

	//std::thread load0(CubeMap::LoadSingleImage, std::ref(data0), std::ref(filePaths[0]), std::ref(width0), std::ref(height0), std::ref(colChan0));
	//std::thread load1(CubeMap::LoadSingleImage, std::ref(data1), std::ref(filePaths[1]), std::ref(width1), std::ref(height1), std::ref(colChan1));
	//std::thread load2(CubeMap::LoadSingleImage, std::ref(data2), std::ref(filePaths[2]), std::ref(width2), std::ref(height2), std::ref(colChan2));
	//std::thread load3(CubeMap::LoadSingleImage, std::ref(data3), std::ref(filePaths[3]), std::ref(width3), std::ref(height3), std::ref(colChan3));
	//std::thread load4(CubeMap::LoadSingleImage, std::ref(data4), std::ref(filePaths[4]), std::ref(width4), std::ref(height4), std::ref(colChan4));
	//std::thread load5(CubeMap::LoadSingleImage, std::ref(data5), std::ref(filePaths[5]), std::ref(width5), std::ref(height5), std::ref(colChan5));

	//load0.join();
	//load1.join();
	//load2.join();
	//load3.join();
	//load4.join();
	//load5.join();


	//if (data0) {
	//	if (*colChan0 == 3) {
	//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, *width0, *height0, 0, GL_RGB, GL_UNSIGNED_BYTE, data0);
	//	}
	//	else if (*colChan0 == 4) {
	//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, *width0, *height0, 0, GL_RGBA, GL_UNSIGNED_BYTE, data0);
	//	}
	//
	//	stbi_image_free(data0);
	//}
	//else {
	//	//std::cout << "Texture could not be Loaded from: " << filePaths[i] << std::endl;
	//}

	//if (data1) {
	//	if (*colChan0 == 3) {
	//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGB, *width1, *height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
	//	}
	//	else if (*colChan0 == 4) {
	//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGBA, *width1, *height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
	//	}
	//
	//	stbi_image_free(data1);
	//}
	//else {
	//	//std::cout << "Texture could not be Loaded from: " << filePaths[i] << std::endl;
	//}
	//
	//if (data2) {
	//	if (*colChan2 == 3) {
	//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGB, *width2, *height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
	//	}
	//	else if (*colChan2 == 4) {
	//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGBA, *width2, *height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
	//	}
	//
	//	stbi_image_free(data2);
	//}
	//else {
	//	//std::cout << "Texture could not be Loaded from: " << filePaths[i] << std::endl;
	//}
	//
	//if (data3) {
	//	if (*colChan3 == 3) {
	//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGB, *width3, *height3, 0, GL_RGB, GL_UNSIGNED_BYTE, data3);
	//	}
	//	else if (*colChan3 == 4) {
	//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGBA,* width3, *height3, 0, GL_RGBA, GL_UNSIGNED_BYTE, data3);
	//	}
	//
	//	stbi_image_free(data3);
	//}
	//else {
	//	//std::cout << "Texture could not be Loaded from: " << filePaths[i] << std::endl;
	//}
	//
	//if (data4) {
	//	if (*colChan4 == 3) {
	//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGB, *width4, *height4, 0, GL_RGB, GL_UNSIGNED_BYTE, data4);
	//	}
	//	else if (*colChan4 == 4) {
	//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGBA,* width4, *height4, 0, GL_RGBA, GL_UNSIGNED_BYTE, data4);
	//	}
	//
	//	stbi_image_free(data4);
	//}
	//else {
	//	//std::cout << "Texture could not be Loaded from: " << filePaths[i] << std::endl;
	//}
	//
	//if (data5) {
	//	if (*colChan5 == 3) {
	//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGB,* width5,* height5, 0, GL_RGB, GL_UNSIGNED_BYTE, data5);
	//	}
	//	else if (*colChan5 == 4) {
	//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGBA,* width5, *height5, 0, GL_RGBA, GL_UNSIGNED_BYTE, data5);
	//	}
	//
	//	stbi_image_free(data5);
	//}
	//else {
	//	//std::cout << "Texture could not be Loaded from: " << filePaths[i] << std::endl;
	//}


















	for (int i = 0; i < filePaths.size(); i++) {
		texData = stbi_load(filePaths[i].c_str(), &texWidth, &texHeight, &colorChannels, 0);
		if (texData) {
			if (colorChannels == 3) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
			}
			else if (colorChannels == 4) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
			}
	
			stbi_image_free(texData);
		}
		else {
			std::cout << "Texture could not be Loaded from: " << filePaths[i] << std::endl;
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
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
	if (GL_TextureIndex <= 15) {
		glUniform1i(GL_UniformLocation, GL_TextureIndex);
		glActiveTexture(GL_TEXTURE0 + GL_TextureIndex);
		glBindTexture(GL_TEXTURE_CUBE_MAP, GL_TextureID);
	}
	else {
		std::cout << "OpenGL Texture Index out of Bounds:" << GL_TextureIndex << std::endl;
	}
}

//void CubeMap::LoadSingleImage(unsigned char* data, std::string file, int* w, int* h, int* cc)
//{
//	data = stbi_load(file.c_str(), w, h, cc, 0);
//}
