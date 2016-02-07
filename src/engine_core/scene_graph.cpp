#include "scene_graph.h"
#include "log.hxx"

using namespace The;

SceneGraph::SceneGraph()
{}

SceneGraph::~SceneGraph()
{}

SceneGraph::Node SceneGraph::Add(Node target, const ISceneElement& element)
{
    THE_ERROR_IF(!target, "Invalid argument", return nullptr);
    NodeImpl* node = static_cast<NodeImpl*>(target);
    node->Add(element);
    return node;
}

void SceneGraph::Remove(Node target, const ISceneElement& element)
{
    THE_ERROR_IF(!target, "Invalid argument", return);
    NodeImpl* node = static_cast<NodeImpl*>(target);
    node->Remove(element);
}

std::list<SceneGraph::Node> SceneGraph::GetChildren(Node target) const
{
    THE_ERROR_IF(!target, "Invalid argument", return std::list<Node>());
    NodeImpl* node = static_cast<NodeImpl*>(target);
    std::list<NodeImpl>& children = node->GetChildren();
    std::list<Node> result;
    for (NodeImpl& child : children)
    {
        result.push_back(&child);
    }
    return result;
}

SceneGraph::NodeImpl::NodeImpl(const ISceneElement& element)
    : m_scene_element(&element)
{}

SceneGraph::Node SceneGraph::NodeImpl::Add(const ISceneElement& element)
{
    m_children.push_back(NodeImpl(element));
    return this;
}

SceneGraph::Node SceneGraph::NodeImpl::Remove(const ISceneElement& element)
{
    // const ISceneElement* element_ptr = &element;
    m_children.remove_if([&element](const NodeImpl& node) {
        return &node.GetData() == &element;
    });
    return this;
}
