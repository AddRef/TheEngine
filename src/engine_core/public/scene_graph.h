#pragma once
#include "misc.hxx"
#include "math.hxx"

#include <cstdint>
#include <list>
#include <stack>

namespace The
{

struct NodeState
{
    mat4 current_matrix;
};

struct ISceneElement : public QueryEnabled
{
    enum class Result
    {
        Ok,
        Fail,
        StateHasNotBeenUpdated
    };
    // Returns StateHasNotBeenUpdated when 'out_state' hasn't been written so 'state' should be used instead
    virtual Result Update(const NodeState& state, NodeState& out_state, uint64_t ts) { state; out_state; ts; return Result::StateHasNotBeenUpdated; }
};

class SceneGraph
{
public:
    typedef struct INode {} *Node;
    SceneGraph();
    ~SceneGraph();

    Node GetRoot() { return &m_root; }
    Node Add(Node target, ISceneElement& element);
    void Remove(Node target, const ISceneElement& element);
    std::list<Node> GetChildren(Node target) const;
    const ISceneElement* GetData(Node target) const;
    void Update(Node target, const NodeState& initial_state, uint64_t ts);
private:
    class NodeImpl : public INode
    {
    public:
        NodeImpl(ISceneElement& element);
        NodeImpl() {}
        Node Add(ISceneElement& element);
        Node Remove(const ISceneElement& element);
        std::list<NodeImpl>& GetChildren() { return m_children; }
        ISceneElement* GetData() const { return m_scene_element; }
    private:
        std::list<NodeImpl> m_children;
        ISceneElement* m_scene_element;
    };
    NodeImpl m_root;
};
}; //namespace The