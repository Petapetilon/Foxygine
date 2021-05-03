#include "BasicShader.h"
#include "ShaderHandeling.h"
#include "../../GameObject/Transform.h"
#include "../Camera.h"
#include "glm.hpp"
#include "ShaderPass.h"
#include "ShaderLibrary.h"


BasicShader::BasicShader(std::string _name)
{
	std::string vertexShaderSource;
	ShaderHandeling::LoadShaderSourceCode("res\\VertexShader\\BasicVert.vert", vertexShaderSource);
	auto vertexShader = ShaderHandeling::CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	ShaderHandeling::BindShaderToProgram(GL_ShaderProgram, vertexShader);

	std::string fragmentShaderSource;
	ShaderHandeling::LoadShaderSourceCode("res\\FragmentShader\\BasicFrag.frag", fragmentShaderSource);
	auto fragmentShader = ShaderHandeling::CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	ShaderHandeling::BindShaderToProgram(GL_ShaderProgram, fragmentShader);
}

BasicShader::BasicShader()
{

}
