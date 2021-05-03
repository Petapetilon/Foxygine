#include "ShaderHandeling.h"


void ShaderHandeling::LoadShaderSourceCode(const std::string& filepath, std::string& source) {
	std::ifstream stream(filepath);
	source = { std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() };
	//std::cout << source << std::endl << std::endl;
}

unsigned int ShaderHandeling::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile: " << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

void ShaderHandeling::BindShaderToProgram(unsigned int programid, unsigned int shaderid) {
	glAttachShader(programid, shaderid);
	glLinkProgram(programid);
	glValidateProgram(programid);
}