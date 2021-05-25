#pragma once
#include <vector>
#include "../../GameObject/Component.h"
#include "UIElement.h"


class Camera;


class Canvas : public Component, public UIElement
{
protected:
	glm::mat4 projectionMatrix;


public:
	glm::mat4 GetCombinedTransform() override;
	virtual std::shared_ptr<UIElement> RayCastFromScreen(Vector2I screenPixel) { return nullptr; }
	virtual std::shared_ptr<UIElement> RayCastFromScreen(Vector2 screenCoordinate) { return nullptr; }
	virtual std::vector<std::shared_ptr<UIElement>> RayCastFromScreenAll(Vector2I screenPixel) { return std::vector<std::shared_ptr<UIElement>>(); }
	virtual std::vector<std::shared_ptr<UIElement>> RayCastFromScreenAll(Vector2 screenCoordinate) { return std::vector<std::shared_ptr<UIElement>>(); }

	virtual std::shared_ptr<UIElement> RayCastFromMouse() { return nullptr; }
	virtual std::vector<std::shared_ptr<UIElement>> RayCastFromMouseAll() { return std::vector<std::shared_ptr<UIElement>>(); }

	void Draw() override;
};

