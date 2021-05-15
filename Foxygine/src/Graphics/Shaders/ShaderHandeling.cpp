#include "ShaderHandeling.h"


void ShaderHandeling::LoadShaderSourceCode(const std::string& filepath, std::string& source) {
	std::ifstream stream(filepath);
	source = { std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() };
	//std::cout << "Shader Source Code: " << std::endl << source << std::endl << std::endl;
}

unsigned int ShaderHandeling::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GL_Call(glShaderSource(id, 1, &src, nullptr));
	GL_Call(glCompileShader(id));

	int result;
	GL_Call(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (!result) {
		int length;
		GL_Call(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GL_Call(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile: " << message << std::endl;
		GL_Call(glDeleteShader(id));
		return 0;
	}

	return id;

	//std::cout << "Successfully compiled Shader Source Code" << std::endl;
}

void ShaderHandeling::BindShaderToProgram(unsigned int programid, unsigned int shaderid) {
	GL_Call(glAttachShader(programid, shaderid));
	GL_Call(glLinkProgram(programid));
	GL_Call(glValidateProgram(programid));
	//std::cout << "Successfully bound Shader to Program" << std::endl;
}