#include "engine_window.h"

#include "SDL.h"
#include "SDL_opengles2.h"
#include "SDL_egl.h"

namespace The
{
    
Window::Window()
{
}

void Window::Open(const Desc& desc)
{
    m_desc = desc;
    SDL_Init(SDL_INIT_VIDEO);
#ifdef WIN32
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif //WIN32
    m_window = SDL_CreateWindow(builder->GetTitle().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                               builder->GetWidth(), builder->GetHeight(), SDL_WINDOW_OPENGL);
    VE_ERROR_IF(!m_window, L"Window creation failed: %S", SDL_GetError());
    m_glcontext = SDL_GL_CreateContext(m_window);
    VE_ERROR_IF(!m_glcontext, L"GL context creation failed: %S", SDL_GetError());

    SetFullscreen(builder->IsFullscreen());
}

void Window::Close()
{
    SDL_GL_DeleteContext(m_glcontext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Window::SetDimensions(uint32_t width, uint32_t height)
{
    m_desc.width = width;
    m_desc.height = height;
    // TODO: update dimensions
}

void Window::SetFullscreen(bool fullscreen)
{
    m_desc.fullscreen = fullscreen;
    // TODO: update fullscreen
}

}