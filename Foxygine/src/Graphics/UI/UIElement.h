#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../../Math/Color.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector2I.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector3I.h"
#include "../../GameObject/Object.h"


class Canvas;
class BoundingRect;
enum class AnkorAlignment;


class UIElement : public Object
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
	AnkorAlignment alignment;
	UIElement* parent;
	Canvas* canvas;
	long uniqueCanvasID;
	std::vector<UIElement*> children;
	glm::mat4 myTransform;

	virtual void OnTransformChanged();

public:
	virtual glm::mat4 GetCombinedTransform();
	virtual Vector2 GetCombinedOffset();
	
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void FixedUpdate(float deltaTime);
	
	void SetAnkor(AnkorAlignment _alignment);
	void SetSizePixelAbsolute(Vector2I dimensions);
	void SetSizeParentRelative(Vector2 dimensionInPercentOfParent);
	void SetPosition(Vector2I pixelPosition);
	void SetPositionLocal(Vector2I pixelOffset);
	void SetRotation(float rotation);
	void SetRotationLocal(float rotation);
	std::shared_ptr<BoundingRect> GetBounds();
	std::shared_ptr<BoundingRect> GetTransform();

	void BuildChildrenBounds();

	virtual void AddElement(UIElement* element);
	void RemoveElement(UIElement* element);
	UIElement* GetElement(std::string name);
	void SetElementIndex(UIElement* element, int newIndex);
	void SetElementIndex(std::string, int newIndex);
	void SetElementIndex(int oldIndex, int newIndex);

	void SetCanvas(Canvas* canvas, long id);
	long ClearCanvas(Canvas* canvas);
};

