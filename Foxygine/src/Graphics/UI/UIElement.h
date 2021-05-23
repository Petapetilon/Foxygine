#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../../Math/Color.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector2I.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector3I.h"


class Canvas;
class BoundingRect;
enum class AnkorAlignment;


class UIElement
{
public:
	enum class AnkorAlignment {
		Center,

		Top,
		Right,
		Bottom,
		Left,

		TopRight,
		BottomRight,
		BottomLeft,
		TopLeft
	};

protected:
	std::shared_ptr<BoundingRect> transformRect;
	std::shared_ptr<BoundingRect> boundingRect;
	std::shared_ptr<Canvas> canvas;
	AnkorAlignment alignment;
	Color color;
	UIElement* parent;
	std::vector<UIElement*> children;
	glm::mat4 myTransform;

	virtual glm::mat4 GetCombinedTransform();

public:
	std::string name;
	
	virtual void Draw();
	
	void SetAnkor(AnkorAlignment _alignment);
	void SetSizePixelAbsolute(Vector2I dimensions);
	void SetSizeParentRelative(Vector2 dimensionInPercentOfParent);
	std::shared_ptr<BoundingRect> GetBounds();
	std::shared_ptr<BoundingRect> GetTransform();

	void BuildChildrenBounds();

	void AddElement(UIElement* element);
	void RemoveElement(UIElement* element);
	UIElement* GetElement(std::string name);
	void SetElementIndex(UIElement* element, int newIndex);
	void SetElementIndex(std::string, int newIndex);
	void SetElementIndex(int oldIndex, int newIndex);
};

