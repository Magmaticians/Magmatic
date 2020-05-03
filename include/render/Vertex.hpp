#ifndef MAGMATIC_VERTEX_HPP
#define MAGMATIC_VERTEX_HPP

#include <glm/glm.hpp>
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

#endif //MAGMATIC_VERTEX_HPP
