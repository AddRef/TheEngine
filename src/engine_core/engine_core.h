#pragma once
#include <memory>

namespace The
{

class SceneGraph
{
    typedef void* Node;
    const static Node Root;
    Node AddObject(Node target_node, ISceneObject* child);
};

struct ISceneObject
{

}; 

class Scene
{
public:
    Scene();

};

class Engine
{
public:
    struct ICallback
    {
        virtual void OnUpdate() = 0;
    };
	Engine();
    ~Engine();
    bool Create(const The::Window::Desc& window_desc);
    void SetCallback(ICallback* callback);
    void SetScene(Scene* scene);
    Window& GetWindow();

private:
	ICallback* m_callback = nullptr;
};

};