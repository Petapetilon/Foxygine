#include "GL.h"
#include <vector>
#include <string>
#include <memory>
#include <list>
#include <iostream>
#include <thread>
#include "../Foxygine.h"

#ifdef WIREFRAME_DEBUG
	#define DRAW_WIREFRAME
#endif


class MeshRenderer;
class SkyboxRenderer;
class Shader;
class Light;
class Camera;
class Canvas;
class Texture;


class Graphics
{
private:
	static SkyboxRenderer* skyBoxRenderer;
	static Shader* currentlyBoundShader;

	static std::list<MeshRenderer*> meshRenderers;
	static std::list<Canvas*> canvases;
	static std::list<Light*> lights;


	static void RenderShadowPrePassForwarded();
	static void RenderUnlitPassForwarded();
	static void RenderUIPassForwarded();

	//static void RenderShadowPrePassDeffered();
	//static void RenderUnlitPassDeffered();
	//static void RenderLitPassDeffered();
	//static void RenderUIPassDeffered();

public:
	static void RenderLitPassForwarded();
	static std::shared_ptr<Camera> camera;
	static long renderedFrames;
	static int CSM_ShadowResolution;
	static int PointLight_ShadowResolution;
	static int SpotLight_ShadowResolution;

	static void Init();
	static void RenderFrame();

	static void RegisterMeshRenderer(MeshRenderer*);
	static void UnregeisterMeshRenderer(MeshRenderer*);
	static void RegisterLight(Light*);
	static void UnregisterLight(Light*);
	static void RegisterCanvas(Canvas*);
	static void UnregisterCanvas(Canvas*);


	/// <summary> Files are: left, right, down, up, front, back </summary>
	static void SetSkybox(std::vector<std::string> filePaths);

	/// <summary> Sets the Skybox from the Given Folder by Loading Textures with the given Filename and appending _left, _right, _up, _down, _front, _back</summary>
	static void SetSkybox(std::string relativeDir, std::string fileName, std::string fileType);

	static void SetEnvironment(std::string filePath);

	static void OnWindowResize(int width, int height);
	static void FinishLoadingResources();
	static Shader* GL_GetCurrentlyBoundShader();
	static void GL_SetCurrentlyBoundShader(Shader* shader);
	static void RenderShadowPass(Light* light);
	static bool TryGetSkybox(std::shared_ptr<Texture>& result);
};

