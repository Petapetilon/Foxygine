#pragma once
#include "Material.h"
class PBRMaterial : public Material
{
public:
	PBRMaterial();
	PBRMaterial(std::string name);


	void SetRoughness(float value);
	void SetMetallic(float value);

	void LoadRoughnessMap(std::string filePath);
	void SetRoughnessMap(std::shared_ptr<Texture2D> texture);
	void SetRoughnessMap(std::string textureName);

	void LoadMetallicMap(std::string filePath);
	void SetMetallicMap(std::shared_ptr<Texture2D> texture);
	void SetMetallicMap(std::string textureName);

	void LoadAOMap(std::string filePath);
	void SetAOMap(std::shared_ptr<Texture2D> texture);
	void SetAOMap(std::string textureName);

	
};

