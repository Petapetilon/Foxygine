#include "GL.h"
#include <vector>
#include <list>
#include <memory>
#include "MeshRenderer.h"
#include "SkyBoxRenderer.h"


class Graphics
{
private:
	static std::shared_ptr<GameObject> skyBoxGo;
	static std::list<MeshRenderer*> meshRenderers;

public:
	static std::shared_ptr<Camera> camera;
	static long renderedFrames;
	static unsigned int GL_CurrentlyBoundShaderProgram;

	static void DrawRenderer();
	static void RegisterMeshRenderer(MeshRenderer*);
	static void UnregeisterMeshRenderer(MeshRenderer*);
	static void SetSkybox(std::vector<std::string> filePaths);
	static void OnWindowResize(int width, int height);
};

