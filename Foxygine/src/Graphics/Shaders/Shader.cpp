#include "Shader.h"
#include "../Rendering/Camera.h"
#include "../../GameObject/Transform.h"
#include "ShaderLibrary.h"
#include "ShaderHandeling.h"
#include "../Graphics.h"
#include <iostream>
#include <typeinfo>
#include "../GL.h"



Shader::Shader(std::string _name)
{
	name = _name;
	std::cout << "Creating Shader: " << name << std::endl;
	if (!ShaderLibrary::RegisterShader(this)) {
		std::cout << "Shader with this name already Exists!" << std::endl;
		name = _name + "-Dup";
		std::cout << "Renamed to: " << name << std::endl;
 	}

	GL_ShaderProgram = glCreateProgram();
	shaderPasses.clear();
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
	if (Graphics::GL_GetCurrentlyBoundShader() == nullptr || Graphics::GL_GetCurrentlyBoundShader()->GL_GetShaderProgram() != GL_ShaderProgram) {
		GL_Call(glUseProgram(GL_ShaderProgram)); 
		Graphics::GL_SetCurrentlyBoundShader(this);
	}
}


#ifdef USE_CACHE
GLint Shader::GL_GetUniformLocation(const std::string& name)
{
	if (shaderUniformLocations.find(name) != shaderUniformLocations.end())
		return shaderUniformLocations[name];

	GLint uniformLoc = glGetUniformLocation(GL_ShaderProgram, name.c_str());
	shaderUniformLocations[name] = uniformLoc;
	return uniformLoc;
}
#endif


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

	std::cout << "Loading Shader Resource: " << shaderFilePath << " for " << name << std::endl;
	ShaderHandeling::BindShaderToProgram(GL_ShaderProgram, shaderID);

	//Testing for lit shader Property
	//GL_BindProgram();
	//if (glGetUniformLocation(GL_ShaderProgram, "u_LightColor[0]") > -1) {
	//	GL_IsLitShader = true;
	//}
	//else {
	//	GL_IsLitShader = false;
	//}

}


bool Shader::GetShaderLitType()
{
	return GL_IsLitShader;
}


void Shader::SetValueVec1(std::string uniformName, float value)
{
#ifdef USE_CACHE
	GL_Call(glUniform1f(GL_GetUniformLocation(uniformName), value));
#else
	GL_Call(glUniform1f(glGetUniformLocation(GL_ShaderProgram, uniformName.c_str()), value));
#endif
}


void Shader::SetValueVec2(std::string uniformName, Vector2 value)
{
#ifdef USE_CACHE
	GL_Call(glUniform2f(GL_GetUniformLocation(uniformName), value.x, value.y));
#else
	GL_Call(glUniform2f(glGetUniformLocation(GL_ShaderProgram, uniformName.c_str()), value.x, value.y));
#endif
}


void Shader::SetValueVec3(std::string uniformName, Vector3 value)
{
#ifdef USE_CACHE
	GL_Call(glUniform3f(GL_GetUniformLocation(uniformName), value.x, value.y, value.z));
#else
	GL_Call(glUniform3f(glGetUniformLocation(GL_ShaderProgram, uniformName.c_str()), value.x, value.y, value.z));
#endif
}


void Shader::SetValueVec4(std::string uniformName, Vector4 value)
{
#ifdef USE_CACHE

	GL_Call(glUniform4f(GL_GetUniformLocation(uniformName), value.x, value.y, value.z, value.w));
#else
	GL_Call(glUniform4f(glGetUniformLocation(GL_ShaderProgram, uniformName.c_str()), value.x, value.y, value.z, value.w));
#endif
}


void Shader::SetValueVec1I(std::string uniformName, int value)
{
#ifdef USE_CACHE
	GL_Call(glUniform1i(GL_GetUniformLocation(uniformName), value));
#else
	GL_Call(glUniform1i(glGetUniformLocation(GL_ShaderProgram, uniformName.c_str()), value));
#endif
}


