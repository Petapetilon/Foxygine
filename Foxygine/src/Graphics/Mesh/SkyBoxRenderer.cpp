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





	//unsigned int captureFBO, captureRBO;
	//glGenFramebuffers(1, &captureFBO);
	//glGenRenderbuffers(1, &captureRBO);
	//
	//glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
	//
	////unsigned int envCubemap;
	////glGenTextures(1, &envCubemap);
	////glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	////for (unsigned int i = 0; i < 6; ++i)
	////{
	////	// note that we store each face with 16 bit floating point values
	////	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
	////		512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	////}
	////glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	////glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	////glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	////glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	////glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//
	//auto cubeMap = new CubeMap();
	//cubeMap->Create("skyBox", Texture::Wrapping::ClampEdge, Texture::Filtering::Linear);
	//
	//glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	//glm::mat4 captureViews[] =
	//{
	//   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	//   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	//   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	//   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	//   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	//   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	//};
	//
	//// convert HDR equirectangular environment map to cubemap equivalent
	//auto equirectangularToCubemapShader = Shader::CreateEmptyShader("equirectangularToCubemapShader");
	//equirectangularToCubemapShader.setInt("equirectangularMap", 0);
	//equirectangularToCubemapShader->SetValueMat4("projection", captureProjection);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, envor);
	//
	//glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	//glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	//for (unsigned int i = 0; i < 6; ++i)
	//{
	//	equirectangularToCubemapShader->SetValueMat4("view", captureViews[i]);
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
	//		GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	//	renderCube(); // renders a 1x1 cube
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void SkyboxRenderer::Draw(std::shared_ptr<Camera> drawingCam) {
	if (!isActive) return;

	//Binding GL Buffers
	GL_Call(glBindVertexArray(GL_VertexArrayObject));
	GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_IndexBufferObject));

	//Binding GL ShaderProg
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	shader->GL_BindProgram();

	//Material Uniforms
	material->GL_SetProperties();

	//Camera Uniforms
	drawingCam->GL_SetCameraUniformForSkybox(std::shared_ptr<Shader>(shader));

	//Gl Draw Call
	glDrawElements(GL_TRIANGLES, GL_BufferData->serializedIndices.size(), GL_UNSIGNED_INT, nullptr);

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	GL_Call(glBindVertexArray(0));
	GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
