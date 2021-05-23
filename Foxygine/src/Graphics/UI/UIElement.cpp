#include "UIElement.h"
#include "Canvas.h"
#include "BoundingRect.h"
#include <iostream>


glm::mat4 UIElement::GetCombinedTransform()
{
	if (parent != nullptr) {
		return parent->GetCombinedTransform() * myTransform;
	}

	std::cout << "UIElements need a Parent to function!" << std::endl;
	return glm::mat4(1);
}


void UIElement::Draw()
{
	for (auto it = children.begin(); it != children.end(); it++)
		(*it)->Draw();
}


void UIElement::SetAnkor(AnkorAlignment _alignment)
{
	alignment = _alignment;
}


void UIElement::SetSizePixelAbsolute(Vector2I dimensions)
{
	boundingRect->SetDimension(dimensions);
}


void UIElement::SetSizeParentRelative(Vector2 dimensionInPercentOfParent)
{
	auto pDim = parent->GetBounds()->GetDimension();
	boundingRect->SetDimension(Vector2(pDim.x * dimensionInPercentOfParent.x, pDim.y * dimensionInPercentOfParent.y) * .01f);
}


std::shared_ptr<BoundingRect> UIElement::GetBounds()
{
	return std::shared_ptr<BoundingRect>(boundingRect);
}


std::shared_ptr<BoundingRect> UIElement::GetTransform()
{
	return std::shared_ptr<BoundingRect>(transformRect);
}


void UIElement::AddElement(UIElement* element)
{
	children.push_back(element);
}


void UIElement::RemoveElement(UIElement* element)
{
	children.erase(std::remove(children.begin(), children.end(), element), children.end());
}


UIElement* UIElement::GetElement(std::string name)
{
	for (auto element : children)
		if (element->name == name)
			return element;
}


void UIElement::SetElementIndex(UIElement* element, int newIndex)
{
	children.erase(std::remove(children.begin(), children.end(), element), children.end());
	children.insert(children.begin() + newIndex, element);
}


void UIElement::SetElementIndex(std::string _name, int newIndex)
{
	auto it = children.begin();
	for (; it != children.end(); it++) {
		if ((*it)->name == _name) {
			children.erase(it);
		}
	}

	children.insert(children.begin() + newIndex, *it);
}


void UIElement::SetElementIndex(int oldIndex, int newIndex)
{
	auto it = children.erase(children.begin() + oldIndex);
	children.insert(children.begin() + newIndex, *it);
}
