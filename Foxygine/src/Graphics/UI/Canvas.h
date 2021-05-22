#pragma once
#include <vector>
#include "../../GameObject/Component.h"
#include "../Rendering/Renderer.h"


class UIElement;

class Canvas : public Component, public Renderer
{
protected:
	std::vector<UIElement*> uiElements;

public:

	void AddElement(UIElement* element);
	void RemoveElement(UIElement* element);
	UIElement* GetElement(std::string name);
	void SetElementIndex(UIElement* element, int newIndex);
	void SetElementIndex(std::string, int newIndex);
	void SetElementIndex(int oldIndex, int newIndex);
};

