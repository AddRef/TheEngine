#pragma once
#include "resource_loader.h"

class Sample 
    : public The::Window::ICallback
    , public The::Engine::ICallback
{
public:
    Sample();
    ~Sample();

    bool Init();
    void Run();

protected:
    // The::Window::ICallback
    virtual void OnKeyDown(InputKey key) override;
    virtual void OnKeyUp(InputKey key) override;
    virtual void OnMouseDown(InputKey key, uint32_t x, uint32_t y) override;
    virtual void OnMouseUp(InputKey key, uint32_t  x, uint32_t  y) override;
    virtual void OnMouseMove(uint32_t x, uint32_t y) override;
    virtual void OnFingerDown(float x, float y, float dx, float dy) override;
    virtual void OnFingerUp(float x, float y, float dx, float dy) override;
    virtual void OnFingerMove(float x, float y, float dx, float dy) override;
    virtual void OnMultiGesture(uint16_t numfingers, float x, float y, float theta, float dist) override;

    // The::Engine::ICallback
    virtual void OnUpdate() override;
private:
    bool loadResources();
    bool initScene();
    bool initEngine();

    The::Texture m_texture;
    The::Mesh m_mesh;
    The::Shader m_shader;

    The::EngineCore m_engine_core;
    The::Scene m_scene;
};