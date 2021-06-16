#include "Lighting.h"
#include "Light.h"
#include "../GL.h"
#include "../Graphics.h"
#include "../Shaders/Shader.h"


std::list<Light*> Lighting::lights;
std::vector<float> Lighting::positions;
std::vector<float> Lighting::directions;
std::vector<float> Lighting::colors;
std::vector<float> Lighting::intensities;
std::vector<int> Lighting::types;


void Lighting::RegisterLight(Light* l)
{
	lights.push_back(l);
}


void Lighting::UnregisterLight(Light* l)
{
	lights.remove(l);
}


void Lighting::GL_SetLightUniforms()
{
	positions.clear();
	directions.clear();
	colors.clear();
	intensities.clear();
	types.clear();

	auto pass = new LightPass();
	for (auto it = lights.begin(); it != lights.end(); ++it) {
	
		if ((*it)->GL_FillLightPass(pass) && types.size() < maxLights) {
			positions.push_back(pass->pos.x);
			positions.push_back(pass->pos.y);
			positions.push_back(pass->pos.z);

			directions.push_back(pass->dir.x);
			directions.push_back(pass->dir.y);
			directions.push_back(pass->dir.z);

			colors.push_back(pass->color.r);
			colors.push_back(pass->color.g);
			colors.push_back(pass->color.b);
			colors.push_back(pass->color.a);

			intensities.push_back(pass->intensity);

			types.push_back((int)pass->type);
		}
	}
	delete pass;


	auto shaderProg = Graphics::GL_GetCurrentlyBoundShader()->GL_GetShaderProgram();
	GL_Call(glUniform3fv(glGetUniformLocation(shaderProg, "u_LightPosition"), types.size(), positions.data()));
	GL_Call(glUniform3fv(glGetUniformLocation(shaderProg, "u_LightDirection"), types.size(), directions.data()));
	GL_Call(glUniform4fv(glGetUniformLocation(shaderProg, "u_LightColor"), types.size(), colors.data()));
	GL_Call(glUniform1fv(glGetUniformLocation(shaderProg, "u_LightIntensity"), types.size(), intensities.data()));
	GL_Call(glUniform1iv(glGetUniformLocation(shaderProg, "u_LightType"), types.size(), types.data()));
	GL_Call(glUniform1i(glGetUniformLocation(shaderProg, "u_NumberLights"), types.size()));
}


void Lighting::GL_RenderShadows()
{
}


void Lighting::GL_BakeEnvironmentLight()
{
}


void Lighting::GL_BakeLightMaps()
{
}
