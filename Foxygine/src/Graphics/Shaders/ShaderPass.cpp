#include "ShaderPass.h"
#include "Shader.h"
#include "../GL.h"


std::string ShaderPass::GetUniformName()
{
	return GL_UniformName;
}



ShaderPassVec1::ShaderPassVec1(float* _val, std::string _GL_UniformName)
{
	val = std::make_shared<float>(*_val);
	GL_UniformName = _GL_UniformName;
}

void ShaderPassVec1::SetShaderPass()
{
	GL_Call(glUniform1f(GL_UniformLocation, *val));
}

void ShaderPassVec1::CreateShaderPass(Shader* shader) 
{
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
}

void ShaderPassVec1::CopyShaderPassValue(ShaderPass* source)
{
	if (source->GetShaderPassType() == this->GetShaderPassType()) {
		val = ((ShaderPassVec1*)source)->val;
	}
}





ShaderPassMat4::ShaderPassMat4(glm::mat4* _val, std::string _GL_UniformName)
{
	val = std::make_shared<glm::mat4>(*_val);
	GL_UniformName = _GL_UniformName;
}

void ShaderPassMat4::SetShaderPass()
{
	GL_Call(glUniformMatrix4fv(GL_UniformLocation, 1, GL_FALSE, &((*val)[0][0])));
	//std::cout << "Setting Matrix: \n" << (*val)[0][0] << "	" << (*val)[1][0] << "	" << (*val)[2][0] << "	" << (*val)[3][0] << std::endl
	//	<< (*val)[0][1] << "	" << (*val)[1][1] << "	" << (*val)[2][1] << "	" <<(*val)[3][1] << std::endl
	//	<< (*val)[0][2] << "	" << (*val)[1][2] << "	" << (*val)[2][2] << "	" <<(*val)[3][2] << std::endl
	//	<< (*val)[0][3] << "	" << (*val)[1][3] << "	" << (*val)[2][3] << "	" <<(*val)[3][3] << std::endl;
}


void ShaderPassMat4::CreateShaderPass(Shader* shader)
{
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
}

void ShaderPassMat4::CopyShaderPassValue(ShaderPass* source)
{
	if (source->GetShaderPassType() == this->GetShaderPassType()) {
		val = ((ShaderPassMat4*)source)->val;
	}
}





ShaderPassMat3::ShaderPassMat3(glm::mat3* _val, std::string _GL_UniformName)
{
	val = std::make_shared<glm::mat3>(*_val);
	GL_UniformName = _GL_UniformName;
}

void ShaderPassMat3::SetShaderPass()
{
	GL_Call(glUniformMatrix3fv(GL_UniformLocation, 1, GL_FALSE, &((*val)[0][0])));
	//std::cout << "Setting Matrix: \n" << (*val)[0][0] << "	" << (*val)[1][0] << "	" << (*val)[2][0] << "	" << (*val)[3][0] << std::endl
	//	<< (*val)[0][1] << "	" << (*val)[1][1] << "	" << (*val)[2][1] << "	" <<(*val)[3][1] << std::endl
	//	<< (*val)[0][2] << "	" << (*val)[1][2] << "	" << (*val)[2][2] << "	" <<(*val)[3][2] << std::endl
	//	<< (*val)[0][3] << "	" << (*val)[1][3] << "	" << (*val)[2][3] << "	" <<(*val)[3][3] << std::endl;
}


void ShaderPassMat3::CreateShaderPass(Shader* shader)
{
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
}

void ShaderPassMat3::CopyShaderPassValue(ShaderPass* source)
{
	if (source->GetShaderPassType() == this->GetShaderPassType()) {
		val = ((ShaderPassMat3*)source)->val;
	}
}





ShaderPassVec4I::ShaderPassVec4I(Vector4I* _val, std::string _GL_UniformName)
{
	val = std::make_shared<Vector4I>(*_val);
	GL_UniformName = _GL_UniformName;
}

void ShaderPassVec4I::SetShaderPass()
{
	GL_Call(glUniform4i(GL_UniformLocation, val->x, val->y, val->z, val->w));
}

