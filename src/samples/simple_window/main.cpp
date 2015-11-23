#include "engine_window.h"

int main()
{
    The::Window window;
    The::Window::Desc window_desc;
    window_desc.fullscreen = false;
    window_desc.width = 640;
    window_desc.height = 480;
    window_desc.title = "simple_window";
    window.Open(window_desc);

    while (window.Process())
    {

    }

    return 0;
}