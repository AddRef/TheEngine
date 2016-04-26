#include "sample.h"

#include "core/engine_core.h"
#include "resource_data/resource_data.h"

#include "log.hxx"
#include "math.h"

const float g_triangle_data[] = 
{
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     0.0f,  1.0f, 0.0f,
};

// @public
Sample::Sample()
{
}

Sample::~Sample()
{
}

bool Sample::Init()
{
    if (!loadResources()) return false;
    if (!initScene()) return false;
    if (!initEngine()) return false;
    return true;
}

void Sample::Run()
{

}

// @protected
// The::Window::ICallback
void Sample::OnKeyDown(InputKey key)
{
    key;
}

void Sample::OnKeyUp(InputKey key)
{
    key;
}

void Sample::OnMouseDown(InputKey key, uint32_t x, uint32_t y)
{
    key; x; y;
}

void Sample::OnMouseUp(InputKey key, uint32_t  x, uint32_t  y)
{
    key; x; y;
}

void Sample::OnMouseMove(uint32_t x, uint32_t y)
{
    x; y;
}

void Sample::OnFingerDown(float x, float y, float dx, float dy)
{
    x; y; dx; dy;
}

void Sample::OnFingerUp(float x, float y, float dx, float dy)
{
    x; y; dx; dy;
}

void Sample::OnFingerMove(float x, float y, float dx, float dy)
{
    x; y; dx; dy;
}

void Sample::OnMultiGesture(uint16_t numfingers, float x, float y, float theta, float dist)
{
    numfingers; x; y; theta; dist;
}

// The::Engine::ICallback
void Sample::OnUpdate()
{

}

// @private
bool Sample::loadResources()
{
    std::string mesh_path = "";
    std::string shader_path = "";
    // Fill mesh data
    The::MeshData::Shape mesh_shape;
    mesh_shape.Positions.Data.assign(g_triangle_data, g_triangle_data + sizeof(g_triangle_data));
    mesh_shape.LayoutType = The::MeshData::LayoutType::Triangle;
    m_mesh_data.AddShape(std::move(mesh_shape));
    // Create object
    m_scene_object = std::make_unique<The::SceneObject>(&m_mesh_data);
    // Load shader
    if (!m_shader_data.Load(shader_path))
    {
        THE_ERROR("Failed to load shader " << shader_path << std::endl, return false);
    }

    return true;
}

bool Sample::initScene()
{
    The::Camera::Desc camera_desc;

    camera_desc.eye_position = The::vector3f(8.0, 2.0, -8.0);
    camera_desc.direction = The::vector3f(0.0, 0.0, 7.0) - camera_desc.eye_position;
    camera_desc.up = The::vector3f(0.0, 1.0, 0.0);

    camera_desc.aspect = 1.0f * m_engine_core.GetWindow().GetDescription().width / m_engine_core.GetWindow().GetDescription().height;
    camera_desc.field_of_view_y = 45.0;
    camera_desc.near_z = 0.1f;
    camera_desc.far_z = 100.0f;

    The::Camera camera(camera_desc);
    m_scene.SetCamera(&camera);
    m_scene.AddElement(*m_scene_object);
    return true;
}

bool Sample::initEngine()
{
    The::Window::Desc window_desc;
    window_desc.fullscreen = false;
    window_desc.width = 640;
    window_desc.height = 480;
    window_desc.title = "simple_window";
    The::Window window;

    // m_engine_core.Create(window_desc);
    m_engine_core.SetScene(&m_scene);
    return true;
}