void ShaderPassVec4I::CreateShaderPass(Shader* shader)
{
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
}

void ShaderPassVec4I::CopyShaderPassValue(ShaderPass* source)
{
	if (source->GetShaderPassType() == this->GetShaderPassType()) {
		val = ((ShaderPassVec4I*)source)->val;
	}
}




ShaderPassVec3I::ShaderPassVec3I(Vector3I* _val, std::string _GL_UniformName)
{
	val = std::make_shared<Vector3I>(*_val);
	GL_UniformName = _GL_UniformName;
}

void ShaderPassVec3I::SetShaderPass()
{
	GL_Call(glUniform3i(GL_UniformLocation, val->x, val->y, val->z));
}

void ShaderPassVec3I::CreateShaderPass(Shader* shader)
{
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
}

void ShaderPassVec3I::CopyShaderPassValue(ShaderPass* source)
{
	if (source->GetShaderPassType() == this->GetShaderPassType()) {
		val = ((ShaderPassVec3I*)source)->val;
	}
}




ShaderPassVec2I::ShaderPassVec2I(Vector2I* _val, std::string _GL_UniformName)
{
	val = std::make_shared<Vector2I>(*_val);
	GL_UniformName = _GL_UniformName;
}

void ShaderPassVec2I::SetShaderPass()
{
	GL_Call(glUniform2i(GL_UniformLocation, val->x, val->y));

}

void ShaderPassVec2I::CreateShaderPass(Shader* shader)
{
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
}

void ShaderPassVec2I::CopyShaderPassValue(ShaderPass* source)
{
	if (source->GetShaderPassType() == this->GetShaderPassType()) {
		val = ((ShaderPassVec2I*)source)->val;
	}
}




ShaderPassVec1I::ShaderPassVec1I(int* _val, std::string _GL_UniformName)
{
	val = std::make_shared<int>(*_val);
	GL_UniformName = _GL_UniformName;
}

void ShaderPassVec1I::SetShaderPass()
{
	GL_Call(glUniform1i(GL_UniformLocation, *val));
}

void ShaderPassVec1I::CreateShaderPass(Shader* shader)
{
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
}

void ShaderPassVec1I::CopyShaderPassValue(ShaderPass* source)
{
	if (source->GetShaderPassType() == this->GetShaderPassType()) {
		val = ((ShaderPassVec1I*)source)->val;
	}
}




ShaderPassVec4::ShaderPassVec4(Vector4* _val, std::string _GL_UniformName)
{
	val = std::make_shared<Vector4>(*_val);
	GL_UniformName = _GL_UniformName;
}

void ShaderPassVec4::SetShaderPass()
{
	GL_Call(glUniform4f(GL_UniformLocation, val->x, val->y, val->z, val->w));
}

void ShaderPassVec4::CreateShaderPass(Shader* shader)
{
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
}

void ShaderPassVec4::CopyShaderPassValue(ShaderPass* source)
{
	if (source->GetShaderPassType() == this->GetShaderPassType()) {
		val = ((ShaderPassVec4*)source)->val;
	}
}




ShaderPassVec3::ShaderPassVec3(Vector3* _val, std::string _GL_UniformName)
{
	val = std::make_shared<Vector3>(*_val);
	GL_UniformName = _GL_UniformName;
}

void ShaderPassVec3::SetShaderPass()
{
	GL_Call(glUniform3f(GL_UniformLocation, val->x, val->y, val->z));
}

void ShaderPassVec3::CreateShaderPass(Shader* shader)
{
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
}

void ShaderPassVec3::CopyShaderPassValue(ShaderPass* source)
{
	if (source->GetShaderPassType() == this->GetShaderPassType()) {
		val = ((ShaderPassVec3*)source)->val;
	}
}




ShaderPassVec2::ShaderPassVec2(Vector2* _val, std::string _GL_UniformName)
{
	val = std::make_shared<Vector2>(*_val);
	GL_UniformName = _GL_UniformName;
}

