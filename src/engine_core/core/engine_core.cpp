#include "log.hxx"
#include "engine_core.h"

INITIALIZE_EASYLOGGINGPP

namespace The
{

////////////////////////////////////////////////////////////////////////////
// EngineCore
bool EngineCore::Init(Api api)
{
    api;
    m_window = std::move(window);
    return true;
}
}; //namespace The