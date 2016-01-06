#include "engine_window.h"
#include <cstdio>

class WindowCallback : public Receiver<The::Window::IInputCallback>
{
protected:
    // The::IWindowCallback
    virtual void OnKeyDown(InputKey key) 
    {
        printf("Key %d is down\n", (uint32_t)key); 
    }
    virtual void OnKeyUp(InputKey key)
    {
        printf("Key %d is up\n", (uint32_t)key);
    }
    virtual void OnMouseDown(InputKey key, uint32_t x, uint32_t y)
    {
        printf("Mouse button %d is down, coordinates: %d, %d\n", (uint32_t)key, x, y);
    }
    virtual void OnMouseUp(InputKey key, uint32_t  x, uint32_t  y)
    {
        printf("Mouse button %d is up, coordinates: %d, %d\n", (uint32_t)key, x, y);
    }
    virtual void OnMouseMove(uint32_t x, uint32_t y)
    {
        printf("Mouse move, coordinates: %d, %d\n", x, y);
    }
    virtual void OnFingerDown(float x, float y, float dx, float dy)
    {
        printf("Finger is down, coordinates: %f, %f, delta: %f, %f\n", x, y, dx, dy);
    }
    virtual void OnFingerUp(float x, float y, float dx, float dy)
    {
        printf("Finger is up, coordinates: %f, %f, delta: %f, %f\n", x, y, dx, dy);
    }
    virtual void OnFingerMove(float x, float y, float dx, float dy)
    {
        printf("Finger move, coordinates: %f, %f, delta: %f, %f\n", x, y, dx, dy);
    }
    virtual void OnMultiGesture(uint16_t numfingers, float x, float y, float theta, float dist)
    {
        printf("Multigesture, fingers: %d, coordinates: %f, %f, theta: %f, distance: %f\n", numfingers, x, y, theta, dist);
    }
};

int main()
{
    The::Window window;
    The::Window::Desc window_desc;
    WindowCallback callback;
    window_desc.fullscreen = false;
    window_desc.width = 640;
    window_desc.height = 480;
    window_desc.title = "simple_window";
    window.Create(window_desc);
    window.AddCallback(callback);
    while (window.Process())
    {
    }
    return 0;
}