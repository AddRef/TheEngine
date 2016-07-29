#pragma once
#include "engine_window.h"
#include "math.hxx"
#include "scene.h"

#include <memory>

namespace The
{

class EngineCore
{
public:
    struct ICallback
    {
        virtual void OnUpdate() = 0;
    };
    enum class Api
    {
        OGL
    };
    struct Config
    {
        Window::Desc window_desc;
        Api api;
    };

    EngineCore() {}
    ~EngineCore() {}
    bool Init(Config config);
    void SetScene(Scene* scene) { scene; }
    Window& GetWindow() { return m_window; }

private:
	ICallback* m_callback = nullptr;
    Window m_window;
};

};