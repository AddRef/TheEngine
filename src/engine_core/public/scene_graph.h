#pragma once
#include "misc.hxx"

#include <cstdint>
#include <list>

namespace The
{

struct NodeState
{
    // Some node state comes here
};

struct ISceneElement : public QueryEnabled
{
    virtual void Update(NodeState& state, uint64_t ts) = 0;
};

class SceneGraph
{
public:
    typedef struct INode {} *Node;
    SceneGraph();
    ~SceneGraph();

    Node GetRoot() { return &m_root; }
    Node Add(Node target, const ISceneElement& element);
    void Remove(Node target, const ISceneElement& element);
    std::list<Node> GetChildren(Node target) const;
private:
    class NodeImpl : public INode
    {
    public:
        NodeImpl(const ISceneElement& element);
        NodeImpl() {}
        Node Add(const ISceneElement& element);
        Node Remove(const ISceneElement& element);
        std::list<NodeImpl>& GetChildren() { return m_children; }
        const ISceneElement& GetData() const { return *m_scene_element; }
    private:
        std::list<NodeImpl> m_children;
        const ISceneElement* m_scene_element;
    };
    NodeImpl m_root;
};
}; //namespace The