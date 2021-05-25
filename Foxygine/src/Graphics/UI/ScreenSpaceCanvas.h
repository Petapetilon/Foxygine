#pragma once
#include "Canvas.h"



class ScreenSpaceCanvas : public Canvas
{
public:
	ScreenSpaceCanvas();

	std::shared_ptr<UIElement> RayCastFromScreen(Vector2I screenPixel) override;
	std::shared_ptr<UIElement> RayCastFromScreen(Vector2 screenCoordinate) override;
	std::vector<std::shared_ptr<UIElement>> RayCastFromScreenAll(Vector2I screenPixel) override;
	std::vector<std::shared_ptr<UIElement>> RayCastFromScreenAll(Vector2 screenCoordinate) override;

	std::shared_ptr<UIElement> RayCastFromMouse() override;

	void OnAttach() override;
	void Draw() override;

};

