#pragma once
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

typedef glm::vec2 vector2f;
typedef glm::vec3 vector3f;
typedef glm::vec4 vector4f;

typedef glm::highp_ivec2 vector2i;
typedef glm::highp_ivec3 vector3i;
typedef glm::highp_ivec4 vector4i;

typedef glm::highp_uvec2 vector2u;
typedef glm::highp_uvec3 vector3u;
typedef glm::highp_uvec4 vector4u;

typedef glm::quat quat;