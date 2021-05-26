#pragma once
#include <string>
#include "MeshRenderer.h"
#include <vector>


class SkyboxRenderer : public MeshRenderer
{
public:
	void SetSkybox(std::vector<std::string> filePaths);
	void SetEnvironment(std::string filePath);
	void Draw(std::shared_ptr<Camera> drawingCam) override;
};

