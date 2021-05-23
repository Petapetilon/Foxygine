#pragma once
#include <vector>
#include "../../Math/Vector2.h"
#include "../../Math/Vector2I.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector3I.h"


class BoundingRect
{
protected:
	Vector2 position;
	Vector2 dimension;
	Vector2 extents;
	float rotation;

public:
	void Translate(Vector2 translation);
	void Translate(float x, float y);
	void Rotate(float angle);
	void Scale(Vector2 scaleMod);
	void Scale(float x, float y);

	void SetPosition(Vector2 _position);
	void SetPosition(float x, float y);
	void SetRotation(float _rotationDegrees);
	void SetDimension(Vector2 _dimension);
	void SetDimension(float x, float y);
	void SetWidth(float width);
	void SetHeight(float height);

	float GetLeftBound();
	float GetRightBound();
	float GetUpperBound();
	float GetLowerBound();

	Vector2 GetPosition();
	Vector2 GetDimension();
	Vector2 GetExtents();
	float GetRotation();

	bool CheckOverlap(BoundingRect other);
	bool CheckPointInisde(Vector2 point);
	std::vector<Vector2> GetBoundingPoints();

	void AdjustBoundToFit(BoundingRect other);
	BoundingRect GetNonRotatedBound();
};

