#include "SkyboxRenderer.h"
#include "../../Graphics/Textures/CubeMap.h"
#include "../../Graphics/Textures/Texture2D.h"
#include "../../Math/Color.h"
#include "Mesh.h"
#include "../Shaders/Shader.h"
#include "../Shaders/ShaderLibrary.h"
#include "../Rendering/Material.h"
#include "../Rendering/Camera.h"



void SkyboxRenderer::SetSkybox(std::vector<std::string> filePaths)
{
	auto _mesh = new Mesh("res\\meshes\\Skybox.obj");
	_mesh->ExcludeMeshData(Mesh::MeshData::Everything);
	_mesh->IncludeMeshData(Mesh::MeshData::RawMeshPoint);
	SetMesh(std::shared_ptr<Mesh>(_mesh));

	auto shader = ShaderLibrary::GetShader("Internal_Skybox_Shader");
	if (shader == nullptr) {
		Shader::CreateSkyboxShader("Internal_Skybox_Shader");
	}

	SetMaterial(std::shared_ptr<Material>(new Material("skyboxMat", "Internal_Skybox_Shader")));
	auto cubeMap = new CubeMap();
	if (cubeMap->LoadCubeMap(filePaths, "skyboxCubeMap", Texture::Wrapping::ClampEdge, Texture::Filtering::Linear)) {
		material->CreateTextureProperty("Skybox", std::shared_ptr<Texture>(cubeMap), Material::TextureSlot::Skybox);
		material->mainColor = Color(1.f, 1, 1, 1);
		isActive = true;
	}
	else {
		isActive = false;
	}
}


void SkyboxRenderer::SetEnvironment(std::string filePath)
{
	auto _mesh = new Mesh("res\\meshes\\Environment.obj");
	_mesh->ExcludeMeshData(Mesh::MeshData::Everything);
	_mesh->IncludeMeshData(Mesh::MeshData::RawMeshPoint);
	SetMesh(std::shared_ptr<Mesh>(_mesh));

	auto shader = ShaderLibrary::GetShader("Internal_Environment_Shader");
	if (shader == nullptr) {
		Shader::CreateEnvironmentShader("Internal_Environment_Shader");
	}

	SetMaterial(std::shared_ptr<Material>(new Material("environmentMat", "Internal_Environment_Shader")));
	auto environmentMap = new Texture2D();
	if (environmentMap->LoadTexture2D(filePath, "environmentMap", Texture::Wrapping::Repeat, Texture::Filtering::Linear)) {
		material->CreateTextureProperty("Environment", std::shared_ptr<Texture>(environmentMap), Material::TextureSlot::Environment);
		material->mainColor = Color(1.f, 1, 1, 1);
		isActive = true;
	}
	else {
		isActive = false;
	}
}


void SkyboxRenderer::Draw(std::shared_ptr<Camera> drawingCam) {
	if (!isActive) return;

	//Binding GL Buffers
	GL_Call(glBindVertexArray(GL_VertexArrayObject));
	GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_IndexBufferObject));

	//Binding GL ShaderProg
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	shader->GL_BindProgram();

	//Material Uniforms
	material->GL_SetProperties();

	//Camera Uniforms
	drawingCam->GL_SetCameraUniformForSkybox(std::shared_ptr<Shader>(shader));


	//Applying Uniform Data
	//shader->GL_SetUniforms();

	//Gl Draw Call
	glDrawElements(GL_TRIANGLES, GL_BufferData->serializedIndices.size(), GL_UNSIGNED_INT, nullptr);

	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	GL_Call(glBindVertexArray(0));
	GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
