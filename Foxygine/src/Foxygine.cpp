#include "Foxygine.h"


Foxygine::Foxygine() {
    r = 1;
    g = 0;
    b = 0;
}
Foxygine::~Foxygine() {}


void Foxygine::StartFoxygine()
{
    //Camera
    auto cameraGo = new GameObject("cameraGO");
    cameraGo->AddComponent<Camera>(new Camera());
    auto cam = cameraGo->GetComponent<Camera>();
    cameraGo->transform->SetPosition(Vector3(0, -3, -10));
    cam->transform->Rotate(Vector3(1, 0, 0), 20.f);
    cam->SetupCamera(glm::radians(60.f), 192.f / 108.f, .001f, 1000.f);
    
    
    Shader::CreateBasicLitShader("shader");


    auto basicMaterial2 = new Material("basicMaterial2", "shader");
    basicMaterial2->CreateMaterialProperty("Glossiness", "glossiness", .75);
    basicMaterial2->CreateMaterialProperty("Metallic", "metallic", .75f);

    auto basicMaterial3 = new Material("basicMaterial3", "shader");
    basicMaterial3->SetMainColor(Color(1.f, 1.f, 1.f, 1.f));
    basicMaterial3->CreateMaterialProperty("Glossiness", "glossiness", .5f);
    basicMaterial3->CreateMaterialProperty("Metallic", "metallic", 0);

    auto colorTex = std::shared_ptr<Texture>(new Texture());
    colorTex->LoadTexture("res\\textures\\rockCol.jpg", "brickCol", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
    basicMaterial3->CreateTextureProperty("color", std::shared_ptr<Texture>(colorTex), Material::TextureSlot::BaseColor);
    auto colorNrm = std::shared_ptr<Texture>(new Texture());
    colorNrm->LoadTexture("res\\textures\\rockNrm.jpg", "brickNrm", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
    basicMaterial3->CreateTextureProperty("normal", std::shared_ptr<Texture>(colorNrm), Material::TextureSlot::NormalMap);


    
    auto smoothSphere2 = new GameObject("smoothSphere");
    smoothSphere2->AddComponent<MeshRenderer>(new MeshRenderer("res\\meshes\\IcoS.obj"));
    smoothSphere2->GetComponent<MeshRenderer>()->SetMaterial(std::shared_ptr<Material>(basicMaterial2));
    smoothSphere2->transform->SetPosition(Vector3(-5, 0, 0));
    
    auto cube = new GameObject("cube");
    cube->AddComponent<MeshRenderer>(new MeshRenderer("res\\meshes\\cube.obj"));
    cube->GetComponent<MeshRenderer>()->SetMaterial(std::shared_ptr<Material>(basicMaterial3));
    cube->transform->SetPosition(Vector3(1, 0, 0));
}


void Foxygine::UpdateFoxygine(float deltaTime)
{
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

    
    GameObjectHandler::GetGameObject("smoothSphere")->GetComponent<MeshRenderer>()->GetMaterial()->SetMainColor(Color(r, g, b, 1));
    GameObjectHandler::GetGameObject("smoothSphere")->transform->Rotate(Vector3(0, 1, 0), glm::degrees(deltaTime));
    GameObjectHandler::GetGameObject("cube")->transform->Rotate(Vector3(r, g, b), glm::degrees(deltaTime));
}


void Foxygine::FixedUpdateFoxygine(float deltaTime) {}


void Foxygine::EndFoxygine() {}
