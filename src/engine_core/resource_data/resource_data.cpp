#include "resource_data.h"
#include "tiny_obj_loader.h"
#include "log.hxx"

namespace The
{
TextureData::TextureData()
{

}
TextureData::~TextureData()
{

}
bool TextureData::Load(const std::string& /*path*/)
{
    return false;
}

MeshData::MeshData()
{

}
MeshData::~MeshData()
{

}
bool MeshData::Load(const std::string& path)
{
    std::vector<tinyobj::shape_t> shapes;
    std::string err = tinyobj::LoadObj(shapes, path.c_str());
    THE_ERROR_IF(!err.empty(), err, return false);
    m_shapes.resize(shapes.size());
    for (uint32_t i = 0; i < shapes.size(); ++i)
    {
        m_shapes[i].Positions.Data = shapes[i].mesh.positions;
        m_shapes[i].Positions.ElementSize = 3;
        m_shapes[i].LayoutType = LayoutType::Triangle;

        m_shapes[i].Normals.Data = shapes[i].mesh.normals;
        m_shapes[i].Normals.ElementSize = 2;

        m_shapes[i].TexCoords.Data = shapes[i].mesh.texcoords;
        m_shapes[i].TexCoords.ElementSize = 2;

        m_shapes[i].Indices.Data = shapes[i].mesh.indices;
        m_shapes[i].Indices.ElementSize = 1;
    }
    return true;
}

ShaderData::ShaderData()
{

}
ShaderData::~ShaderData()
{

}

bool ShaderData::Load(const std::string& /*path*/)
{
   return false;
}
};
