#include "Foxygine.h"
#include "GameObject/SimpleCamera.h"
#include "Graphics/Graphics.h"
#include <vector>


Foxygine::Foxygine() {
    r = 1;
    g = 0;
    b = 0;
}
Foxygine::~Foxygine() {}


void Foxygine::StartFoxygine()
{
    std::vector<std::string> skyboxTextures;
    skyboxTextures.push_back("res\\textures\\Skybox\\blue skybox_left.png");
    skyboxTextures.push_back("res\\textures\\Skybox\\blue skybox_right.png");
    skyboxTextures.push_back("res\\textures\\Skybox\\blue skybox_down.png");
    skyboxTextures.push_back("res\\textures\\Skybox\\blue skybox_up.png");
    skyboxTextures.push_back("res\\textures\\Skybox\\blue skybox_front.png");
    skyboxTextures.push_back("res\\textures\\Skybox\\blue skybox_back.png");
    Graphics::SetSkybox(skyboxTextures);

    //Camera
    auto cameraGo = GameObject::CreateGameObject("cameraGO");
    cameraGo->AddComponent<Camera>(new Camera());
    cameraGo->transform->SetPosition(Vector3(0, -1, -10));
    cameraGo->transform->Rotate(Vector3(1, 0, 0), 20.f);
    auto resolution = Window::GetInstance()->GetWindowResolution();
    auto cam = cameraGo->GetComponent<Camera>();
    cam->SetupCamera(glm::radians(60.f), (float)resolution.x / (float)resolution.y, .001f, 1000.f);
    cameraGo->AddComponent<SimpleCamera>(new SimpleCamera());

    
        
    

    Shader::CreateBasicLitShader("shader");
    auto standardShader = ShaderLibrary::GetShader("shader");
    Vector4 direction(1, -2, .5f, 1);
    standardShader->SetShaderPass(new ShaderPassVec4(&direction, "u_DirLightDirection"));
    Color color(1, .95f, .8f, 1);
    standardShader->SetShaderPass(new ShaderPassColor(&color, "u_DirLightColor"));

    auto basicMaterial2 = new Material("basicMaterial2", "shader");
    basicMaterial2->CreateMaterialProperty("Glossiness", "glossiness", .75);
    basicMaterial2->CreateMaterialProperty("Metallic", "metallic", .75f);

    auto basicMaterial3 = new Material("basicMaterial3", "shader");
    basicMaterial3->SetMainColor(Color(1.f, 1.f, 1.f, 1.f));
    basicMaterial3->CreateMaterialProperty("Glossiness", "glossiness", .5f);
    basicMaterial3->CreateMaterialProperty("Metallic", "metallic", 0);


    auto colorTex = std::shared_ptr<Texture2D>(new Texture2D());
    colorTex->LoadTexture2DOptimized("res\\textures\\Bricks01_col.jpg", "brickCol", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
    basicMaterial3->CreateTextureProperty("color", std::shared_ptr<Texture>(colorTex), Material::TextureSlot::BaseColor);

    auto colorNrm = std::shared_ptr<Texture2D>(new Texture2D());
    colorNrm->LoadTexture2DOptimized("res\\textures\\Bricks01_nrm.jpg", "brickNrm", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
    basicMaterial3->CreateTextureProperty("normal", std::shared_ptr<Texture>(colorNrm), Material::TextureSlot::NormalMap);

    auto colorSpec = std::shared_ptr<Texture2D>(new Texture2D());
    colorSpec->LoadTexture2DOptimized("res\\textures\\Bricks01_rgh.jpg", "brickSpec", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
    basicMaterial3->CreateTextureProperty("specular", std::shared_ptr<Texture>(colorSpec), Material::TextureSlot::Specular);



    
    auto smoothSphere2 = GameObject::CreateGameObject("smoothSphere");
    smoothSphere2->AddComponent<MeshRenderer>(new MeshRenderer("res\\meshes\\IcoS.obj"));
    smoothSphere2->GetComponent<MeshRenderer>()->SetMaterial(std::shared_ptr<Material>(basicMaterial2));
    smoothSphere2->transform->SetPosition(Vector3(-5, 0, 0));
    
    auto cube = GameObject::CreateGameObject("cube");
    cube->AddComponent<MeshRenderer>(new MeshRenderer("res\\meshes\\cube.obj"));
    cube->GetComponent<MeshRenderer>()->SetMaterial(std::shared_ptr<Material>(basicMaterial3));
}


void Foxygine::UpdateFoxygine(float deltaTime)
{
    if (deltaTime < .1f) {
        if (r > 0 && g < 1 && b == 0) {
            r -= deltaTime * .25f;
            g += deltaTime * .25f;
        }

        if (r == 0 && g > 0 && b < 1) {
            g -= deltaTime * .25f;
            b += deltaTime * .25f;
        }

        if (r < 1 && g == 0 && b > 0) {
            b -= deltaTime * .25f;
            r += deltaTime * .25f;
        }

        if (r < 0) r = 0;
        if (g < 0) g = 0;
        if (b < 0) b = 0;

        if (r > 1) r = 1;
        if (g > 1)  g = 1;
        if (b > 1) b = 1;
    }

    
    GameObjectHandler::FindGameObject("smoothSphere")->GetComponent<MeshRenderer>()->GetMaterial()->SetMainColor(Color(r, g, b, 1));
    GameObjectHandler::FindGameObject("smoothSphere")->transform->Rotate(Vector3(0, 1, 0), glm::degrees(deltaTime));
    GameObjectHandler::FindGameObject("cube")->transform->Rotate(Vector3(r, g, b), glm::degrees(deltaTime));
}


void Foxygine::FixedUpdateFoxygine(float deltaTime) {}


void Foxygine::EndFoxygine() {}
