#pragma once
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace The
{
using vector2f = glm::vec2;
using vector3f = glm::vec3;
using vector4f = glm::vec4;

using vector2i = glm::highp_ivec2;
using vector3i = glm::highp_ivec3;
using vector4i = glm::highp_ivec4;

using vector2u = glm::highp_uvec2;
using vector3u = glm::highp_uvec3;
using vector4u = glm::highp_uvec4;

using quat = glm::quat;
using mat4 = glm::mat4;
};