#pragma once
#include <vector>
#include "../../GameObject/Component.h"
#include "../Rendering/Renderer.h"
#include "UIElement.h"



class Canvas : public Component, public Renderer, public UIElement
{
public:
	virtual std::shared_ptr<UIElement> RayCastFromScreen(Vector2I screenPixel) {}
	virtual std::shared_ptr<UIElement> RayCastFromScreen(Vector2 screenCoordinate) {}
	virtual std::vector<std::shared_ptr<UIElement>> RayCastFromScreenAll(Vector2I screenPixel) {}
	virtual std::vector<std::shared_ptr<UIElement>> RayCastFromScreenAll(Vector2 screenCoordinate) {}
};

