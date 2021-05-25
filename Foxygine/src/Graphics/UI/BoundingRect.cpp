#include "BoundingRect.h"
#include <gtx/transform.hpp>
#include <iostream>



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
		return position.x - Vector2(extents.x, -extents.y).Rotated(rotation).x;
	else if (rotation <= 180)
		return position.x - extents.Rotated(rotation).x;
	else if (rotation <= 270)
		return position.x + Vector2(extents.x, -extents.y).Rotated(rotation).x;
	else
		return position.x + extents.Rotated(rotation).x;
}


float BoundingRect::GetRightBound()
{
	if (rotation <= 90)
		return position.x + Vector2(extents.x, -extents.y).Rotated(rotation).x;
	else if (rotation <= 180)
		return position.x + extents.Rotated(rotation).x;
	else if (rotation <= 270)
		return position.x - Vector2(extents.x, -extents.y).Rotated(rotation).x;
	else
		return position.x - extents.Rotated(rotation).x;
}


float BoundingRect::GetUpperBound()
{
	if (rotation <= 90)
		return position.y + extents.Rotated(rotation).y;
	else if (rotation <= 180)
		return position.y + Vector2(extents.x, -extents.y).Rotated(rotation).y;
	else if (rotation <= 270)
		return position.y - extents.Rotated(rotation).y;
	else
		return position.y - Vector2(extents.x, -extents.y).Rotated(rotation).y;
}


float BoundingRect::GetLowerBound()
{
	if (rotation <= 90)
		return position.y - extents.Rotated(rotation).y;
	else if (rotation <= 180)
		return position.y - Vector2(extents.x, -extents.y).Rotated(rotation).y;
	else if (rotation <= 270)
		return position.y + extents.Rotated(rotation).y;
	else
		return position.y + Vector2(extents.x, -extents.y).Rotated(rotation).y;
}


Vector2 BoundingRect::GetUp()
{
	return Vector2(0, 1).Rotated(rotation);
}


Vector2 BoundingRect::GetRight()
{
	return Vector2(1, 0).Rotated(rotation);
}


Vector2 BoundingRect::GetDown()
{
	return Vector2(0, -1).Rotated(rotation);
}


Vector2 BoundingRect::GetLeft()
{
	return Vector2(-1, 0).Rotated(rotation);
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


bool BoundingRect::CheckOverlap(std::shared_ptr<BoundingRect> other)
{
	auto points = other->GetBoundingPointsGlobal();
	for (auto it = points.begin(); it != points.end(); it++) {
		if (CheckPointInisde((*it - position).Rotated(-rotation)))
			return true;
	}

	if (CheckPointInisde(other->position - position))
		return true;

	return other->CheckOverlap(std::shared_ptr<BoundingRect>(this));
}


bool BoundingRect::CheckPointInisde(Vector2 point)
{
	Vector2 localPoint = (point - position).Rotated(-rotation);
	if (localPoint.x < extents.x &&
		localPoint.x > -extents.x &&
		localPoint.y < extents.x &&
		localPoint.y > -extents.y)
		return true;
	return false;
}


std::vector<Vector2> BoundingRect::GetBoundingPointsLocal()
{
	std::vector<Vector2> points;
	points.push_back(position - extents);
	points.push_back(position + Vector2(extents.x, -extents.y));
	points.push_back(position + extents);
	points.push_back(position + Vector2(-extents.x, extents.y));
	return points;
}


std::vector<Vector2> BoundingRect::GetBoundingPointsGlobal()
{
	std::vector<Vector2> points;
	points.push_back(position + Vector2::Rotate(extents * -1.f, rotation));
	points.push_back(position + Vector2::Rotate(Vector2(extents.x, -extents.y), rotation));
	points.push_back(position + Vector2::Rotate(extents, rotation));
	points.push_back(position + Vector2::Rotate(Vector2(-extents.x, extents.y), rotation));
	return points;
}


void BoundingRect::AdjustBoundToFit(std::shared_ptr<BoundingRect> other)
{
	Vector2 offset = other->position - position;
	//Right
	if (offset.x > 0) {
		//Right Up
		if (offset.y > 0) {
			Vector2 RU = offset + other->extents;
			Vector2 LD = extents * -1;

			SetPosition((RU + LD) * .5);
			SetDimension(RU - LD);
		}
		//Right Down
		else {
			Vector2 RD = offset + Vector2(other->extents.x, -other->extents.y);
			Vector2 LU = Vector2(-extents.x, extents.y);

			SetPosition((RD + LU) * .5);
			SetDimension((RD - LU).Unsigned());
		}
	}
	//Left
	else {
		//Left Up
		if (offset.y > 0) {
			Vector2 LU = offset + Vector2(-other->extents.x, other->extents.y);
			Vector2 RD = Vector2(extents.x, -extents.y);

			SetPosition((LU + RD) * .5);
			SetDimension((LU - RD).Unsigned());
		}
		//Left Down
		else {
			Vector2 LD = offset - other->extents;
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


glm::mat4 BoundingRect::GetTransform()
{
	glm::mat4 mat(1);
	mat = glm::rotate(mat, rotation, glm::vec3(0, 0, 1));
	mat = glm::translate(mat, (glm::vec3)position);
	mat = glm::scale(mat, (glm::vec3)dimension);
	return mat;
}


void BoundingRect::Init()
{
	SetPosition(0, 0);
	SetDimension(0, 0);
	SetRotation(0);
}
