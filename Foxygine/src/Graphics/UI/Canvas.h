#pragma once
#include <vector>
#include <map>
#include "../../GameObject/Component.h"
#include "UIElement.h"


class Camera;


class Canvas : public UIElement, public Component
{
protected:
	glm::mat4 projectionMatrix;

	long latestElementID;
	std::map<std::string, long> elementNameLookup;
	std::map<long, UIElement*> elementMap;


public:
	UIElement* FindElement(std::string _name);
	UIElement* FindElement(long id);
	void RegisterElement(UIElement* element);
	void UnregisterElement(UIElement* element);


	virtual void OnWindowResize() = 0;

	virtual std::shared_ptr<UIElement> RayCastFromScreen(Vector2I screenPixel) = 0;//{ return nullptr; }
	virtual std::shared_ptr<UIElement> RayCastFromScreen(Vector2 screenCoordinate) = 0;//{ return nullptr; }
	virtual std::vector<std::shared_ptr<UIElement>> RayCastFromScreenAll(Vector2I screenPixel) = 0;//{ return std::vector<std::shared_ptr<UIElement>>(); }
	virtual std::vector<std::shared_ptr<UIElement>> RayCastFromScreenAll(Vector2 screenCoordinate) = 0;//{ return std::vector<std::shared_ptr<UIElement>>(); }

	virtual std::shared_ptr<UIElement> RayCastFromMouse() = 0;// { return nullptr; }
	virtual std::vector<std::shared_ptr<UIElement>> RayCastFromMouseAll() = 0;//{ return std::vector<std::shared_ptr<UIElement>>(); }

	virtual void OnAttach() override;
	virtual void OnDetach() override;
};

