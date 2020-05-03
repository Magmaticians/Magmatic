#ifndef MAGMATIC_MODELDATA_HPP
#define MAGMATIC_MODELDATA_HPP

#include "render/Vertex.hpp"
#include "render/LogicalDevice.hpp"
#include "render/Bitmap.hpp"

#include <vector>
#include <vulkan/vulkan.hpp>
//todo: optimize import
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>


namespace magmatic::render
{
	class ModelData
	{
	public:
		struct SamplerSettings
		{
			vk::Filter min_filter;
			vk::Filter mag_filter;
			vk::SamplerAddressMode modeU;
			vk::SamplerAddressMode modeV;
			vk::SamplerAddressMode modeW;
		};

		struct PrimitiveData {
			uint32_t first_index;
			uint32_t index_count;
			uint32_t vertex_count;
		};

		using MeshData = std::vector<PrimitiveData>;



		struct NodeData
		{
			using self_pointer = std::shared_ptr<NodeData>;

			self_pointer parent;
			std::vector<self_pointer> children;
			glm::mat4 matrix;

			MeshData mesh_data;
		};
	public:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		std::vector<SamplerSettings> sampler_settings;
		std::vector<Bitmap> texture_bitmaps;

		std::vector<NodeData::self_pointer> nodes;
	public:
		ModelData() = default;

		ModelData(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices)
		: vertices(std::move(vertices)), indices(std::move(indices)) {};
	};
}


#endif //MAGMATIC_MODELDATA_HPP
