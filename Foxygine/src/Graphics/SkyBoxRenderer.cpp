#include "SkyBoxRenderer.h"
#include "../Graphics/Textures/CubeMap.h"
#include "../Math/Color.h"


void SkyBoxRenderer::SetSkybox(std::vector<std::string> filePaths)
{
	SetMesh(std::shared_ptr<Mesh>(new Mesh("res\\meshes\\Skybox.obj")));
	auto shader = ShaderLibrary::GetShader("skyboxShader");
	if (shader == nullptr) {
		Shader::CreateSkyboxShader("skyboxShader");
	}

	SetMaterial(std::shared_ptr<Material>(new Material("skyboxMat", "skyboxShader")));
	auto cubeMap = new CubeMap();
	cubeMap->LoadCubeMap(filePaths, "skyboxCubeMap", Texture::Wrapping::ClampEdge, Texture::Filtering::Linear);
	material->CreateTextureProperty("color", std::shared_ptr<Texture>(cubeMap), Material::TextureSlot::BaseColor);
	material->SetMainColor(Color(1.f, 1, 1, 1));
}


void SkyBoxRenderer::Draw(std::shared_ptr<Camera> drawingCam) {
	if (!isActive || !gameObject) return;
	//if (!shader) shader = std::shared_ptr<Shader>(new BasicShader());

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
