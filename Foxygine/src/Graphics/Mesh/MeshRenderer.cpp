#include "MeshRenderer.h"
#include "Mesh.h"
#include "InstanceRenderer.h"
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
		GL_BufferData->serializedVertexData.data(), GL_STATIC_DRAW));


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

	//Gl Draw Call
	if (instanceRenderers.size() > 0) {
		shader->SetValueVec1I("u_InstancedRendering", 1);
		GL_Call(glDrawElementsInstanced(GL_TRIANGLES, GL_BufferData->serializedIndices.size(), GL_UNSIGNED_INT, 0, instanceRenderers.size()));
	}
	else {
		shader->SetValueVec1I("u_InstancedRendering", 0);
		shader->SetValueMat4("u_ObjectTransform", *transform->GetGlobalMatrix());
		glDrawElements(GL_TRIANGLES, GL_BufferData->serializedIndices.size(), GL_UNSIGNED_INT, nullptr);
	}

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
	//glDrawElements(GL_TRIANGLES, GL_BufferData->serializedIndices.size(), GL_UNSIGNED_INT, nullptr);
	if (instanceRenderers.size() > 0) {
		Graphics::GL_GetCurrentlyBoundShader()->SetValueVec1I("u_InstancedRendering", 1);
		GL_Call(glDrawElementsInstanced(GL_TRIANGLES, GL_BufferData->serializedIndices.size(), GL_UNSIGNED_INT, 0, instanceRenderers.size()));
	}
	else {
		Graphics::GL_GetCurrentlyBoundShader()->SetValueVec1I("u_InstancedRendering", 0);
		Graphics::GL_GetCurrentlyBoundShader()->SetValueMat4("u_ObjectTransform", *transform->GetGlobalMatrix());
		glDrawElements(GL_TRIANGLES, GL_BufferData->serializedIndices.size(), GL_UNSIGNED_INT, nullptr);
	}
}


void MeshRenderer::LinkInstanceRenderer(InstanceRenderer* renderer)
{
	if (mesh == nullptr) return;

	instanceRenderers[renderer->gameObject->uniqueID] = renderer;
	renderer->mainRenderer = this;
	renderer->instanceIndex = instanceRenderers.size() - 1;

	if (instanceRenderers.size() == 0) {
		glDeleteBuffers(1, &GL_InstanceDataBufferObject);
		gameObject->RemoveComponent<InstanceRenderer>();
		return;
	}

	if (instanceRenderers.size() == 1) {
		GL_InstanceBufferData = new float[16 * 1024];
		glGenBuffers(1, &GL_InstanceDataBufferObject);

		GL_Call(glBindVertexArray(GL_VertexArrayObject));
		GL_Call(glBindBuffer(GL_ARRAY_BUFFER, GL_InstanceDataBufferObject));
		GL_Call(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16 * 1024, GL_InstanceBufferData, GL_DYNAMIC_DRAW));

		GL_Call(glEnableVertexAttribArray(5));
		GL_Call(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(0 * sizeof(float))));
		GL_Call(glEnableVertexAttribArray(6));
		GL_Call(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(4 * sizeof(float))));
		GL_Call(glEnableVertexAttribArray(7));
		GL_Call(glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(8 * sizeof(float))));
		GL_Call(glEnableVertexAttribArray(8));
		GL_Call(glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12 * sizeof(float))));


		GL_Call(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GL_Call(glVertexAttribDivisor(5, 1));
		GL_Call(glVertexAttribDivisor(6, 1));
		GL_Call(glVertexAttribDivisor(7, 1));
		GL_Call(glVertexAttribDivisor(8, 1));
		GL_Call(glBindVertexArray(0));


		InstanceRenderer* myInstance = nullptr;
		gameObject->AddComponent<InstanceRenderer>(new InstanceRenderer(this));
	}

	renderer->mainRenderer->UpdateInstanceRendererData(renderer);
}


void MeshRenderer::UnlinkInstanceRenderer(InstanceRenderer* renderer)
{
	instanceRenderers.erase(renderer->gameObject->uniqueID);
	renderer->mainRenderer = nullptr;

	short k = 0;
	for (auto instance : instanceRenderers) {
		instance.second->instanceIndex = k++;
	}
}


void MeshRenderer::UpdateInstanceRendererData(InstanceRenderer* renderer)
{
	auto mat = *renderer->transform->GetGlobalMatrix();
	GL_InstanceBufferData[renderer->instanceIndex * 16] = mat[0][0];
	GL_InstanceBufferData[renderer->instanceIndex * 16 + 1] = mat[0][1];
	GL_InstanceBufferData[renderer->instanceIndex * 16 + 2] = mat[0][2];
	GL_InstanceBufferData[renderer->instanceIndex * 16 + 3] = mat[0][3];

	GL_InstanceBufferData[renderer->instanceIndex * 16 + 4] = mat[1][0];
	GL_InstanceBufferData[renderer->instanceIndex * 16 + 5] = mat[1][1];
	GL_InstanceBufferData[renderer->instanceIndex * 16 + 6] = mat[1][2];
	GL_InstanceBufferData[renderer->instanceIndex * 16 + 7] = mat[1][3];

	GL_InstanceBufferData[renderer->instanceIndex * 16 + 8] = mat[2][0];
	GL_InstanceBufferData[renderer->instanceIndex * 16 + 9] = mat[2][1];
	GL_InstanceBufferData[renderer->instanceIndex * 16 + 10] = mat[2][2];
	GL_InstanceBufferData[renderer->instanceIndex * 16 + 11] = mat[2][3];

	GL_InstanceBufferData[renderer->instanceIndex * 16 + 12] = mat[3][0];
	GL_InstanceBufferData[renderer->instanceIndex * 16 + 13] = mat[3][1];
	GL_InstanceBufferData[renderer->instanceIndex * 16 + 14] = mat[3][2];
	GL_InstanceBufferData[renderer->instanceIndex * 16 + 15] = mat[3][3];

	GL_Call(glBindBuffer(GL_ARRAY_BUFFER, GL_InstanceDataBufferObject));
	GL_Call(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16 * 1024, GL_InstanceBufferData, GL_DYNAMIC_DRAW));
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


Component* MeshRenderer::CopyLinked(std::size_t& compHash)
{
	auto copy = new InstanceRenderer(this);
	copy->mainRenderer = this;
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


void MeshRenderer::OnDestroy()
{
	Graphics::UnregeisterMeshRenderer(this);

	GL_BufferData.reset();
	material.reset();
	mesh.reset();

	GL_Call(glDeleteVertexArrays(1, &GL_VertexArrayObject));
	GL_Call(glDeleteBuffers(1, &GL_VertexBufferObject));
	GL_Call(glDeleteBuffers(1, &GL_IndexBufferObject));
}
