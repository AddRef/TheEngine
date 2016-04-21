#pragma once
#include "resource_data.h"
#include "misc.hxx"

#include <memory>


namespace The
{
class EngineCore;

class Texture
{
};

class Shader
{
};

class Mesh
{

};

class ResourceManager
{
public:
    ResourceManager(EngineCore& engine_core);
private:
	EngineCore* m_engine_core = nullptr;
};

}