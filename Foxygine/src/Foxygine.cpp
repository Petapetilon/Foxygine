#include "Foxygine.h"
#include "Foxygine Include.h"
#include "GameObject/SimpleCamera.h"
#include "Graphics/Graphics.h"
#include <vector>
#include "Graphics/UI/BoundingRect.h"


Foxygine::Foxygine() {
    r = 1;
    g = 0;
    b = 0;
}
Foxygine::~Foxygine() {}


void Foxygine::StartFoxygine()
{
    Graphics::SetSkybox("res\\textures\\Skybox", "blue skybox", "png");
    //Graphics::SetEnvironment("res\\textures\\Environment\\misty_pines_4k.png");
   
    //Camera
    player = GameObject::CreateGameObject("Player");
    player->transform->SetPosition(Vector3(0, -1, -10));
    player->transform->Rotate(Vector3(1, 0, 0), 20.f);
    auto resolution = Window::GetInstance()->GetWindowResolution();

    player->AddComponent<Camera>(new Camera());
    auto cam = player->GetComponent<Camera>();
    cam->SetupCamera(glm::radians(70.f), (float)resolution.x / (float)resolution.y, .001f, 1000.f);
    player->AddComponent<SimpleCamera>(new SimpleCamera());

    auto lightGo = GameObject::CreateGameObject("lightGo");
    lightGo->AddComponent<DirectionalLight>(new DirectionalLight(Color(1, .9f, .7f, 1.f), 1));
    lightGo->transform->SetRotation(0, 100, 20, true);
    lightGo->transform->Translate(lightGo->transform->Forward() * -10);

    auto pointLightGo = GameObject::CreateGameObject("pointLightGo");
    pointLightGo->AddComponent<PointLight>(new PointLight(Color(.2f, .7f, .6f, 1), 1.f));
    pointLightGo->transform->Translate(10, 0, 0);




    
    auto basicMaterial3 = new PBRMaterial();
    basicMaterial3->mainColor = Color(1.f, 1.f, 1.f, 1.f);
    //basicMaterial3->LoadAlbedoTexture("res\\textures\\Rocks01_col.jpg");
    //basicMaterial3->LoadNormalMap("res\\textures\\Rocks01_nrm.jpg");
    basicMaterial3->mainColor = Color(1, 0, 1, 1.f);
    basicMaterial3->SetRoughness(.1f);
    basicMaterial3->SetMetallic(.9f);

    auto groundMaterial = new PBRMaterial();
    groundMaterial->mainColor = Color(1, 1, 1, 1.f);
    groundMaterial->uvScale = Vector2(100, 100);
    groundMaterial->normalMappingStrength = .4f;
    groundMaterial->SetRoughness(.75f);
    groundMaterial->SetMetallic(.05f);
    groundMaterial->LoadAlbedoTexture("res\\textures\\4K-ground_cracked-diffuse.jpg");
    groundMaterial->LoadNormalMap("res\\textures\\4K-ground_cracked-normal.jpg");






    auto groundGo = GameObject::CreateGameObject("ground");
    groundGo->AddComponent<MeshRenderer>(new MeshRenderer("res\\meshes\\Plane.obj", true));
    groundGo->GetComponent<MeshRenderer>()->SetMaterial(std::shared_ptr<Material>(groundMaterial));
    groundGo->transform->Scale(Vector3(1000, 1, 1000));
    groundGo->transform->Translate(Vector3(0, -2.5f, 0));
  
    auto smoothSphere2 = GameObject::CreateGameObject("sphere");
    smoothSphere2->AddComponent<MeshRenderer>(new MeshRenderer("res\\meshes\\IcoS.obj", true));
    smoothSphere2->GetComponent<MeshRenderer>()->SetMaterial(std::shared_ptr<Material>(basicMaterial3));
    smoothSphere2->transform->SetPosition(Vector3(-5, 0, 0));
 
    auto cube = GameObject::CreateGameObject("cube");
    cube->AddComponent<MeshRenderer>(new MeshRenderer("res\\meshes\\cube.obj", true));
    cube->GetComponent<MeshRenderer>()->SetMaterial(std::shared_ptr<Material>(basicMaterial3));

    auto empty = GameObject::CreateGameObject("empty");
    
    bullet = smoothSphere2;
    bullet->AddComponent<KinematikBody>(new KinematikBody());



    //auto canvasGo = GameObject::CreateGameObject("canvasGo");
    //canvasGo->AddComponent<ScreenSpaceCanvas>(new ScreenSpaceCanvas());
    //auto canvas = canvasGo->GetComponent<ScreenSpaceCanvas>();
    //auto img = new UIImage("image", canvas, colorTex);
    //canvas->AddElement(img);
    //canvas->SetPosition(Vector2I(0, 0));
    //
    //img->SetPosition(Vector2I(250, 250));
    //img->SetSizePixelAbsolute(Vector2I(250, 250));
    //img->SetRotation(90);
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

    auto cubeGo = GameObject::FindGameObject("cube");
    cubeGo->transform->Rotate(Vector3(r, g, b), glm::degrees(deltaTime));
    cubeGo->GetComponent<MeshRenderer>()->GetMaterial()->SetMaterialProperty("roughness", b);
    cubeGo->GetComponent<MeshRenderer>()->GetMaterial()->SetMaterialProperty("metallic", r);
    GameObject::FindGameObject("pointLightGo")->transform->SetPosition(Vector3(sinf(Graphics::renderedFrames * .005f) * 10, 1, cosf(Graphics::renderedFrames * .005f) * 10));
}


void Foxygine::FixedUpdateFoxygine(float deltaTime) {
    if (Keyboard::GetKey(KeyCode::L_Ctrl)) {
        //auto player = GameObject::FindGameObject("Player");
        auto instance = GameObject::CreateLinkedInstance(bullet);
        instance->transform->SetPosition(player->transform->Position() * -1);
        //instance->AddComponent<KinematikBody>(new KinematikBody());
        instance->GetComponent<KinematikBody>()->SetKinematikVelocity(player->transform->Forward() * -100, 200, true);
    }
}


void Foxygine::EndFoxygine() {}
