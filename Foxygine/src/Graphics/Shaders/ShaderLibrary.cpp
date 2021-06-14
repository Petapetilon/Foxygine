#include "ShaderLibrary.h"
#include <iostream>
#include "../Rendering/Camera.h"


std::list<std::shared_ptr<Shader>> ShaderLibrary::registeredShaders;


bool ShaderLibrary::RegisterShader(Shader* _shader)
{
	for (auto shader : registeredShaders) {
		if (shader->name == _shader->name) {
			return false;
		}
	}

	std::cout << "Shader registered: " << _shader->name << std::endl;
 	registeredShaders.push_back(std::shared_ptr<Shader>(_shader));
	return true;
}

void ShaderLibrary::UnregisterShader(Shader* _shader)
{
	for (auto shader : registeredShaders) {
		if (shader->name == _shader->name) {
			registeredShaders.remove(shader);
		}
	}
}

std::shared_ptr<Shader> ShaderLibrary::GetShader(std::string name)
{
	for (auto shader : registeredShaders) {
		if (shader->name == name) {
			return std::shared_ptr<Shader>(shader);
		}
	}

	std::cout << "Shader not found: " << name << std::endl;
	return nullptr;
}

bool ShaderLibrary::TryGetShader(std::string name, std::shared_ptr<Shader>& result)
{
	for (auto shader : registeredShaders) {
		if (shader->name == name) {
			result = std::shared_ptr<Shader>(shader);
			return true;
		}
	}

	std::cout << "Shader not found: " << name << std::endl;
	result = nullptr;
	return false;
}


std::shared_ptr<Shader> ShaderLibrary::GL_BindWireframeShader()
{
	auto shader = GetShader("Internal_Wireframe");
	if (shader == nullptr) {
		shader = Shader::CreateEmptyShader("Internal_Wireframe");
		shader->LoadShaderResource("res\\VertexShader\\BasicVert.vert", Shader::ShaderType::VertexShader);
		shader->LoadShaderResource("res\\FragmentShader\\UnlitColor.frag", Shader::ShaderType::FragmentShader);
	}

	shader->GL_BindProgram();
	return std::shared_ptr<Shader>(shader);
}

std::shared_ptr<Shader> ShaderLibrary::GL_BindTessellatedWireframeShader()
{
	auto shader = GetShader("Internal_TessWireframe");
	if (shader == nullptr) {
		shader = Shader::CreateEmptyShader("Internal_TessWireframe");
		shader->LoadShaderResource("res\\VertexShader\\BasicVert.vert", Shader::ShaderType::VertexShader);
		shader->LoadShaderResource("res\\FragmentShader\\UnlitColor.frag", Shader::ShaderType::FragmentShader);
		shader->LoadShaderResource("res\\TessellationShader\\BasicTessControl.tesc", Shader::ShaderType::TessControlShader);
		shader->LoadShaderResource("res\\TessellationShader\\BasicTessEval.tese", Shader::ShaderType::TessEvalShader);
	}

	shader->GL_BindProgram();
	return std::shared_ptr<Shader>(shader);
}
