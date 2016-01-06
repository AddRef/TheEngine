#pragma once
#include "resource_data.h"
#include "misc.hxx"

#include <memory>


namespace The
{
class EngineCore;

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
private:
	EngineCore* m_engine_core = nullptr;
};

}