#pragma once

#include <glm/glm.hpp>

namespace Crystal::Math { 

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranlation, glm::vec3& outRotation, glm::vec3& outScale);

}