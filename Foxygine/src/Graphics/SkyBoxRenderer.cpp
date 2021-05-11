#include "SkyboxRenderer.h"
#include "../Graphics/Textures/CubeMap.h"
#include "../Math/Color.h"


void SkyboxRenderer::SetSkybox(std::vector<std::string> filePaths)
{
	auto _mesh = new Mesh("res\\meshes\\Skybox.obj");
	_mesh->ExcludeMeshData(Mesh::MeshData::Everything);
	_mesh->IncludeMeshData(Mesh::MeshData::RawMeshPoint);
	SetMesh(std::shared_ptr<Mesh>(_mesh));

	auto shader = ShaderLibrary::GetShader("skyboxShader");
	if (shader == nullptr) {
		Shader::CreateSkyboxShader("skyboxShader");
	}

	SetMaterial(std::shared_ptr<Material>(new Material("skyboxMat", "skyboxShader")));
	auto cubeMap = new CubeMap();
	if (cubeMap->LoadCubeMap(filePaths, "skyboxCubeMap", Texture::Wrapping::ClampEdge, Texture::Filtering::Linear)) {
		material->CreateTextureProperty("Skybox", std::shared_ptr<Texture>(cubeMap), Material::TextureSlot::Skybox);
		material->SetMainColor(Color(0.f, 1, 1, 1));
		isActive = true;
	}
	else {
		isActive = false;
	}
}


void SkyboxRenderer::Draw(std::shared_ptr<Camera> drawingCam) {
	if (!isActive) return;

	//Binding GL Buffers
	glBindVertexArray(GL_VertexArrayObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_IndexBufferObject);

	//Binding GL ShaderProg
	glDepthMask(GL_FALSE);
	shader->GL_BindProgram();

	//Material Uniforms
	material->GL_SetProperties();

	//Camera Uniforms
	drawingCam->GL_SetCameraUniformForSkybox(std::shared_ptr<Shader>(shader));


	//Applying Uniform Data
	shader->GL_SetUniforms();

	//Gl Draw Call
	glDrawElements(GL_TRIANGLES, GL_BufferData->serializedIndices.size(), GL_UNSIGNED_INT, nullptr);
	glDepthMask(GL_TRUE);
}