void ShaderPassVec2::SetShaderPass()
{
	GL_Call(glUniform2f(GL_UniformLocation, val->x, val->y));
}

void ShaderPassVec2::CreateShaderPass(Shader* shader)
{
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
}

void ShaderPassVec2::CopyShaderPassValue(ShaderPass* source)
{
	if (source->GetShaderPassType() == this->GetShaderPassType()) {
		val = ((ShaderPassVec2*)source)->val;
	}
}




ShaderPassColor::ShaderPassColor(Color* _val, std::string _GL_UniformName)
{
	val = std::make_shared<Color>(*_val);
	GL_UniformName = _GL_UniformName;
}

void ShaderPassColor::SetShaderPass()
{
	GL_Call(glUniform4f(GL_UniformLocation, val->r, val->g, val->b, val->a));
}

void ShaderPassColor::CreateShaderPass(Shader* shader)
{
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
}

void ShaderPassColor::CopyShaderPassValue(ShaderPass* source)
{
	if (source->GetShaderPassType() == this->GetShaderPassType()) {
		val = ((ShaderPassColor*)source)->val;
	}
}




ShaderPassBool::ShaderPassBool(bool* _val, std::string _GL_UniformName)
{
	val = std::make_shared<bool>(*_val);
	GL_UniformName = _GL_UniformName;
}

void ShaderPassBool::SetShaderPass()
{
	GL_Call(glUniform1i(GL_UniformLocation, *val));
}

void ShaderPassBool::CreateShaderPass(Shader* shader)
{
	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
}

void ShaderPassBool::CopyShaderPassValue(ShaderPass* source)
{
	if (source->GetShaderPassType() == this->GetShaderPassType()) {
		val = ((ShaderPassBool*)source)->val;
	}
}




//ShaderPassVec1Array::ShaderPassVec1Array(float* _val, std::string _GL_UniformName, int index)
//{
//	val = std::make_shared<float>(*_val);
//	GL_UniformName = _GL_UniformName;
//}
//
//void ShaderPassVec1Array::SetShaderPass()
//{
//	GL_Call(glUniform1f(GL_UniformLocation, *val);
//}
//
//void ShaderPassVec1Array::CreateShaderPass(Shader* shader)
//{
//	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
//}
//
//void ShaderPassVec1Array::CopyShaderPassValue(ShaderPass* source)
//{
//	if (source->GetShaderPassType() == this->GetShaderPassType()) {
//		val = ((ShaderPassVec1Array*)source)->val;
//	}
//}
//
//
//
//
//ShaderPassColor::ShaderPassColor(Color* _val, std::string _GL_UniformName)
//{
//	val = std::make_shared<Color>(*_val);
//	GL_UniformName = _GL_UniformName;
//}
//
//void ShaderPassColor::SetShaderPass()
//{
//	GL_Call(glUniform4f(GL_UniformLocation, val->r, val->g, val->b, val->a);
//}
//
//void ShaderPassColor::CreateShaderPass(Shader* shader)
//{
//	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
//}
//
//void ShaderPassColor::CopyShaderPassValue(ShaderPass* source)
//{
//	if (source->GetShaderPassType() == this->GetShaderPassType()) {
//		val = ((ShaderPassColor*)source)->val;
//	}
//}
//
//
//
//
//ShaderPassBool::ShaderPassBool(bool* _val, std::string _GL_UniformName)
//{
//	val = std::make_shared<bool>(*_val);
//	GL_UniformName = _GL_UniformName;
//}
//
//void ShaderPassBool::SetShaderPass()
//{
//	GL_Call(glUniform1i(GL_UniformLocation, *val);
//}
//
//void ShaderPassBool::CreateShaderPass(Shader* shader)
//{
//	GL_UniformLocation = glGetUniformLocation(shader->GL_GetShaderProgram(), GL_UniformName.c_str());
//}
//
//void ShaderPassBool::CopyShaderPassValue(ShaderPass* source)
//{
//	if (source->GetShaderPassType() == this->GetShaderPassType()) {
//		val = ((ShaderPassBool*)source)->val;
//	}
//}
