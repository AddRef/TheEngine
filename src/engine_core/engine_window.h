#pragma once
#include "input_keys.h"

#include <string>
#include <set>
#include <memory>

struct SDL_Window;
typedef void* SDL_GLContext;

namespace The
{


class Window
{
public:
    struct ICallback
    {
        virtual void OnKeyDown(InputKey key) { key; }
        virtual void OnKeyUp(InputKey key) { key; }
        virtual void OnMouseDown(InputKey key, uint32_t x, uint32_t y) { key; x; y; }
        virtual void OnMouseUp(InputKey key, uint32_t  x, uint32_t  y) { key; x; y; }
        virtual void OnMouseMove(uint32_t x, uint32_t y) { x; y; }
        virtual void OnFingerDown(float x, float y, float dx, float dy) { x; y; dx; dy; }
        virtual void OnFingerUp(float x, float y, float dx, float dy) { x; y; dx; dy; }
        virtual void OnFingerMove(float x, float y, float dx, float dy) { x; y; dx; dy; }
        virtual void OnMultiGesture(uint16_t numfingers, float x, float y, float theta, float dist) { numfingers; x; y; theta; dist; }
    };

    struct Desc
    {
        std::string title       = "window";
        uint32_t    width       = 640;
        uint32_t    height      = 480;
        bool        fullscreen  = false;
    };

    Window() {}
    ~Window() {}
    bool Create(const Desc& desc);
    void Destroy();
    void SetDimensions(uint32_t width, uint32_t height);
    void SetFullscreen(bool fullscreen);
    void SetTitle(const std::string& title);
    const Desc& GetDescription() const { return m_desc; }
    void RegisterInputCallback(Window::ICallback& callbacks);
    void UnregisterInputCallback(Window::ICallback& callbacks);
    bool Process() const;
private:
    static InputKey convertSdlCodeToExternal(uint32_t sdl_key);

    Desc m_desc;
    bool m_opened = false;

    SDL_Window*     m_window;
    std::set<Window::ICallback*> m_input_callbacks;
};

};