#include "log.hxx"
#include "engine_core.h"

INITIALIZE_EASYLOGGINGPP

namespace The
{
SceneGraph::Node SceneGraph::AddObject(Node target_node, ISceneObject* child)
{
    target_node; child;
    return Node();
}

bool EngineCore::Init(The::Window&& window)
{
    m_window = std::move(window);
    return true;
}
};