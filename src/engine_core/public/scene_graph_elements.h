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
    mat4 m_view;
    mat4 m_projection;
};

class ObjectElement : public ISceneElement
{
public:
    ObjectElement();
    ~ObjectElement();
private:
};

class MovementElement : public ISceneElement
{
public:
    MovementElement(const vector3f& initial_shift);
    ~MovementElement();
    virtual Result Update(const NodeState& state, NodeState& out_state, uint64_t ts) override;
    void Shift(const vector3f& shift);
private:
    vector3f m_shift;
};
}; // namespace The