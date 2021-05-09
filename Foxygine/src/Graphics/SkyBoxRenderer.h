#pragma once
#include <string>
#include "MeshRenderer.h"


class SkyboxRenderer : public MeshRenderer
{
public:
	void SetSkybox(std::vector<std::string> filePaths);
	void Draw(std::shared_ptr<Camera> drawingCam) override;
};

