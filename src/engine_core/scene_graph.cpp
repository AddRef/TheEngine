#include "scene_graph.h"
#include "log.hxx"

#include <stack>

using namespace The;

///////////////////////////////////////////////////////////////////////////
// SceneGraph
///////////////////////////////////////////////////////////////////////////

SceneGraph::SceneGraph()
{}

SceneGraph::~SceneGraph()
{}

SceneGraph::Node SceneGraph::Add(Node target, ISceneElement& element)
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

const ISceneElement* SceneGraph::GetData(Node target) const
{
    THE_ERROR_IF(!target, "Invalid argument", return nullptr);
    NodeImpl* node = static_cast<NodeImpl*>(target);
    return node->GetData();
}

void SceneGraph::Update(Node target, const NodeState& state, uint64_t ts)
{
    THE_ERROR_IF(!target, "Invalid argument", return);
    NodeImpl* node = static_cast<NodeImpl*>(target);
    NodeState children_state;
    ISceneElement::Result result = node->GetData()->Update(state, children_state, ts);
    THE_ERROR_IF(result == ISceneElement::Result::Fail, "Failed to update ISceneObject: " << node->GetData(), return);
    for (auto& child : node->GetChildren())
    {
        if (result == ISceneElement::Result::StateHasNotBeenUpdated)
            Update(&child, state, ts);
        else
            Update(&child, children_state, ts);
    }
}

///////////////////////////////////////////////////////////////////////////
// SceneGraph::NodeImpl
///////////////////////////////////////////////////////////////////////////
SceneGraph::NodeImpl::NodeImpl(ISceneElement& element)
    : m_scene_element(&element)
{}

SceneGraph::Node SceneGraph::NodeImpl::Add(ISceneElement& element)
{
    m_children.push_back(NodeImpl(element));
    return this;
}

SceneGraph::Node SceneGraph::NodeImpl::Remove(const ISceneElement& element)
{
    // const ISceneElement* element_ptr = &element;
    m_children.remove_if([&element](const NodeImpl& node) {
        return node.GetData() == &element;
    });
    return this;
}
