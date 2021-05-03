#pragma once
#include <string>
#include <list>
#include <memory>
#include "../GL.h"


class Camera;
class Transform;
class ShaderPass;


class Shader {
protected:
	unsigned int GL_ShaderProgram;
	std::list<ShaderPass*> shaderPasses;
	void AddShaderPass(ShaderPass* newPass);
	ShaderPass* GetShaderPass(std::string uniformName);

public:
	std::string name;
	Shader(std::string);
	Shader();

	unsigned int GL_GetShaderProgram();
	void GL_BindProgram();
	void SetShaderPass(ShaderPass* pass);



	virtual void GL_SetUniforms();
	virtual ~Shader() { glDeleteProgram(GL_ShaderProgram); }

	bool operator ==(const Shader& right) const { return GL_ShaderProgram == right.GL_ShaderProgram; }
	bool operator !=(const Shader& right) const { return GL_ShaderProgram != right.GL_ShaderProgram; }
};


