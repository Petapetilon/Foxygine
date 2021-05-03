#include "Shader.h"
#include "ShaderPass.h"
#include "../Camera.h"
#include "../../GameObject/Transform.h"
#include "ShaderLibrary.h"
#include "ShaderHandeling.h"
#include <iostream>
#include <typeinfo>



Shader::Shader(std::string _name)
{
	name = _name;
	if (!ShaderLibrary::RegisterShader(this)) {
		std::cout << "Shader with this name already Exists!" << std::endl;
		name = _name + "-Dup";
		std::cout << "Renamed to: " << name << std::endl;
		ShaderLibrary::RegisterShader(this);
 	}

	GL_ShaderProgram = glCreateProgram();








	std::string vertexShaderSource;
	ShaderHandeling::LoadShaderSourceCode("res\\VertexShader\\BasicVert.vert", vertexShaderSource);
	auto vertexShader = ShaderHandeling::CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	ShaderHandeling::BindShaderToProgram(GL_ShaderProgram, vertexShader);

	std::string fragmentShaderSource;
	ShaderHandeling::LoadShaderSourceCode("res\\FragmentShader\\BasicFrag.frag", fragmentShaderSource);
	auto fragmentShader = ShaderHandeling::CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	ShaderHandeling::BindShaderToProgram(GL_ShaderProgram, fragmentShader);
}

Shader::Shader()
{
	name = "unnamedShader";
	if (!ShaderLibrary::RegisterShader(this)) {
		std::cout << "Shader with this name already Exists!" << std::endl;
		name = "unnamedShaderDup";
		std::cout << "Renamed to: " << name << std::endl;
		ShaderLibrary::RegisterShader(this);
	}

	GL_ShaderProgram = glCreateProgram();
}

unsigned int Shader::GL_GetShaderProgram() {
	return GL_ShaderProgram; 
}



void Shader::GL_BindProgram() { 
	glUseProgram(GL_ShaderProgram); 
}



void Shader::GL_SetUniforms() {
	for (auto pass : shaderPasses) {
		pass->SetShaderPass();
	}
}


void Shader::AddShaderPass(ShaderPass* newPass)
{
	for (auto pass : shaderPasses) {
		if (pass == newPass) {
			return;
		}
	}

	newPass->CreateShaderPass(this);
	shaderPasses.push_back(newPass);
}



void Shader::SetShaderPass(ShaderPass* pass)
{
	auto foundPass = GetShaderPass(pass->GetUniformName());


	if (foundPass != nullptr) {
		foundPass->CopyShaderPassValue(pass);
		delete[] pass;
	}
	else {
		AddShaderPass(pass);
	}
}



ShaderPass* Shader::GetShaderPass(std::string uniformName)
{
	for (auto pass : shaderPasses) {
		if (pass->GetUniformName() == uniformName) {
			return pass;
		}
	}

	return nullptr;
}
