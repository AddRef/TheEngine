#pragma once
#include <string>

namespace The
{

class Window
{
public:
    struct Desc
    {
        std::string title;
        uint32_t    width;
        uint32_t    height;
        uint32_t    fullscreen;
    };

    Window();
    void Open(const Desc& desc);
    void Close();
    void SetDimensions(uint32_t width, uint32_t height);
    void SetFullscreen(bool fullscreen);
    const Desc& GetDescription() const { return m_desc; }
private:
    WindowDescription m_desc;
    bool m_opened = false;

    SDL_Window*     m_window;
    SDL_GLContext*  m_glcontext;

};

};