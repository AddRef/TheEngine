#include "scene_graph_elements.h"

using namespace The;

CameraElement::CameraElement(const Desc& desc)
    : m_desc(desc)
{}

CameraElement::~CameraElement()
{}

void CameraElement::Update(NodeState& state, uint64_t ts)
{
    state; ts;
}

ObjectElement::ObjectElement()
{}

ObjectElement::~ObjectElement()
{
}

void ObjectElement::Update(NodeState& state, uint64_t ts)
{
    state; ts;
}

MovementElement::MovementElement()
{}

MovementElement::~MovementElement()
{}

void MovementElement::Update(NodeState& state, uint64_t ts)
{
    state; ts;
}
