#pragma once
#include <Crystal/Core/UUID.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <yaml-cpp/yaml.h>

namespace YAML {
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs);

		static bool decode(const Node& node, glm::vec2& rhs);
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs);

		static bool decode(const Node& node, glm::vec3& rhs);
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs);

		static bool decode(const Node& node, glm::vec4& rhs);
	};
	template<>
	struct convert<Crystal::UUID>
	{
		static Node encode(const Crystal::UUID& uuid);

		static bool decode(const Node& node, Crystal::UUID& uuid);
	};
}

namespace Crystal
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v);

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v);

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v);
}
