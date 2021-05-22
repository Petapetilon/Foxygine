#include "Canvas.h"
#include "UIElement.h"
#include <algorithm>


void Canvas::AddElement(UIElement* element)
{
	uiElements.push_back(element);
}


void Canvas::RemoveElement(UIElement* element)
{
	uiElements.erase(std::remove(uiElements.begin(), uiElements.end(), element), uiElements.end());
}


UIElement* Canvas::GetElement(std::string name)
{
	for (auto element : uiElements)
		if (element->name == name)
			return element;
}


void Canvas::SetElementIndex(UIElement* element, int newIndex)
{
}


void Canvas::SetElementIndex(std::string, int newIndex)
{
}


void Canvas::SetElementIndex(int oldIndex, int newIndex)
{
}
