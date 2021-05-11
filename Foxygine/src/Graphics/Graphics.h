#include "GL.h"
#include <vector>
#include <string>
#include <memory>
#include <list>
#include <iostream>


class MeshRenderer;
class SkyboxRenderer;
class Shader;
class Light;
class Camera;


class Graphics
{
private:
	static SkyboxRenderer* skyBoxRenderer;
	static Shader* currentlyBoundShader;

	static std::list<MeshRenderer*> meshRenderers;
	static std::list<Light*> lights;

	static int numberOfDirLights;
	static int numberOfPointLights;

public:
	static std::shared_ptr<Camera> camera;
	static long renderedFrames;

	static void Init();
	static void DrawRenderer();
	static void RegisterMeshRenderer(MeshRenderer*);
	static void UnregeisterMeshRenderer(MeshRenderer*);
	static void RegisterLight(Light*);
	static void UnregisterLight(Light*);
	static void SetSkybox(std::vector<std::string> filePaths);
	static void OnWindowResize(int width, int height);
	static void FinishLoadingResources();
	static Shader* GL_GetCurrentlyBoundShader();
	static void GL_SetCurrentlyBoundShader(Shader* shader);
};