void Shader::SetValueVec2I(std::string uniformName, Vector2I value)
{
#ifdef USE_CACHE
	GL_Call(glUniform2i(GL_GetUniformLocation(uniformName), value.x, value.y));
#else
	GL_Call(glUniform2i(glGetUniformLocation(GL_ShaderProgram, uniformName.c_str()), value.x, value.y));
#endif
}


void Shader::SetValueVec3I(std::string uniformName, Vector3I value)
{
#ifdef USE_CACHE
	GL_Call(glUniform3i(GL_GetUniformLocation(uniformName), value.x, value.y, value.z));
#else
	GL_Call(glUniform3i(glGetUniformLocation(GL_ShaderProgram, uniformName.c_str()), value.x, value.y, value.z));
#endif
}


void Shader::SetValueVec4I(std::string uniformName, Vector4I value)
{
#ifdef USE_CACHE
	GL_Call(glUniform4i(GL_GetUniformLocation(uniformName), value.x, value.y, value.z, value.w));
#else
	GL_Call(glUniform4i(glGetUniformLocation(GL_ShaderProgram, uniformName.c_str()), value.x, value.y, value.z, value.w));
#endif
}


void Shader::SetValueColor(std::string uniformName, Color value)
{
#ifdef USE_CACHE
	GL_Call(glUniform4f(GL_GetUniformLocation(uniformName), value.r, value.g, value.b, value.a));
#else
	GL_Call(glUniform4f(glGetUniformLocation(GL_ShaderProgram, uniformName.c_str()), value.r, value.g, value.b, value.a));
#endif
}


void Shader::SetValueMat3(std::string uniformName, glm::mat3 value)
{
#ifdef USE_CACHE
	GL_Call(glUniformMatrix3fv(GL_GetUniformLocation(uniformName), 1, GL_FALSE, &value[0][0]));
#else
	GL_Call(glUniformMatrix3fv(glGetUniformLocation(GL_ShaderProgram, uniformName.c_str()), 1, GL_FALSE, &value[0][0]));
#endif
}


void Shader::SetValueMat4(std::string uniformName, glm::mat4 value)
{
#ifdef USE_CACHE
	GL_Call(glUniformMatrix4fv(GL_GetUniformLocation(uniformName), 1, GL_FALSE, &value[0][0]));
#else
	GL_Call(glUniformMatrix4fv(glGetUniformLocation(GL_ShaderProgram, uniformName.c_str()), 1, GL_FALSE, &value[0][0]));
#endif
}


std::shared_ptr<Shader> Shader::CreateEmptyShader(std::string _name)
{
	ShaderLibrary::RegisterShader(new Shader(_name));
	auto shader = ShaderLibrary::GetShader(_name);
	shader->GL_IsLitShader = false;
	return std::shared_ptr<Shader>(shader);
}


std::shared_ptr<Shader> Shader::CreateBasicLitShader(std::string _name)
{
	ShaderLibrary::RegisterShader(new Shader(_name));
	auto shader = ShaderLibrary::GetShader(_name);
	shader->LoadShaderResource("res\\VertexShader\\BasicVert.vert", ShaderType::VertexShader);
	shader->LoadShaderResource("res\\FragmentShader\\BasicFrag.frag", ShaderType::FragmentShader);
	shader->GL_IsLitShader = true;
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


std::shared_ptr<Shader> Shader::CreateSkyboxShader(std::string _name)
{
	ShaderLibrary::RegisterShader(new Shader(_name));
	auto shader = ShaderLibrary::GetShader(_name);
	shader->LoadShaderResource("res\\VertexShader\\SkyBoxVert.vert", ShaderType::VertexShader);
	shader->LoadShaderResource("res\\FragmentShader\\SkyBoxFrag.frag", ShaderType::FragmentShader);
	shader->GL_IsLitShader = false;
	return std::shared_ptr<Shader>(shader);
}


void Shader::GL_UnbindPrograms()
{
	GL_Call(glUseProgram(0));
}
