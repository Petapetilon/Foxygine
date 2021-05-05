#include "Shader.h"
#include "ShaderPass.h"
#include "../Camera.h"
#include "../../GameObject/Transform.h"
#include "ShaderLibrary.h"
#include "ShaderHandeling.h"
#include "../Graphics.h"
#include <iostream>
#include <typeinfo>



Shader::Shader(std::string _name)
{
	name = _name;
	std::cout << "Creating Shader: " << name << std::endl;
	if (!ShaderLibrary::RegisterShader(this)) {
		std::cout << "Shader with this name already Exists!" << std::endl;
		name = _name + "-Dup";
		std::cout << "Renamed to: " << name << std::endl;
		//ShaderLibrary::RegisterShader(this);
 	}

	GL_ShaderProgram = glCreateProgram();
	shaderPasses.clear();
	//Graphics::GL_CurrentlyBoundShaderProgram = GL_ShaderProgram;







	
	//std::string vertexShaderSource;
	//ShaderHandeling::LoadShaderSourceCode("res\\VertexShader\\BasicVert.vert", vertexShaderSource);
	//auto vertexShader = ShaderHandeling::CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	//ShaderHandeling::BindShaderToProgram(GL_ShaderProgram, vertexShader);
	//
	//std::string fragmentShaderSource;
	//ShaderHandeling::LoadShaderSourceCode("res\\FragmentShader\\BasicFrag.frag", fragmentShaderSource);
	//auto fragmentShader = ShaderHandeling::CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	//ShaderHandeling::BindShaderToProgram(GL_ShaderProgram, fragmentShader);
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
	if (Graphics::GL_CurrentlyBoundShaderProgram != GL_ShaderProgram) {
		glUseProgram(GL_ShaderProgram); 
		Graphics::GL_CurrentlyBoundShaderProgram = GL_ShaderProgram;
		std::cout << "Binding shader program: " << name << std::endl;
	}
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

void Shader::LoadShaderResource(std::string shaderFilePath, ShaderType shaderType)
{
	GL_UnbindPrograms();
	std::string shaderSource;
	ShaderHandeling::LoadShaderSourceCode(shaderFilePath, shaderSource);
	unsigned int shaderID = -2;

	switch (shaderType)
	{
	case Shader::ShaderType::VertexShader:
		shaderID = ShaderHandeling::CompileShader(GL_VERTEX_SHADER, shaderSource);
		break;

	case Shader::ShaderType::GeometryShader:
		shaderID = ShaderHandeling::CompileShader(GL_GEOMETRY_SHADER, shaderSource);
		break;

	case Shader::ShaderType::FragmentShader:
		shaderID = ShaderHandeling::CompileShader(GL_FRAGMENT_SHADER, shaderSource);
		break;
	}

	std::cout << "Loading shader Resource: " << shaderFilePath << " with GLID: " << shaderID << " for " << name << std::endl;
	ShaderHandeling::BindShaderToProgram(GL_ShaderProgram, shaderID);
}

std::shared_ptr<Shader> Shader::CreateBasicLitShader(std::string _name)
{
	ShaderLibrary::RegisterShader(new Shader(_name));
	auto shader = ShaderLibrary::GetShader(_name);
	shader->LoadShaderResource("res\\VertexShader\\BasicVert.vert", ShaderType::VertexShader);
	shader->LoadShaderResource("res\\FragmentShader\\BasicFrag.frag", ShaderType::FragmentShader);
	return std::shared_ptr<Shader>(shader);
}

std::shared_ptr<Shader> Shader::CreateBasicUnlitShader(std::string _name)
{
	auto shader = ShaderLibrary::GetShader(_name);
	return std::shared_ptr<Shader>(shader);
}

std::shared_ptr<Shader> Shader::CreateBasicLitUIShader(std::string _name)
{
	auto shader = ShaderLibrary::GetShader(_name);
	return std::shared_ptr<Shader>(shader);
}

std::shared_ptr<Shader> Shader::CreateBasicUnlitUIShader(std::string _name)
{
	auto shader = ShaderLibrary::GetShader(_name);
	return std::shared_ptr<Shader>(shader);
}

void Shader::GL_UnbindPrograms()
{
	glUseProgram(-1);
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
