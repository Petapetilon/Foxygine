#pragma once
#include "Shader.h"
#include <list>


class ShaderLibrary
{
private:
	static std::list<std::shared_ptr<Shader>> registeredShaders;

public:
	static bool RegisterShader(Shader* _shader);
	static void UnregisterShader(Shader* _shader);
	static std::shared_ptr<Shader> GetShader(std::string name);

	static std::shared_ptr<Shader> GL_BindWireframeShader();
	static std::shared_ptr<Shader> GL_BindTessellatedWireframeShader();
};

