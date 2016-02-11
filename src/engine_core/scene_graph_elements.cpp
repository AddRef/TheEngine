#include "scene_graph_elements.h"

using namespace The;

CameraElement::CameraElement(const Desc& desc)
    : m_desc(desc)
{
    m_desc.direction = glm::normalize(m_desc.direction);
    m_viewMatrix = glm::lookAt(m_desc.eye_position, m_desc.eye_position + m_desc.direction, m_desc.up);
    m_projectionMatrix = glm::perspective(m_desc.field_of_view_y, m_desc.aspect, m_desc.near_z, m_desc.far_z);
}

CameraElement::~CameraElement()
{}

ISceneElement::Result CameraElement::Update(const NodeState& state, NodeState& out_state, uint64_t ts)
{
    state; out_state; ts;
    return ISceneElement::Result::StateHasNotBeenUpdated;
}

ObjectElement::ObjectElement()
{}

ObjectElement::~ObjectElement()
{
}

ISceneElement::Result ObjectElement::Update(const NodeState& state, NodeState& out_state, uint64_t ts)
{
    state; out_state; ts;
    return ISceneElement::Result::StateHasNotBeenUpdated;
}

MovementElement::MovementElement()
{}

MovementElement::~MovementElement()
{}

ISceneElement::Result MovementElement::Update(const NodeState& state, NodeState& out_state, uint64_t ts)
{
    state; out_state; ts;
    return ISceneElement::Result::StateHasNotBeenUpdated;
}
