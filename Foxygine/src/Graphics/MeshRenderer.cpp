#include "MeshRenderer.h"
#include "Graphics.h"
#include "Shaders/Shader.h"
#include "Shaders/ShaderPass.h"
#include "Material.h"
#include <iostream>



MeshRenderer::MeshRenderer(const std::string& objFile)
{
	std::cout << "creating MeshRenderer" << std::endl;

	//GL VAO
	glGenVertexArrays(1, &GL_VertexArrayObject);
	//GL VBO
	glGenBuffers(1, &GL_VertexBufferObject);
	//GL Indices
	glGenBuffers(1, &GL_IndexBufferObject);


	SetMesh(std::shared_ptr<Mesh>(new Mesh(objFile)));
	Graphics::RegisterMeshRenderer(this);
	isActive = true;
}



MeshRenderer::MeshRenderer()
{
	std::cout << "creating MeshRenderer" << std::endl;

	//GL VAO
	glGenVertexArrays(1, &GL_VertexArrayObject);
	//GL VBO
	glGenBuffers(1, &GL_VertexBufferObject);
	//GL Indices
	glGenBuffers(1, &GL_IndexBufferObject);

	Graphics::RegisterMeshRenderer(this);
	isActive = true;
}



MeshRenderer::~MeshRenderer()
{
	Graphics::UnregeisterMeshRenderer(this);

	glDeleteVertexArrays(1, &GL_VertexArrayObject);
	glDeleteBuffers(1, &GL_VertexBufferObject);
	glDeleteBuffers(1, &GL_IndexBufferObject);
}



void MeshRenderer::SetMesh(std::shared_ptr<Mesh> _mesh)
{
	mesh = std::shared_ptr<Mesh>(_mesh);
	GL_BufferData = mesh->SerializeMeshData();

	//Vertex Array Object for saving the Vertex Attrib Pointers for each Buffer
	glBindVertexArray(GL_VertexArrayObject);
	
	//Bind Vertex Buffer 
	glBindBuffer(GL_ARRAY_BUFFER, GL_VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 
		GL_BufferData->serializedVertexData.size() * sizeof(GL_FLOAT),
		&GL_BufferData->serializedVertexData[0], GL_STATIC_DRAW);


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

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(
			i, 
			GL_BufferData->vertexAttribPointerDataSize[i],
			GL_BufferData->vertexAttribPointerDataType[i], 
			GL_FALSE, 
			attribPointerSize, (void*)(attribPointerStride));
	}

	//Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_IndexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		GL_BufferData->serializedIndices.size() * sizeof(unsigned int),
		&GL_BufferData->serializedIndices[0], GL_STATIC_DRAW);
}



void MeshRenderer::Draw(std::shared_ptr<Camera> drawingCam) {
	if (!isActive || !gameObject) return;
	//if (!shader) shader = std::shared_ptr<Shader>(new BasicShader());

	//Binding GL Buffers
	glBindVertexArray(GL_VertexArrayObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_IndexBufferObject);

	//Binding GL ShaderProg
	//std::cout << shader->name << std::endl;
	shader->GL_BindProgram();
	
	//Material Uniforms
	material->GL_SetProperties();

	//Camera Uniforms
	drawingCam->GL_SetCameraUniform(std::shared_ptr<Shader>(shader));

	//Graphics Uniforms
	int renderedFrames = Graphics::renderedFrames;
	shader->SetShaderPass(new ShaderPassVec1I(&renderedFrames, "u_RenderedFrames"));
	
	//Object Uniforms
	glm::mat4 orientation = transform->GetOrientationMatrix();
	shader->SetShaderPass(new ShaderPassMat4(&orientation, "u_ObjectOrientation"));
	shader->SetShaderPass(new ShaderPassMat4(transform->GetGlobalMatrix(), "u_ObjectTransform"));
	
	//Applying Uniform Data
	shader->GL_SetUniforms();

	//Gl Draw Call
	glDrawElements(GL_TRIANGLES, GL_BufferData->serializedIndices.size(), GL_UNSIGNED_INT, nullptr);
}

