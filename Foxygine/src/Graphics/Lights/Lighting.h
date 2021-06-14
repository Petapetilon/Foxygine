#pragma once
#include <list>
#include <vector>
#include "../../Math/Color.h"
#include "../../Math/Vector3.h"


class Light;


class Lighting
{
private:
	static const int maxLights = 128;

public:
	enum class LightType {
		Directional = 0,
		Ambient = 1,
		Point = 2,
	};

	struct LightPass {
	public: 
		LightType type;
		Vector3 pos;
		Vector3 dir;
		Color color;
		float intensity;
		glm::mat4 LSM;
	};

private:
	static std::list<Light*> lights;
	static std::vector<float> positions;
	static std::vector<float> directions;
	static std::vector<float> colors;
	static std::vector<float> intensities;
	static std::vector<int> types;

public:
	static void RegisterLight(Light* l);
	static void UnregisterLight(Light* l);
	static void LightDataChanged(Light* l);

	static void GL_SetLightUniforms();
	static void GL_RenderShadows();
	static void GL_BakeEnvironmentLight();
	static void GL_BakeLightMaps();
};

