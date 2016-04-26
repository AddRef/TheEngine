#pragma once
#include "math.hxx"

#include <set>

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

struct IRenderer
{

};

struct ISceneElement
{
   virtual void Update(uint64_t time) = 0;
   virtual void Render(IRenderer& renderer, const Camera& camera) = 0;
};

class SceneObject : public ISceneElement
{
public:
    SceneObject(const MeshData* mesh);
    virtual void Update(uint64_t time);
    virtual void Render(IRenderer& renderer, const Camera& camera);
private:
    const MeshData* m_mesh;
};

class Scene
{
public:
    Scene();
    ~Scene();
    void SetCamera(Camera* camera);
    void AddElement(ISceneElement& element);
    void RemoveElement(ISceneElement& element);

    void Update(uint64_t);
    void Render(IRenderer& renderer);
private:
    Camera* m_camera = nullptr;
    std::set<ISceneElement*> m_scene_elements;
};

};