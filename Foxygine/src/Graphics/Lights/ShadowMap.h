#pragma once
#include <vector>
#include <memory>


class Texture2D;
class Shader;
class Light;

class ShadowMap
{
private:
	static std::vector<Texture2D*> shadowMaps;
	static unsigned int shadowDepthMapResolution;
	static long currentlyRenderedFrame;
	static short alreadyRenderedShadowMaps;
	static std::shared_ptr<Shader> shadowMapShader;

	static ShadowMap sm;

public:

	unsigned int GL_ShadowMapFrameBufferObject;
	unsigned int GL_ShadowDepthMap;
	unsigned int GL_Colrmap;
	static const short shadowMapCount = 4;
	static void InitShadowMap();
	static void FreeShadowMap();
	static void SetShadowMapResolution(unsigned int resolution);
	static void RenderShadowMap(Light*);
	static std::shared_ptr<Shader> GetShadowMapShader();
	static void GL_BindShadowMap();
};

