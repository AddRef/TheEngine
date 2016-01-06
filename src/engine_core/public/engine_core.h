#pragma once
#include "engine_window.h"

#include <memory>

namespace The
{

struct ISceneObject
{

}; 

class SceneGraph
{
    typedef void* Node;
    const static Node Root;
    Node AddObject(Node target_node, ISceneObject* child);
};

class Scene
{
public:
    Scene();

};

class EngineCore
{
public:
    struct ICallback
    {
        virtual void OnUpdate() = 0;
    };
    EngineCore();
    ~EngineCore();
    bool Create(const The::Window::Desc& window_desc);
    void SetScene(Scene* scene);
    Window& GetWindow();

private:
	ICallback* m_callback = nullptr;
};

};