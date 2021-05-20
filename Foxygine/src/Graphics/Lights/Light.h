#pragma once
#include "../../Math/Color.h"


class Light {
protected:
	Color color;
	float intensity;

public:
	enum class LightType {
		Directional = 0,
		Point = 1,
		Ambient_Local = 2,
		Ambient_Global = 3,
		Ambient_Directional = 4
	};

	virtual void GL_SetLightingPasses(int index) {}
	virtual void GL_RenderShadowMap() {}
	virtual void SetShadowMapResolution(unsigned int resolution) {}
	virtual bool GetShadowCapabilities() { return false; }
};