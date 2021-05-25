#include "ScreenSpaceCanvas.h"
#include "../../Peripherals/Window.h"
#include "gtx/matrix_decompose.hpp"
#include "BoundingRect.h"



ScreenSpaceCanvas::ScreenSpaceCanvas()
{
	canvas = this;
	parent = nullptr;

	transformRect = std::shared_ptr<BoundingRect>(new BoundingRect());
	boundingRect = std::shared_ptr<BoundingRect>(new BoundingRect());
}


std::shared_ptr<UIElement> ScreenSpaceCanvas::RayCastFromScreen(Vector2I screenPixel)
{
	return std::shared_ptr<UIElement>();
}


std::shared_ptr<UIElement> ScreenSpaceCanvas::RayCastFromScreen(Vector2 screenCoordinate)
{
	return std::shared_ptr<UIElement>();
}


std::vector<std::shared_ptr<UIElement>> ScreenSpaceCanvas::RayCastFromScreenAll(Vector2I screenPixel)
{
	return std::vector<std::shared_ptr<UIElement>>();
}


std::vector<std::shared_ptr<UIElement>> ScreenSpaceCanvas::RayCastFromScreenAll(Vector2 screenCoordinate)
{
	return std::vector<std::shared_ptr<UIElement>>();
}


std::shared_ptr<UIElement> ScreenSpaceCanvas::RayCastFromMouse()
{
	return std::shared_ptr<UIElement>();
}


std::vector<std::shared_ptr<UIElement>> ScreenSpaceCanvas::RayCastFromMouseAll()
{
	return std::vector<std::shared_ptr<UIElement>>();
}


void ScreenSpaceCanvas::OnAttach()
{
	__super::OnAttach();

	auto res = Window::GetInstance()->GetWindowResolution();
	projectionMatrix = glm::ortho(0, res.x, res.y, 0);
}


void ScreenSpaceCanvas::Draw()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	__super::Draw();

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}


void ScreenSpaceCanvas::OnWindowResize()
{
	UIElement::OnTransformChanged();
}
