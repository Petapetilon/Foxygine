#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <istream>
#include "../GL.h"

//#define GLEW_STATIC
//#include <GL/glew.h>
//
//#define GLEW_ALREADY_INCLUDED


class ShaderHandeling
{
public:
	static void LoadShaderSourceCode(const std::string& filepath, std::string& source);

	static unsigned int CompileShader(unsigned int type, const std::string& source);

	static void BindShaderToProgram(unsigned int programid, unsigned int shaderid);
};

