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
	bool GL_IsLitShader;
	unsigned int GL_ShaderProgram;
	std::list<ShaderPass*> shaderPasses;

	void AddShaderPass(ShaderPass* newPass);
	ShaderPass* GetShaderPass(std::string uniformName);
	Shader(std::string);
	Shader();

public:
	enum class ShaderType {
		VertexShader,
		GeometryShader,
		FragmentShader
	};

	std::string name;

	unsigned int GL_GetShaderProgram();
	void GL_BindProgram();
	void SetShaderPass(ShaderPass* pass);
	void LoadShaderResource(std::string shaderSourceFilePath, ShaderType shaderType);
	bool GetShaderLitType();

	static std::shared_ptr<Shader> CreateBasicLitShader(std::string _name);
	static std::shared_ptr<Shader> CreateBasicUnlitShader(std::string _name);
	static std::shared_ptr<Shader> CreateBasicLitUIShader(std::string _name);
	static std::shared_ptr<Shader> CreateBasicUnlitUIShader(std::string _name);
	static std::shared_ptr<Shader> CreateSkyboxShader(std::string _name);
	static void GL_UnbindPrograms();


	virtual void GL_SetUniforms();
	virtual ~Shader() { glDeleteProgram(GL_ShaderProgram); }

	bool operator ==(const Shader& right) const { return GL_ShaderProgram == right.GL_ShaderProgram; }
	bool operator !=(const Shader& right) const { return GL_ShaderProgram != right.GL_ShaderProgram; }
};


