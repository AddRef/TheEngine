#include "log.hxx"
#include "engine_core.h"

INITIALIZE_EASYLOGGINGPP

namespace The
{

////////////////////////////////////////////////////////////////////////////
// EngineCore
bool EngineCore::Init(The::Window&& window)
{
    m_window = std::move(window);
    return true;
}
}; //namespace The