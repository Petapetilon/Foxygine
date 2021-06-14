#include "MeshRenderer.h"
#include "Mesh.h"
#include "../Graphics.h"
#include "../Shaders/Shader.h"
#include "../Rendering/Material.h"
#include "../Lights/Light.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/Transform.h"
#include "../../Peripherals/Window.h"
#include "../Shaders/Shader.h"
#include "../Rendering/Camera.h"



MeshRenderer::MeshRenderer(const std::string& objFile, bool _castShadow)
{
	//GL VAO
	GL_Call(glGenVertexArrays(1, &GL_VertexArrayObject));
	//GL VBO
	GL_Call(glGenBuffers(1, &GL_VertexBufferObject));
	//GL Indices
	GL_Call(glGenBuffers(1, &GL_IndexBufferObject));


	SetMesh(std::shared_ptr<Mesh>(new Mesh(objFile)));
	//Graphics::RegisterMeshRenderer(this);
	isActive = true;
	castShadow = _castShadow;
}


MeshRenderer::MeshRenderer()
{
	//GL VAO
	GL_Call(glGenVertexArrays(1, &GL_VertexArrayObject));
	//GL VBO
	GL_Call(glGenBuffers(1, &GL_VertexBufferObject));
	//GL Indices
	GL_Call(glGenBuffers(1, &GL_IndexBufferObject));

	//Graphics::RegisterMeshRenderer(this);
	isActive = true;
	castShadow = false;
}


MeshRenderer::~MeshRenderer()
{
	Graphics::UnregeisterMeshRenderer(this);

	GL_Call(glDeleteVertexArrays(1, &GL_VertexArrayObject));
	GL_Call(glDeleteBuffers(1, &GL_VertexBufferObject));
	GL_Call(glDeleteBuffers(1, &GL_IndexBufferObject));
}


void MeshRenderer::SetMesh(std::shared_ptr<Mesh> _mesh)
{
	mesh = std::shared_ptr<Mesh>(_mesh);
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


void MeshRenderer::Draw(std::shared_ptr<Camera> drawingCam) {
	if (!isActive || !gameObject) return;

	//Binding GL Buffers
	GL_Call(glBindVertexArray(GL_VertexArrayObject));
	GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_IndexBufferObject));

	if(shader->GL_BindProgram()){
		//Camera Uniforms
		drawingCam->GL_SetCameraUniform(std::shared_ptr<Shader>(shader));

		//Graphics Uniforms
		shader->SetValueVec1I("u_RenderedFrames", Graphics::renderedFrames);
	}

	//Material Uniforms
	material->GL_SetProperties();

	//Object Uniforms
	shader->SetValueMat4("u_ObjectTransform", *transform->GetGlobalMatrix());

	//Gl Draw Call
	glDrawElements(GL_TRIANGLES, GL_BufferData->serializedIndices.size(), GL_UNSIGNED_INT, nullptr);

#ifdef DRAW_WIREFRAME
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(3);
	
	auto wireFrameShader = ShaderLibrary::GL_BindTessellatedWireframeShader();
	wireFrameShader->SetValueMat4("u_ObjectTransform", *transform->GetGlobalMatrix());
	wireFrameShader->SetValueVec1I("u_RenderedFrames", Graphics::renderedFrames);
	wireFrameShader->SetValueVec2("u_ScreenRes", Window::GetInstance()->GetWindowResolution());
	drawingCam->GL_SetCameraUniform(wireFrameShader);
	GL_Call(glPatchParameteri(GL_PATCH_VERTICES, 3));
	glDrawElements(GL_PATCHES, GL_BufferData->serializedIndices.size(), GL_UNSIGNED_INT, nullptr);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
#endif
}


void MeshRenderer::DrawShadowMap(Light* light)
{
	if (!isActive || !gameObject || !castShadow) return;

	GL_Call(glBindVertexArray(GL_VertexArrayObject));
	GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_IndexBufferObject));

	//Object Uniforms
	Graphics::GL_GetCurrentlyBoundShader()->SetValueMat4("u_ObjectTransform", *transform->GetGlobalMatrix());

	//Gl Draw Call
	glDrawElements(GL_TRIANGLES, GL_BufferData->serializedIndices.size(), GL_UNSIGNED_INT, nullptr);
}


Component* MeshRenderer::Copy(std::size_t& compHash)
{
	auto copy = new MeshRenderer();
	copy->SetMesh(mesh);
	copy->SetMaterial(material);
	copy->castShadow = castShadow;
	compHash = typeid(copy).hash_code();
	return copy;
}

void MeshRenderer::OnAttach()
{
	Graphics::RegisterMeshRenderer(this);
}


void MeshRenderer::OnDetach()
{
	Graphics::UnregeisterMeshRenderer(this);
}

