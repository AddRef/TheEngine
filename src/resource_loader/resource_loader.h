#pragme once

namespace The
{

class TextureData
{
public:
    TextureData();
};

class GeometryData
{
public:
    GeometryData();
};

class ShaderData
{
public:
    ShaderData();
};

class Loader
{
    enum class Error
    {
        Ok,
        PathDoesntExist
    };
    static Error LoadTexture(const std::string& path, TextureData& out_data);
    static Error LoadGeometry(const std::string& path, GeometryData& out_data);
    static Error LoadShader(const std::string& path, ShaderData& out_data);
};
}