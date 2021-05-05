#include "GL.h"
#include <vector>
#include <list>
#include <memory>
#include "MeshRenderer.h"


class Graphics
{
private:

public:
	static std::list<MeshRenderer*> meshRenderers;
	static std::shared_ptr<Camera> camera;
	static long renderedFrames;
	static unsigned int GL_CurrentlyBoundShaderProgram;

	void static DrawRenderer();
	void static RegisterMeshRenderer(MeshRenderer*);
	void static UnregeisterMeshRenderer(MeshRenderer*);
};

