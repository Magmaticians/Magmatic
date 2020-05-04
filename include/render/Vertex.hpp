#ifndef MAGMATIC_VERTEX_HPP
#define MAGMATIC_VERTEX_HPP

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <vulkan/vulkan.hpp>

namespace magmatic::render {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 uv;

		static constexpr uint32_t binding = 0;

		static vk::VertexInputBindingDescription getBindingDescription()
		{
			return vk::VertexInputBindingDescription(
					binding,
					sizeof(Vertex),
					vk::VertexInputRate::eVertex
			);
		}

		static std::array<vk::VertexInputAttributeDescription, 4> getAttributeDescriptions() {
			return {
					vk::VertexInputAttributeDescription(0, binding, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position)),
					vk::VertexInputAttributeDescription(1, binding, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal)),
					vk::VertexInputAttributeDescription(2, binding, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color)),
					vk::VertexInputAttributeDescription(3, binding, vk::Format::eR32G32Sfloat, offsetof(Vertex, uv))
			};
		}
	};
}

namespace std {
	template<> struct hash<magmatic::render::Vertex> {
		size_t operator()(magmatic::render::Vertex const& vertex) const {
			return (((hash<glm::vec3>()(vertex.position) ^
			         (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
			       (hash<glm::vec3>()(vertex.color) << 1) >> 1) ^
					(hash<glm::vec2>()(vertex.uv) << 1);
		}
	};
}

#endif //MAGMATIC_VERTEX_HPP
