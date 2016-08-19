#pragma once
#include "input_keys.h"
#include "callbacks.hxx"
#include "misc.hxx"

#include <string>
#include <set>
#include <memory>

struct SDL_Window;
typedef void* SDL_GLContext;

namespace The
{


class Window
    : public Emitter
    , public NotCopyable
{
public:
    struct IInputCallback
    {
        virtual void OnKeyDown(InputKey) {}
        virtual void OnKeyUp(InputKey) {}
        virtual void OnMouseDown(InputKey, uint32_t, uint32_t) {}
        virtual void OnMouseUp(InputKey, uint32_t, uint32_t) {}
        virtual void OnMouseMove(uint32_t, uint32_t) {}
        virtual void OnFingerDown(float, float, float, float) {}
        virtual void OnFingerUp(float, float, float, float) {}
        virtual void OnFingerMove(float, float, float, float) {}
        virtual void OnMultiGesture(uint16_t, float, float, float, float) {}
    };

    struct Desc
    {
        std::string title       = "window";
        uint32_t    width       = 640;
        uint32_t    height      = 480;
        bool        fullscreen  = false;
    };

    Window();
    ~Window();
    Window& operator = (Window&& rvalue);
    bool Create(const Desc& desc);
    void Destroy();
    void SetDimensions(uint32_t width, uint32_t height);
    void SetFullscreen(bool fullscreen);
    void SetTitle(const std::string& title);
    const Desc& GetDescription() const { return m_desc; }
    bool Process() const;
private:
    static InputKey convertSdlCodeToExternal(uint32_t sdl_key);

    Desc m_desc;
    SDL_Window*     m_window;
    
    static bool s_single_instance_is_created; // Ensures there is only one window created
};

};