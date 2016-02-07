#include "log.hxx"
#include "engine_core.h"

INITIALIZE_EASYLOGGINGPP

namespace The
{
bool EngineCore::Create(const The::Window::Desc& window_desc)
{
    bool result = m_window.Create(window_desc);
    THE_ERROR_IF(!result, "Failed to create window", return false);
    return true;
}
}; //namespace The