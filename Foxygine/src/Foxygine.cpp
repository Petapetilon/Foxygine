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

    Shader::CreateBasicLitShader("basicShader");

    //Camera
    auto cameraGo = GameObject::CreateGameObject("cameraGO");
    cameraGo->transform->SetPosition(Vector3(0, -1, -10));
    cameraGo->transform->Rotate(Vector3(1, 0, 0), 20.f);
    auto resolution = Window::GetInstance()->GetWindowResolution();

    cameraGo->AddComponent<Camera>(new Camera());
    auto cam = cameraGo->GetComponent<Camera>();
    cam->SetupCamera(glm::radians(70.f), (float)resolution.x / (float)resolution.y, .001f, 1000.f);
    cameraGo->AddComponent<SimpleCamera>(new SimpleCamera());



    auto lightGo = GameObject::CreateGameObject("lightGo");
    lightGo->AddComponent<DirectionalLight>(new DirectionalLight(Color(1, .9f, .7f, 1.f), 1));
    //lightGo->transform->SetPosition(Vector3(-8, 5, 5));
    lightGo->transform->SetRotation(0, 100, 20, true);
    lightGo->transform->Translate(lightGo->transform->Forward() * -10);
    //lightGo->transform->SetPosition(0, 10, 0);
    //lightGo->transform->SetRotation(0, 0, 45, false);

    
    //lightGo->AddComponent<Camera>(new Camera());
    //auto cam = lightGo->GetComponent<Camera>();
    //cam->SetupCamera(glm::radians(70.f), (float)resolution.x / (float)resolution.y, .001f, 1000.f);
    //lightGo->AddComponent<SimpleCamera>(new SimpleCamera());




    auto basicMaterial2 = new Material("basicMaterial2", "basicShader");
    basicMaterial2->CreateMaterialProperty("Glossiness", "glossiness", .75);
    basicMaterial2->CreateMaterialProperty("Metallic", "metallic", .75f);

    //lightGo->AddComponent<MeshRenderer>(new MeshRenderer("res\\meshes\\IcoS.obj", true));
    //lightGo->GetComponent<MeshRenderer>()->SetMaterial(std::shared_ptr<Material>(basicMaterial2));
    
    auto basicMaterial3 = new Material("basicMaterial3", "basicShader");
    basicMaterial3->SetMainColor(Color(1.f, 1.f, 1.f, 1.f));
    basicMaterial3->CreateMaterialProperty("Glossiness", "glossiness", .5f);
    basicMaterial3->CreateMaterialProperty("Metallic", "metallic", 0);

    auto groundMaterial = new Material("groundMaterial", "basicShader");
    groundMaterial->SetMainColor(Color(1, 1, 1, 1.f));
    groundMaterial->CreateMaterialProperty("Glossiness", "glossiness", .05f);
    groundMaterial->CreateMaterialProperty("Metallic", "metallic", 0);


    auto colorTex = std::shared_ptr<Texture2D>(new Texture2D());
    colorTex->LoadTexture2D("res\\textures\\rockCol.jpg", "brickCol", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
    basicMaterial3->CreateTextureProperty("color", std::shared_ptr<Texture>(colorTex), Material::TextureSlot::BaseColor);

    auto colorNrm = std::shared_ptr<Texture2D>(new Texture2D());
    colorNrm->LoadTexture2D("res\\textures\\rockNrm.jpg", "brickNrm", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
    basicMaterial3->CreateTextureProperty("normal", std::shared_ptr<Texture>(colorNrm), Material::TextureSlot::NormalMap);


    auto groundTex = std::shared_ptr<Texture2D>(new Texture2D());
    groundTex->LoadTexture2D("res\\textures\\4K-ground_cracked-diffuse.jpg", "groundCol", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
    groundMaterial->CreateTextureProperty("color", std::shared_ptr<Texture>(groundTex), Material::TextureSlot::BaseColor);

    auto groundNrm = std::shared_ptr<Texture2D>(new Texture2D());
    groundNrm->LoadTexture2D("res\\textures\\4K-ground_cracked-normal.jpg", "groundNrm", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
    groundMaterial->CreateTextureProperty("normal", std::shared_ptr<Texture>(groundNrm), Material::TextureSlot::NormalMap);

    groundMaterial->SetNormalMappingStrength(.25f);



    auto groundGo = GameObject::CreateGameObject("ground");
    groundGo->AddComponent<MeshRenderer>(new MeshRenderer("res\\meshes\\Plane.obj", true));
    groundGo->GetComponent<MeshRenderer>()->SetMaterial(std::shared_ptr<Material>(groundMaterial));
    groundGo->transform->Scale(Vector3(10, 1, 10));
    groundGo->transform->Translate(Vector3(0, -2.5f, 0));


    
    auto smoothSphere2 = GameObject::CreateGameObject("smoothSphere");
    smoothSphere2->AddComponent<MeshRenderer>(new MeshRenderer("res\\meshes\\IcoS.obj", true));
    smoothSphere2->GetComponent<MeshRenderer>()->SetMaterial(std::shared_ptr<Material>(basicMaterial2));
    smoothSphere2->transform->SetPosition(Vector3(-5, 0, 0));


    
    auto cube = GameObject::CreateGameObject("cube");
    cube->AddComponent<MeshRenderer>(new MeshRenderer("res\\meshes\\cube.obj", true));
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
    //GameObjectHandler::FindGameObject("light")->transform->Rotate(Vector3(0, 1, 0), glm::degrees(deltaTime));
    GameObjectHandler::FindGameObject("cube")->transform->Rotate(Vector3(r, g, b), glm::degrees(deltaTime));
}


void Foxygine::FixedUpdateFoxygine(float deltaTime) {}


void Foxygine::EndFoxygine() {}
