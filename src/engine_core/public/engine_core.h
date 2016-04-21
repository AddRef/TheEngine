#pragma once
#include "engine_window.h"
#include "math.hxx"

#include <memory>

namespace The
{

class MeshData;

class Camera
{
public:
    struct Desc
    {
        vector3f eye_position;
        vector3f direction;
        vector3f up;
        double   aspect;
        double   field_of_view_y;
        double   near_z;
        double   far_z;
    };
    Camera(const Desc& desc);
    ~Camera();
    const Desc& GetDesc() const;
    void Update(uint64_t time);
private:
    Desc m_desc;
    mat4 m_view;
    mat4 m_projection;
};

struct ISceneElement
{
   virtual void Update(uint64_t time) = 0;
};

class SceneObject : public ISceneElement
{
public:
    SceneObject(const MeshData* mesh);
    virtual void Update(uint64_t time);
private:
    const MeshData* m_mesh;
};

class Scene
{
public:
    Scene();
    ~Scene();
    void SetCamera(Camera* camera);
    void AddElement(ISceneElement* element);
    void RemoveElement(ISceneElement* element);

    void Update(uint64_t);
private:
    Camera* m_camera = nullptr;
    std::set<ISceneElement*> m_scene_elements;
};

class EngineCore
{
public:
    struct ICallback
    {
        virtual void OnUpdate() = 0;
    };
    EngineCore() {}
    ~EngineCore() {}
    bool Init(The::Window&& window);
    void SetScene(Scene* scene) { scene; }
    Window& GetWindow() { return m_window; }

private:
	ICallback* m_callback = nullptr;
    Window m_window;
};

};