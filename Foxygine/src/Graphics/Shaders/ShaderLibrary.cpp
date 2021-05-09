#include "ShaderLibrary.h"
#include <iostream>


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
