#pragma once
#include <string>
#include <memory>
#include <iostream>
#include "../../Math/Vector2.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector4.h"
#include "../../Math/Vector2I.h"
#include "../../Math/Vector3I.h"
#include "../../Math/Vector4I.h"
#include "../../Math/Color.h"
#include "glm.hpp"


class Shader;

class ShaderPass {
protected:
	enum class ShaderPassType {
		None,
		Vec1,
		Vec2,
		Vec3,
		Vec4,
		Vec1I,
		Vec2I,
		Vec3I,
		Vec4I,
		Mat4,
		Color,
		Bool
	};

	std::string GL_UniformName;
	int GL_UniformLocation;

public:
	std::string GetUniformName();

	virtual void SetShaderPass() {}
	virtual void CreateShaderPass(Shader* shader) {}
	virtual void CopyShaderPassValue(ShaderPass* source) {}
	virtual ShaderPassType GetShaderPassType() { return ShaderPassType::None; }
	virtual ~ShaderPass() {}

	bool operator == (const ShaderPass right) const { return GL_UniformName == right.GL_UniformName; }
	bool operator != (const ShaderPass right) const { return GL_UniformName != right.GL_UniformName; }
};



class ShaderPassVec1 : public ShaderPass {
private:
	std::shared_ptr<float> val;

public:
	ShaderPassVec1(float* _val, std::string _uniformName);
	~ShaderPassVec1() { val.reset(); }

	void SetShaderPass() override;
	void CreateShaderPass(Shader* shader) override;
	void CopyShaderPassValue(ShaderPass* source) override;
	ShaderPassType GetShaderPassType() override { return ShaderPassType::Vec1; }
	std::shared_ptr<float> GetValueRef() { return val; }
};



class ShaderPassVec2 : ShaderPass {
private:
	std::shared_ptr <Vector2> val;

public:
	ShaderPassVec2(Vector2* _val, std::string _uniformName);	
	~ShaderPassVec2() { val.reset(); }

	void SetShaderPass() override;
	void CreateShaderPass(Shader* shader) override;
	void CopyShaderPassValue(ShaderPass* source) override;
	ShaderPassType GetShaderPassType() override { return ShaderPassType::Vec2; }
	std::shared_ptr<Vector2> GetValueRef() { return val; }
};



class ShaderPassVec3 : public ShaderPass {
private:
	std::shared_ptr <Vector3> val;

public:
	ShaderPassVec3(Vector3* _val, std::string _uniformName);
	~ShaderPassVec3() { val.reset(); }

	void SetShaderPass() override;
	void CreateShaderPass(Shader* shader) override;	
	void CopyShaderPassValue(ShaderPass* source) override;
	ShaderPassType GetShaderPassType() override { return ShaderPassType::Vec3; }
	std::shared_ptr<Vector3> GetValueRef() { return val; }
};



class ShaderPassVec4 : public ShaderPass {
private:
	std::shared_ptr <Vector4> val;

public:
	ShaderPassVec4(Vector4* _val, std::string _uniformName);
	~ShaderPassVec4() { val.reset(); }

	void SetShaderPass() override;
	void CreateShaderPass(Shader* shader) override;
	void CopyShaderPassValue(ShaderPass* source) override;
	ShaderPassType GetShaderPassType() override { return ShaderPassType::Vec4; }
	std::shared_ptr<Vector4> GetValueRef() { return val; }
};



class ShaderPassVec1I : public ShaderPass {
private:
	std::shared_ptr<int> val;

public:
	ShaderPassVec1I(int* _val, std::string _uniformName);
	~ShaderPassVec1I() { val.reset(); }

	void SetShaderPass() override;
	void CreateShaderPass(Shader* shader) override;
	void CopyShaderPassValue(ShaderPass* source) override;
	ShaderPassType GetShaderPassType() override { return ShaderPassType::Vec1I; }
	std::shared_ptr<int> GetValueRef() { return val; }
};



class ShaderPassVec2I : public ShaderPass {
private:
	std::shared_ptr <Vector2I> val;

public:
	ShaderPassVec2I(Vector2I* _val, std::string _uniformName);
	~ShaderPassVec2I() { val.reset(); }

	void SetShaderPass() override;
	void CreateShaderPass(Shader* shader) override;
	void CopyShaderPassValue(ShaderPass* source) override;
	ShaderPassType GetShaderPassType() override { return ShaderPassType::Vec2I; }
	std::shared_ptr<Vector2I> GetValueRef() { return val; }
};



class ShaderPassVec3I : public ShaderPass {
private:
	std::shared_ptr <Vector3I> val;

public:
	ShaderPassVec3I(Vector3I* _val, std::string _uniformName);
	~ShaderPassVec3I() { val.reset(); }

	void SetShaderPass() override;
	void CreateShaderPass(Shader* shader) override;
	void CopyShaderPassValue(ShaderPass* source) override;
	ShaderPassType GetShaderPassType() override { return ShaderPassType::Vec3I; }
	std::shared_ptr<Vector3I> GetValueRef() { return val; }
};



class ShaderPassVec4I : public ShaderPass {
private:
	std::shared_ptr <Vector4I> val;

public:
	ShaderPassVec4I(Vector4I* _val, std::string _uniformName);
	~ShaderPassVec4I() { val.reset(); }

	void SetShaderPass() override;
	void CreateShaderPass(Shader* shader) override;
	void CopyShaderPassValue(ShaderPass* source) override;
	ShaderPassType GetShaderPassType() override { return ShaderPassType::Vec4I; }
	std::shared_ptr<Vector4I> GetValueRef() { return val; }
};



class ShaderPassMat4 : public ShaderPass{
private:
	std::shared_ptr < glm::mat4> val;

public:
	ShaderPassMat4(glm::mat4* _val, std::string _uniformName);
	~ShaderPassMat4() { val.reset(); }

	void SetShaderPass() override;
	void CreateShaderPass(Shader* shader) override;
	void CopyShaderPassValue(ShaderPass* source) override;
	ShaderPassType GetShaderPassType() override { return ShaderPassType::Mat4; }
	std::shared_ptr<glm::mat4> GetValueRef() { return val; }
};



class ShaderPassColor : public ShaderPass{
private:
	std::shared_ptr<Color> val;

public:
	ShaderPassColor(Color* _val, std::string _uniformName);
	~ShaderPassColor() { val.reset(); }

	void SetShaderPass() override;
	void CreateShaderPass(Shader* shader) override;
	void CopyShaderPassValue(ShaderPass* source) override;
	ShaderPassType GetShaderPassType() override { return ShaderPassType::Color; }
	std::shared_ptr<Color> GetValueRef() { return val; }
};



class ShaderPassBool : public ShaderPass{
private:
	std::shared_ptr<bool> val;

public:
	ShaderPassBool(bool* _val, std::string _uniformName);
	~ShaderPassBool() { val.reset(); }

	void SetShaderPass() override;
	void CreateShaderPass(Shader* shader) override;
	void CopyShaderPassValue(ShaderPass* source) override;
	ShaderPassType GetShaderPassType() override { return ShaderPassType::Bool; }
	std::shared_ptr<bool> GetValueRef() { return val; }
};