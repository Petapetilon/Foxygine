#include "UIImage.h"
#include "../Mesh/Mesh.h"
#include "../Rendering/Material.h"
#include "../Textures/Texture2D.h"
#include "../Rendering/Camera.h"
#include "../Graphics.h"
#include "BoundingRect.h"
#include "Canvas.h"
#include "../../Math/Color.h"
#include "../../Peripherals/Window.h"



void UIImage::GL_SetupData()
{
	//GL VAO
	GL_Call(glGenVertexArrays(1, &GL_VertexArrayObject));
	//GL VBO
	GL_Call(glGenBuffers(1, &GL_VertexBufferObject));
	//GL Indices
	GL_Call(glGenBuffers(1, &GL_IndexBufferObject));

	SetMaterial(std::shared_ptr<Material>(new Material("imageMat", "Basic Unlit UI")));
	material->mainColor = Color(1, 1, 1, 1.f);
	mesh = new Mesh();

	std::vector<Vector3> points;
	points.push_back(Vector3(0, 0, 0));
	points.push_back(Vector3(1, 0, 0));
	points.push_back(Vector3(1, 1, 0));
	points.push_back(Vector3(0, 1, 0));
	mesh->SetPositions(points);

	std::vector<Vector2> uvs;
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 1));
	mesh->SetUVs(uvs);

	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);
	mesh->SetIndices(indices);

	
	GL_BufferData = mesh->SerializeMeshData();

	//Vertex Array Object for saving the Vertex Attrib Pointers for each Buffer
	GL_Call(glBindVertexArray(GL_VertexArrayObject));

	//Bind Vertex Buffer 
	GL_Call(glBindBuffer(GL_ARRAY_BUFFER, GL_VertexBufferObject));
	GL_Call(glBufferData(GL_ARRAY_BUFFER,
		GL_BufferData->serializedVertexData.size() * sizeof(GL_FLOAT),
		&GL_BufferData->serializedVertexData[0], GL_STATIC_DRAW));


	//Vertex Buffer Object Layout 
	int attribPointerSize = 0;
	for (int i = 0; i < GL_BufferData->vertexAttribPointerCount; i++) {
		attribPointerSize += GL_BufferData->vertexAttribPointerDataSize[i] * sizeof(GL_BufferData->vertexAttribPointerDataType[i]);
	}

	for (int i = 0; i < GL_BufferData->vertexAttribPointerCount; i++) {
		int attribPointerStride = 0;
		for (int j = 0; j < i; j++) {
			attribPointerStride += GL_BufferData->vertexAttribPointerDataSize[j] * sizeof(GL_BufferData->vertexAttribPointerDataType[j]);
		}

		GL_Call(glEnableVertexAttribArray(i));
		GL_Call(glVertexAttribPointer(
			i,
			GL_BufferData->vertexAttribPointerDataSize[i],
			GL_BufferData->vertexAttribPointerDataType[i],
			GL_FALSE,
			attribPointerSize, (void*)(attribPointerStride)));
	}

	//Index Buffer
	GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_IndexBufferObject));
	GL_Call(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		GL_BufferData->serializedIndices.size() * sizeof(unsigned int),
		&GL_BufferData->serializedIndices[0], GL_STATIC_DRAW));

	GL_Call(glBindVertexArray(0));
	GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}


void UIImage::OnTransformChanged()
{
	Vector2 res = Window::GetInstance()->GetWindowResolution();
	auto corners = transformRect->GetBoundingPointsGlobal();

	GL_BufferData->serializedVertexData[0] = corners[0].x / res.x;
	GL_BufferData->serializedVertexData[1] = corners[0].y / res.y;
	GL_BufferData->serializedVertexData[5] = corners[1].x / res.x;
	GL_BufferData->serializedVertexData[6] = corners[1].y / res.y;
	GL_BufferData->serializedVertexData[10] = corners[2].x / res.x;
	GL_BufferData->serializedVertexData[11] = corners[2].y / res.y;
	GL_BufferData->serializedVertexData[15] = corners[3].x / res.x;
	GL_BufferData->serializedVertexData[16] = corners[3].y / res.y;


	GL_Call(glBindVertexArray(GL_VertexArrayObject));

	//Bind Vertex Buffer 
	GL_Call(glBindBuffer(GL_ARRAY_BUFFER, GL_VertexBufferObject));
	GL_Call(glBufferData(GL_ARRAY_BUFFER,
		GL_BufferData->serializedVertexData.size() * sizeof(GL_FLOAT),
		&GL_BufferData->serializedVertexData[0], GL_STATIC_DRAW));

	GL_Call(glBindVertexArray(0));

	__super::OnTransformChanged();
}


UIImage::UIImage()
{
	transformRect = std::shared_ptr<BoundingRect>(new BoundingRect());
	boundingRect = std::shared_ptr<BoundingRect>(new BoundingRect());

	GL_SetupData();
}


UIImage::UIImage(std::string _name, UIElement* parent)
{
	transformRect = std::shared_ptr<BoundingRect>(new BoundingRect());
	boundingRect = std::shared_ptr<BoundingRect>(new BoundingRect());

	GL_SetupData();

	name = _name;
	parent->AddElement(this);
}


UIImage::UIImage(std::string _name, UIElement* parent, std::string textureFilePath)
{
	transformRect = std::shared_ptr<BoundingRect>(new BoundingRect());
	boundingRect = std::shared_ptr<BoundingRect>(new BoundingRect());

	GL_SetupData();

	name = _name;
	parent->AddElement(this);
	SetActive(true);
	
	auto colTex = std::shared_ptr<Texture2D>(new Texture2D());
	colTex->LoadTexture2D("textureFilePath", "color", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
	material->CreateTextureProperty("color", std::shared_ptr<Texture>(colTex), Material::TextureSlot::BaseColor);
}


UIImage::UIImage(std::string _name, UIElement* parent, std::shared_ptr<Texture2D> texture)
{
	transformRect = std::shared_ptr<BoundingRect>(new BoundingRect());
	boundingRect = std::shared_ptr<BoundingRect>(new BoundingRect());

	GL_SetupData();

	name = _name;
	parent->AddElement(this);
	SetActive(true);

	material->CreateTextureProperty("color", std::shared_ptr<Texture>(texture), Material::TextureSlot::BaseColor);
}


void UIImage::Draw()
{
	if (!isActive) return;

	//Binding GL Buffers
	GL_Call(glBindVertexArray(GL_VertexArrayObject));
	GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_IndexBufferObject));

	if (shader->GL_BindProgram()) {
		//Graphics Uniforms
		int renderedFrames = Graphics::renderedFrames;
		shader->SetValueVec1I("u_RenderedFrames", renderedFrames);
		//shader->SetValueMat4("u_CanvasToScreen", canvas->GetCombinedTransform());
	}

	//Material Uniforms
	material->GL_SetProperties();

	//Object Uniforms
	//shader->SetValueMat4("u_ObjectTransform", transformRect->GetTransform());
	//
	//
	//shader->SetValueVec2("u_PositionOffset", transformRect->GetPosition());
	//shader->SetValueVec2("u_Scale", Vector2(100, 100));
	//shader->SetValueVec2("u_WindowSize", Window::GetInstance()->GetWindowResolution());

	//Gl Draw Call
	glDrawElements(GL_TRIANGLES, GL_BufferData->serializedIndices.size(), GL_UNSIGNED_INT, nullptr);

	//Draw children
	__super::Draw();
}
