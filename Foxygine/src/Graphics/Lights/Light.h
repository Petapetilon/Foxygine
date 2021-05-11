#pragma once
#include "../../Math/Color.h"


class Light {
protected:
	Color color;

public:
	enum class LightType {
		None,
		Directional,
		Point
	};

	virtual LightType GL_SetLightingPasses(int index) { return LightType::None; }
};