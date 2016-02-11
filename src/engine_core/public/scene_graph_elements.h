#include "scene_graph.h"
#include "math.hxx"

namespace The
{
class CameraElement : public ISceneElement
{
public:
    struct Desc
    {
        vector3f eye_position;
        vector3f direction;
        vector3f up;
        double   aspect;
        double   field_of_view_y;
        double   near_z;
        double   far_z;
    };

    CameraElement(const Desc& desc);
    ~CameraElement();
    const Desc& GetDesc() const { return m_desc; }
    virtual Result Update(const NodeState& state, NodeState& out_state, uint64_t ts) override;
private:
    Desc m_desc;
    mat4 m_viewMatrix;
    mat4 m_projectionMatrix;
};

class ObjectElement : public ISceneElement
{
public:
    ObjectElement();
    ~ObjectElement();
    virtual Result Update(const NodeState& state, NodeState& out_state, uint64_t ts) override;
private:
};

class MovementElement : public ISceneElement
{
public:
    MovementElement();
    ~MovementElement();
    virtual Result Update(const NodeState& state, NodeState& out_state, uint64_t ts) override;
private:
};
}; // namespace The