#pragma once
#include <string>
#include <cstdint>
#include <vector>

namespace The
{

class ResourceData
{
public:
    virtual bool Load(const std::string& path) = 0;
    virtual bool IsLoaded() const
    {
        return m_loaded;
    }
protected:
    bool m_loaded = false;
};

class TextureData final : public ResourceData
{
public:
    TextureData();
    ~TextureData();
    // ResourceData
    virtual bool Load(const std::string& path) override;
};

class MeshData final : public ResourceData
{
public:
    MeshData();
    ~MeshData();

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

        template <typename TElementDesc=ElementDesc>
        ElementDesc& operator = (TElementDesc&& rvalue)
        {
            Data = std::forward(rvalue.Data);
            ElementSize = rvalue.ElementSize;
            return *this;
        }
    };

    struct Shape
    {
        ElementDesc<float>      Positions;
        ElementDesc<float>      TexCoords;
        ElementDesc<float>      Normals;
        ElementDesc<uint32_t>   Indices;
        LayoutType              LayoutType;

        template <typename TShape = Shape>
        Shape& operator = (TShape&& shape)
        {
            Positions = std::forward(shape.Positions);
            TexCoords = std::forward(shape.TexCoords);
            Normals = std::forward(shape.Normals);
            Indices = std::forward(shape.Indices);
            LayoutType = std::forward(shape.LayoutType);
            return *this;
        }
    };
    // ResourceData
    virtual bool Load(const std::string& path) override;
    // MeshData
    const std::vector<Shape>& GetDesc() const { return m_shapes; }
    void AddShape(const Shape& shape) { m_shapes.push_back(shape); }
    void AddShape(Shape&& shape) { m_shapes.push_back(std::move(shape)); }
    void Clear() { m_shapes.clear(); }
private:
    std::vector<Shape> m_shapes;
};

class ShaderData final : public ResourceData
{
public:    
    ShaderData();
    ~ShaderData();
    // ResourceData
    virtual bool Load(const std::string& path) override;
    // ShaderData

};

}