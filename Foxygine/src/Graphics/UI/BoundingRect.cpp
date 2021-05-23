#include "BoundingRect.h"




void BoundingRect::Translate(Vector2 translation)
{
	position = position + translation;
}


void BoundingRect::Translate(float x, float y)
{
	position.x += x;
	position.y += y;
}


void BoundingRect::Rotate(float angle)
{
	rotation += angle;
	
	while (rotation < 0) {
		rotation += 360;
	}

	while (rotation > 360) {
		rotation -= 360;
	}
}


void BoundingRect::Scale(Vector2 scaleMod)
{
	dimension.x *= scaleMod.x;
	dimension.y *= scaleMod.y;
}


void BoundingRect::Scale(float x, float y)
{
	dimension.x *= x;
	dimension.y *= y;
}


void BoundingRect::SetPosition(Vector2 _position)
{
	position = _position;
}


void BoundingRect::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}


void BoundingRect::SetRotation(float _rotationDegrees)
{
	rotation = _rotationDegrees;
	
	while (rotation < 0) {
		rotation += 360;
	}

	while (rotation > 360) {
		rotation -= 360;
	}
}


void BoundingRect::SetDimension(Vector2 _dimension)
{
	dimension = _dimension;
	extents = dimension * .5;
}


void BoundingRect::SetDimension(float x, float y)
{
	dimension.x = x;
	dimension.y = y;
	extents.x = x * .5;
	extents.y = y * .5;
}


void BoundingRect::SetWidth(float width)
{
	dimension.x = width;
	extents.x = width * .5;
}


void BoundingRect::SetHeight(float height)
{
	dimension.y = height;
	extents.y = height * .5f;
}


float BoundingRect::GetLeftBound()
{
	if (rotation <= 90)
		return position.x - Vector2(extents.x, -extents.y).Rotate(rotation).x;
	else if (rotation <= 180)
		return position.x - extents.Rotate(rotation).x;
	else if (rotation <= 270)
		return position.x + Vector2(extents.x, -extents.y).Rotate(rotation).x;
	else
		return position.x + extents.Rotate(rotation).x;
}


float BoundingRect::GetRightBound()
{
	if (rotation <= 90)
		return position.x + Vector2(extents.x, -extents.y).Rotate(rotation).x;
	else if (rotation <= 180)
		return position.x + extents.Rotate(rotation).x;
	else if (rotation <= 270)
		return position.x - Vector2(extents.x, -extents.y).Rotate(rotation).x;
	else
		return position.x - extents.Rotate(rotation).x;
}


float BoundingRect::GetUpperBound()
{
	if (rotation <= 90)
		return position.y + extents.Rotate(rotation).y;
	else if (rotation <= 180)
		return position.y + Vector2(extents.x, -extents.y).Rotate(rotation).y;
	else if (rotation <= 270)
		return position.y - extents.Rotate(rotation).y;
	else
		return position.y - Vector2(extents.x, -extents.y).Rotate(rotation).y;
}


float BoundingRect::GetLowerBound()
{
	if (rotation <= 90)
		return position.y - extents.Rotate(rotation).y;
	else if (rotation <= 180)
		return position.y - Vector2(extents.x, -extents.y).Rotate(rotation).y;
	else if (rotation <= 270)
		return position.y + extents.Rotate(rotation).y;
	else
		return position.y + Vector2(extents.x, -extents.y).Rotate(rotation).y;
}


Vector2 BoundingRect::GetPosition()
{
	return position;
}


Vector2 BoundingRect::GetDimension()
{
	return dimension;
}


Vector2 BoundingRect::GetExtents()
{
	return extents;
}


float BoundingRect::GetRotation()
{
	return rotation;
}


bool BoundingRect::CheckOverlap(BoundingRect other)
{
	auto points = other.GetBoundingPoints();
	for (auto it = points.begin(); it != points.end(); it++) {
		if (CheckPointInisde((*it - position).Rotate(-rotation)))
			return true;
	}

	if (CheckPointInisde(other.position - position))
		return true;

	return other.CheckOverlap(*this);
}


bool BoundingRect::CheckPointInisde(Vector2 point)
{
	Vector2 localPoint = (point - position).Rotate(-rotation);
	if (localPoint.x < extents.x &&
		localPoint.x > -extents.x &&
		localPoint.y < extents.x &&
		localPoint.y > -extents.y)
		return true;
	return false;
}


std::vector<Vector2> BoundingRect::GetBoundingPoints()
{
	std::vector<Vector2> points;
	points.push_back(position + extents);
	points.push_back(position + Vector2(extents.x, -extents.y));
	points.push_back(position - extents);
	points.push_back(position + Vector2(-extents.x, extents.y));
	return points;
}


void BoundingRect::AdjustBoundToFit(BoundingRect other)
{
	Vector2 offset = other.position - position;
	//Right
	if (offset.x > 0) {
		//Right Up
		if (offset.y > 0) {
			Vector2 RU = offset + other.extents;
			Vector2 LD = extents * -1;

			SetPosition((RU + LD) * .5);
			SetDimension(RU - LD);
		}
		//Right Down
		else {
			Vector2 RD = offset + Vector2(other.extents.x, -other.extents.y);
			Vector2 LU = Vector2(-extents.x, extents.y);

			SetPosition((RD + LU) * .5);
			SetDimension((RD - LU).Unsign());
		}
	}
	//Left
	else {
		//Left Up
		if (offset.y > 0) {
			Vector2 LU = offset + Vector2(-other.extents.x, other.extents.y);
			Vector2 RD = Vector2(extents.x, -extents.y);

			SetPosition((LU + RD) * .5);
			SetDimension((LU - RD).Unsign());
		}
		//Left Down
		else {
			Vector2 LD = offset - other.extents;
			Vector2 RU = extents * -1;

			SetPosition((LD + RU) * .5);
			SetDimension(LD - RU);
		}
	}
}


BoundingRect BoundingRect::GetNonRotatedBound()
{
	BoundingRect nonRotBound;
	nonRotBound.SetPosition(position);
	nonRotBound.SetRotation(0);
	float maxY = GetUpperBound();
	float minY = GetLowerBound();
	float maxX = GetRightBound();
	float minX = GetLeftBound();
	nonRotBound.SetDimension(Vector2(maxX - minX, maxY - minY));

	return nonRotBound;
}
