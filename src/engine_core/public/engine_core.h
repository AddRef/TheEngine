#pragma once
#include "engine_window.h"

#include <memory>

namespace The
{

struct ISceneObject
{

}; 

class Scene
{
public:
    Scene() {}
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
    bool Create(const The::Window::Desc& window_desc);
    void SetScene(Scene* scene) { scene; }
    Window& GetWindow() { return m_window; }

private:
	ICallback* m_callback = nullptr;
    Window  m_window;
};

};