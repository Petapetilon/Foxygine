#pragma once
#include <string>
#include <list>
#include <memory>
#include "../GL.h"
#include <map>
#include "../../Math/Vector2.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector4.h"
#include "../../Math/Vector2I.h"
#include "../../Math/Vector3I.h"
#include "../../Math/Vector4I.h"
#include "../../Math/Color.h"


class Camera;
class Transform;
class ShaderPass;


class Shader {
protected:
	bool GL_IsLitShader;
	unsigned int GL_ShaderProgram;
	std::list<ShaderPass*> shaderPasses;

	//Cache
	std::map<std::string, GLint> shaderUniformLocations;
	GLint GL_GetUniformLocation(const std::string& name);

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
	bool GL_BindProgram();
	void LoadShaderResource(std::string shaderSourceFilePath, ShaderType shaderType);
	bool GetShaderLitType();

	void SetValueVec1(std::string uniformName, float value);
	void SetValueVec2(std::string uniformName, Vector2 value);
	void SetValueVec3(std::string uniformName, Vector3 value);
	void SetValueVec4(std::string uniformName, Vector4 value);
	void SetValueVec1I(std::string uniformName, int value);
	void SetValueVec2I(std::string uniformName, Vector2I value);
	void SetValueVec3I(std::string uniformName, Vector3I value);
	void SetValueVec4I(std::string uniformName, Vector4I value);
	void SetValueColor(std::string uniformName, Color value);
	void SetValueMat3(std::string uniformName, glm::mat3 value);
	void SetValueMat4(std::string uniformName, glm::mat4 value);

	static std::shared_ptr<Shader> CreateEmptyShader(std::string _name);
	static std::shared_ptr<Shader> CreateBasicLitShader(std::string _name);
	static std::shared_ptr<Shader> CreateBasicUnlitShader(std::string _name);
	static std::shared_ptr<Shader> CreateBasicLitUIShader(std::string _name);
	static std::shared_ptr<Shader> CreateBasicUnlitUIShader(std::string _name);
	static std::shared_ptr<Shader> CreateSkyboxShader(std::string _name);
	static void GL_UnbindPrograms();


	virtual ~Shader() { glDeleteProgram(GL_ShaderProgram); }

	bool operator ==(const Shader& right) const { return GL_ShaderProgram == right.GL_ShaderProgram; }
	bool operator !=(const Shader& right) const { return GL_ShaderProgram != right.GL_ShaderProgram; }
};


