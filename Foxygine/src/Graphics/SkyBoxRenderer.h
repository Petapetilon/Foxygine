#pragma once
#include <string>
#include "MeshRenderer.h"


class SkyBoxRenderer : public MeshRenderer
{
public:
	void SetSkybox(std::vector<std::string> filePaths);
	void Draw(std::shared_ptr<Camera> drawingCam) override;
};

