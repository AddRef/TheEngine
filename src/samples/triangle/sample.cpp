#include "sample.h"

#include "engine_core.h"
#include "log.h"
#include "math.h"

// @public
Sample()
{
}

~Sample()
{
}

bool Init()
{
    if (!loadResources()) return false;
    if (!initScene()) return false;
    if (!initEngine()) return false;
}

void Run()
{

}

// @protected
// The::Window::ICallback
void Sample::OnKeyDown(InputKey key)
{

}

void Sample::OnKeyUp(InputKey key)
{

}

void Sample::OnMouseDown(InputKey key, uint32_t x, uint32_t y)
{

}

void Sample::OnMouseUp(InputKey key, uint32_t  x, uint32_t  y)
{

}

void Sample::OnMouseMove(uint32_t x, uint32_t y)
{

}

void Sample::OnFingerDown(float x, float y, float dx, float dy)
{

}

void Sample::OnFingerUp(float x, float y, float dx, float dy)
{

}

void Sample::OnFingerMove(float x, float y, float dx, float dy)
{

}

void Sample::OnMultiGesture(uint16_t numfingers, float x, float y, float theta, float dist)
{

}

// The::Engine::ICallback
void Sample::OnUpdate()
{

}

// @private
bool Sample::loadResources()
{
    std::string texture_path = "";
    std::string mesh_path = "";
    std::string shader_path = "";
    The::TextureData texture_data;
    The::MeshData mesh_data;
    The::ShaderData shader_data;
    if (!texture_data.Load(texture_path))
    {
        THE_ERROR("Failed to load texture " << texture_path << std::endl, return false);
    }
    if (!mesh_data.Load(mesh_path))
    {
        THE_ERROR("Failed to load mesh " << mesh_path << std::endl, return false);
    }
    if (!shader_data.Load(shader_path))
    {
        THE_ERROR("Failed to load shader " << shader_path << std::endl, return false);
    }

    m_texture.Create(texture_data);
    m_mesh.Create(mesh_data);
    m_shader.Create(shader_data);
}

bool Sample::initScene()
{
    // The::EngineCore engine_core;
    // The::Object object(mesh, texture, shader);
    // The::Camera::Desc camera_desc;
    // cameraSetup.eye_position = vector3f(8.0, 2.0, -8.0);
    // cameraSetup.direction = vector3f(0.0, 0.0, 7.0) - camera_desc.eye_position;
    // cameraSetup.up = vector3f(0.0, 1.0, 0.0);

    // cameraSetup.aspect = 1.0f * m_window.GetWidth() / m_window.GetHeight();
    // cameraSetup.field_of_view_y = 45.0;
    // cameraSetup.near_z = 0.1f;
    // cameraSetup.far_z = 100.0f;

    // The::Camera camera(vector3f());
    // The::Light light;
    // The::MoveObject move_object(vector3f(0.0f, 0.0f, 0.0f);
    // The::MoveObject move_camera();
    // The::MoveObject move_light;

    // The::Scene scene;
    // The:Scene::Node* move_object_node = scene.Add(scene.Root(), move_object);
    // scene.Add(move_object_node, object);
    
    // The::Scene::Node* move_camera_node = scene.Add(scene.Root(), move_camera);
    // scene.Add(move_camera_node, camera);

    // The::Scene::Node* move_light_node = scene.Add(scene.Root(), move_light);
    // scene.Add(move_light_node, light);

}

bool Sample::initEngine()
{
    The::Window::Desc window_desc;
    WindowCallback callback;
    window_desc.fullscreen = false;
    window_desc.width = 640;
    window_desc.height = 480;
    window_desc.title = "simple_window";

    m_engine_core.Create(window_desc);
    engine_core.SetScene(&m_scene);
}