#ifndef MAGMATIC_TEXTUREDVERTEX_HPP
#define MAGMATIC_TEXTUREDVERTEX_HPP

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>


namespace magmatic::render
{
	struct TexturedVertex
	{
		glm::vec2 position;
		glm::vec2 textureCoord;

		static constexpr uint32_t binding = 0;

		static vk::VertexInputBindingDescription getBindingDescription()
		{
			return vk::VertexInputBindingDescription(
					binding,
					sizeof(TexturedVertex),
					vk::VertexInputRate::eVertex
					);
		}

		static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescription()
		{
			return{
				vk::VertexInputAttributeDescription(
						0,
						binding,
						vk::Format::eR32G32Sfloat,
						offsetof(TexturedVertex, position)
						),
				vk::VertexInputAttributeDescription(
						1,
						binding,
						vk::Format::eR32G32Sfloat,
						offsetof(TexturedVertex, textureCoord)
						)
			}
		}
	};
}


#endif //MAGMATIC_TEXTUREDVERTEX_HPP
