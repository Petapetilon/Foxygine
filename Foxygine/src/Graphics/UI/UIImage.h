#pragma once
#include "UIElement.h"
#include "../Rendering/Renderer.h"
#include "../GL.h"
#include <vector>


class Mesh;
class Camera;
class Texture2D;
struct SerializedMesh;


class UIImage : public UIElement, public Renderer
{
protected:
	std::shared_ptr<SerializedMesh> GL_BufferData;
	Mesh* mesh;

	void GL_SetupData();

public:
	UIImage();
	UIImage(UIElement* parent);
	UIImage(UIElement* parent, std::string textureFilePath);
	UIImage(UIElement* parent, std::shared_ptr<Texture2D> texture);

	void Draw() override;
};

