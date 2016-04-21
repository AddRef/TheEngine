#include "log.hxx"
#include "engine_core.h"

INITIALIZE_EASYLOGGINGPP

namespace The
{
////////////////////////////////////////////////////////////////////////////
// Camera
Camera::Camera(const Desc& desc)
    : m_desc(desc)
{
    m_desc.direction = glm::normalize(m_desc.direction);
    m_view = glm::lookAt(m_desc.eye_position, m_desc.eye_position + m_desc.direction, m_desc.up);
    m_projection = glm::perspective(m_desc.field_of_view_y, m_desc.aspect, m_desc.near_z, m_desc.far_z);
}

Camera::~Camera()
{}

void Camera::Update(uint64_t time)
{
    time;
}

const Camera::Desc& Camera::GetDesc() const
{
    return m_desc;
}

////////////////////////////////////////////////////////////////////////////
// SceneObject
SceneObject::SceneObject(const MeshData* mesh)
    : m_mesh(mesh) {}

void SceneObject::Update(uint64_t time)
{
    time;
}


////////////////////////////////////////////////////////////////////////////
// Scene
Scene::Scene()
{}

Scene::~Scene()
{}

void Scene::SetCamera(Camera* camera)
{
    m_camera = camera;
}

void Scene::AddElement(ISceneElement* element)
{
    if (element)
        m_scene_elements.insert(element);
}

void Scene::RemoveElement(ISceneElement* element)
{
    if (!m_scene_elements.empty())
    {
        m_scene_elements.erase(element);
    }
}

void Scene::Update(uint64_t time)
{
    time;
}

////////////////////////////////////////////////////////////////////////////
// EngineCore
bool EngineCore::Init(The::Window&& window)
{
    m_window = std::move(window);
    return true;
}
}; //namespace The