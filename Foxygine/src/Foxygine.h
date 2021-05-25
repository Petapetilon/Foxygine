#pragma once
#include <iostream>
#include <math.h>
#include <string>
#include <memory>




#include "GameObject/GameObject.h"
#include "GameObject/GameObjectHandler.h"
#include "GameObject/Component.h"
#include "GameObject/Transform.h"


#include "Graphics/Mesh/MeshRenderer.h"

#include "Graphics/Rendering/Material.h"
#include "Graphics/Rendering/Camera.h"

#include "Graphics/Shaders/Shader.h"
#include "Graphics/Shaders/ShaderLibrary.h"

#include "Graphics/Textures/Texture2D.h"

#include "Graphics/Lights/DirectionalLight.h"

#include "Graphics/UI/ScreenSpaceCanvas.h"
#include "Graphics/UI/BoundingRect.h"
#include "Graphics/UI/UIImage.h"


#include "Math/Constants.h"
#include "Math/Quaternion.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Vector2I.h"
#include "Math/Vector3I.h"
#include "Math/Vector4I.h"


#include "Scene/Scene.h"


#include "Peripherals/Window.h"
#include "Peripherals/Keyboard.h"
#include "Peripherals/Mouse.h"
#include "Peripherals/GamePad.h"

#define FOXYGINE_DEBUG
#define USE_NAME_LOOKUP
//#define USE_CACHE


class Foxygine
{
public:
	float r, g, b;

	Foxygine();
	~Foxygine();

	void StartFoxygine();
	void UpdateFoxygine(float deltaTime);
	void FixedUpdateFoxygine(float deltaTime);
	void EndFoxygine();


private:

};
