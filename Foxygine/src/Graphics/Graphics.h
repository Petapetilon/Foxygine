#include "GL.h"
#include <vector>
#include <list>
#include <memory>
#include "MeshRenderer.h"


class Graphics
{
private:
	static std::list<MeshRenderer*> meshRenderers;

public:
	static std::shared_ptr<Camera> camera;
	static long renderedFrames;

	void static DrawMeshRenderer();
	void static RegisterMeshRenderer(MeshRenderer*);
	void static UnregeisterMeshRenderer(MeshRenderer*);
};

