#pragma once
#include <memory>

namespace The
{

class SceneGraph
{
    typedef void* Node;
    const static Node Root;
    Node AddObject(Node target_node, ISceneObject* child);
};

struct ISceneObject
{

};

class Scene
{
public:
    Scene();

};

class EngineCore
{
public:
	EngineCore();
    void SetScene(Scene* scene);

private:
	
};

};