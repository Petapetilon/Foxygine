#include "UIElement.h"
#include "Canvas.h"
#include "BoundingRect.h"
#include <iostream>



void UIElement::OnTransformChanged()
{
	boundingRect->Init();
	boundingRect->AdjustBoundToFit(transformRect);

	for (auto it = children.begin(); it != children.end(); it++) 
		(*it)->OnTransformChanged();
}


glm::mat4 UIElement::GetCombinedTransform()
{
	if (parent != nullptr) {
		return parent->GetCombinedTransform() * myTransform;
	}

	std::cout << "UIElements need a Parent to function!" << std::endl;
	return glm::mat4(1);
}


Vector2 UIElement::GetCombinedOffset()
{
	return Vector2();
}


void UIElement::Draw()
{
	if (!isActive) return;

	for (auto it = children.begin(); it != children.end(); it++)
		(*it)->Draw();
}


void UIElement::Update(float deltaTime)
{
	if (!isActive) return;

	for (auto it = children.begin(); it != children.end(); it++)
		(*it)->Update(deltaTime);
}


void UIElement::FixedUpdate(float deltaTime)
{
	if (!isActive) return;

	for (auto it = children.begin(); it != children.end(); it++)
		(*it)->FixedUpdate(deltaTime);
}


void UIElement::SetAnkor(AnkorAlignment _alignment)
{
	alignment = _alignment;
}


void UIElement::SetSizePixelAbsolute(Vector2I dimensions)
{
	transformRect->SetDimension(Vector2(dimensions.x, dimensions.y));
	OnTransformChanged();
}


void UIElement::SetSizeParentRelative(Vector2 dimensionInPercentOfParent)
{
	auto pDim = parent->GetBounds()->GetDimension();
	transformRect->SetDimension(Vector2(pDim.x * dimensionInPercentOfParent.x, pDim.y * dimensionInPercentOfParent.y) * .01f);
	OnTransformChanged();
}


void UIElement::SetPosition(Vector2I pixelPosition)
{
	transformRect->SetPosition(pixelPosition);
	OnTransformChanged();
}


void UIElement::SetPositionLocal(Vector2I pixelOffset)
{
	if (parent == nullptr) {
		transformRect->SetPosition(pixelOffset);
		OnTransformChanged();
		return;
	}

	Vector2 position = parent->GetTransform()->GetRight() * (float)pixelOffset.x;
	position = position + parent->GetTransform()->GetUp() * (float)pixelOffset.y;
	transformRect->SetPosition(position);
	OnTransformChanged();
}


void UIElement::SetRotation(float rotation)
{
	transformRect->SetRotation(rotation);
	OnTransformChanged();
}


void UIElement::SetRotationLocal(float rotation)
{
	if (parent == nullptr) {
		transformRect->SetRotation(rotation);
		OnTransformChanged();
		return;
	}

	transformRect->SetRotation(parent->GetTransform()->GetRotation() + rotation);
	OnTransformChanged();
}


std::shared_ptr<BoundingRect> UIElement::GetBounds()
{
	return std::shared_ptr<BoundingRect>(boundingRect);
}


std::shared_ptr<BoundingRect> UIElement::GetTransform()
{
	return std::shared_ptr<BoundingRect>(transformRect);
}


void UIElement::BuildChildrenBounds()
{
	boundingRect->Init();
	boundingRect->AdjustBoundToFit(transformRect);

	for (auto it = children.begin(); it != children.end(); it++) {
		(*it)->BuildChildrenBounds();
		boundingRect->AdjustBoundToFit((*it)->GetBounds());
	}
}


void UIElement::AddElement(UIElement* element)
{
	if(element->parent == this)
		return;


	if (element->parent != nullptr) {
		element->parent->RemoveElement(element);
	}

	children.push_back(element);
	element->parent = this;

	canvas->RegisterElement(element);
}


void UIElement::RemoveElement(UIElement* element)
{
	children.erase(std::remove(children.begin(), children.end(), element), children.end());

	if(canvas == element->canvas)
		canvas->UnregisterElement(element);

	element->parent = nullptr;
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

void UIElement::SetCanvas(Canvas* _canvas, long id)
{
	if (canvas != nullptr)
		canvas->UnregisterElement(this);

	canvas = _canvas;
	uniqueCanvasID = id;
}

long UIElement::ClearCanvas(Canvas* _canvas)
{
	if (canvas == _canvas) {
		long temp = uniqueCanvasID;
		uniqueCanvasID = -1;
		canvas = nullptr;
		return temp;
	}
}
