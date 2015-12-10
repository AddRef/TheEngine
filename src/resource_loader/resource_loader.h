#pragme once
#include "common.h"

#include <string>

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
};

class ShaderData : 
{
public:    
    ShaderData();
    ~ShaderData();
    bool Load(const std::string& path);
};
}