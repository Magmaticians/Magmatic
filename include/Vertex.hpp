#ifndef MAGMATIC_VERTEX_HPP
#define MAGMATIC_VERTEX_HPP

#include <iostream>
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

namespace magmatic {
	struct Vertex {
		glm::vec2 position;
		glm::vec3 color;

		static vk::VertexInputBindingDescription getBindingDescription() {
			return vk::VertexInputBindingDescription(
					0,
					sizeof(Vertex),
					vk::VertexInputRate::eVertex
			);
		}

		static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions() {
			return {
					vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position)),
					vk::VertexInputAttributeDescription(0, 1, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color))
			};
		}
	};
}

#endif //MAGMATIC_VERTEX_HPP
