#pragma once
#include <string>
#include <cstdint>
#include <vector>

namespace The
{

class ResourceData
{
public:
    virtual bool IsLoaded() const
    {
        return m_loaded;
    }
protected:
    bool m_loaded = false;
};

class TextureData : public ResourceData
{
public:
    TextureData();
    ~TextureData();
    bool Load(const std::string& path);
};

class MeshData : public ResourceData
{
public:
    MeshData();
    ~MeshData();
    bool Load(const std::string& path);

    enum class LayoutType
    {
        Triangle,
        Strip,
        Fan,
        Points
    };

    template <typename Type>
    struct ElementDesc
    {
        std::vector<Type>   Data;
        uint32_t            ElementSize;
    };

    typedef ElementDesc<float>      PositionsDesc;
    typedef ElementDesc<float>      TexCoordDesc;
    typedef ElementDesc<float>      NormalsDesc;
    typedef ElementDesc<uint32_t>   IndicesDesc;

    struct Shape
    {
        ElementDesc<float>      Positions;
        ElementDesc<float>      TexCoords;
        ElementDesc<float>      Normals;
        ElementDesc<uint32_t>   Indices;
        LayoutType              LayoutType;
    };

    virtual const std::vector<Shape>& GetDesc() const { return m_shapes; }
private:
    std::vector<Shape> m_shapes;
};

class ShaderData : public ResourceData
{
public:    
    ShaderData();
    ~ShaderData();
    bool Load(const std::string& path);
};

}