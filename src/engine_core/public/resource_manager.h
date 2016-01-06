#pragma once
#include "resource_data.h"
#include <memory>

template <typename TBase>
struct Shared
{
	using Ptr = std::shared_ptr<TBase>;
	using CPtr = std::shared_ptr<TBase>;
};

namespace The
{

class Texture : public Shared<Texture>
{
};

class Shader : public Shared<Shader>
{
};

class Mesh : public Shared<Mesh>
{

};

class ResourceManager
{
public:
    ResourceManager(EngineCore& engine_core);
    Texture::Ptr CreateTexture(const TextureData& texture_data);
    Shader::Ptr CreateShader(const ShaderData& shader_data);
    Mesh::Ptr CreateMesh(const MeshData& mesh_data);
};

}