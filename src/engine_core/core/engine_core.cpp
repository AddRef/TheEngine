#include "log.hxx"
#include "engine_core.h"

INITIALIZE_EASYLOGGINGPP

namespace The
{

////////////////////////////////////////////////////////////////////////////
// EngineCore
bool EngineCore::Init(Config config)
{
    m_config = config;
    m_window.Create(config.window_desc);
    return true;
}
}; //namespace The