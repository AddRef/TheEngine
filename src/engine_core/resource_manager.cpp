#include "resource_manager.h"

namespace The
{
ResourceManager::ResourceManager(EngineCore& engine_core)
    : m_engine_core(&engine_core)
{

}

Texture::Ptr ResourceManager::CreateTexture(const TextureData& texture_data)
{
    texture_data;
    return nullptr;
}

Shader::Ptr ResourceManager::CreateShader(const ShaderData& shader_data)
{
    shader_data;
    return nullptr;
}

Mesh::Ptr ResourceManager::CreateMesh(const MeshData& mesh_data)
{
    mesh_data;
    return nullptr;
}
};