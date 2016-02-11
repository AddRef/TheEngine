#include "scene_graph_elements.h"

using namespace The;

CameraElement::CameraElement(const Desc& desc)
    : m_desc(desc)
{
    m_desc.direction = glm::normalize(m_desc.direction);
    m_view = glm::lookAt(m_desc.eye_position, m_desc.eye_position + m_desc.direction, m_desc.up);
    m_projection = glm::perspective(m_desc.field_of_view_y, m_desc.aspect, m_desc.near_z, m_desc.far_z);
}

CameraElement::~CameraElement()
{}

ISceneElement::Result CameraElement::Update(const NodeState& state, NodeState& out_state, uint64_t ts)
{
    ts;
    out_state.current_matrix = state.current_matrix * m_view * m_projection;
    return Result::Ok;
}

ObjectElement::ObjectElement()
{}

ObjectElement::~ObjectElement()
{
}

ISceneElement::Result ObjectElement::Update(const NodeState& state, NodeState& out_state, uint64_t ts)
{
    // Draw object here;
    return ISceneElement::Update(state, out_state, ts);
}

MovementElement::MovementElement(const vector3f& initial_shift)
    : m_shift(initial_shift)
{}

MovementElement::~MovementElement()
{}

ISceneElement::Result MovementElement::Update(const NodeState& state, NodeState& out_state, uint64_t ts)
{
    ts;
    if (m_shift == vector3f(0, 0, 0))
    {
        return Result::StateHasNotBeenUpdated;
    }
    out_state.current_matrix = glm::translate(glm::mat4(1.0), m_shift) * state.current_matrix;
    return Result::Ok;
}
