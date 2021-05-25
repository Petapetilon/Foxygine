#include "Canvas.h"



glm::mat4 Canvas::GetCombinedTransform()
{
	return projectionMatrix;
}


void Canvas::Draw()
{
	for (auto it = children.begin(); it != children.end(); it++)
		(*it)->Draw();
}

