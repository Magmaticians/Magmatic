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
#include <functional>


namespace magmatic::render
{
	class ModelData
	{
	public:
		struct SamplerSettings
		{
			vk::Filter min_filter;
			vk::Filter mag_filter;
			vk::SamplerAddressMode mode_U;
			vk::SamplerAddressMode mode_V;
			vk::SamplerAddressMode mode_W;
		};

		struct PrimitiveData {
			uint32_t first_index;
			uint32_t index_count;
			uint32_t vertex_count;

			uint32_t material_index;
		};

		using MeshData = std::vector<PrimitiveData>;

		struct MaterialData
		{
			glm::vec4 color_factor = glm::vec4(1.0f);
			uint32_t texture_index;
		};

		struct TextureData
		{
			uint16_t bitmap_index;
			std::optional<int16_t> sampler_index;
		};

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

		std::vector<MaterialData> material_data;
		std::vector<TextureData> texture_data;
		std::vector<Bitmap> texture_bitmaps;

		std::vector<NodeData::self_pointer> nodes;
	public:
		ModelData() = default;

		ModelData(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices)
		: vertices(std::move(vertices)), indices(std::move(indices)) {};
	};
}

namespace std {
	template<> struct hash<magmatic::render::ModelData::SamplerSettings> {
		size_t operator()(magmatic::render::ModelData::SamplerSettings const& sampler_settings) const {
			const auto mag_filter = std::hash<int>()(static_cast<int>(sampler_settings.mag_filter));
			const auto min_filter = std::hash<int>()(static_cast<int>(sampler_settings.min_filter));
			const auto mode_u = std::hash<int>()(static_cast<int>(sampler_settings.mode_U));
			const auto mode_w = std::hash<int>()(static_cast<int>(sampler_settings.mode_W));
			const auto mode_v = std::hash<int>()(static_cast<int>(sampler_settings.mode_V));

			return (((((mag_filter
			^ (min_filter << 1u) >> 1u)
			^ (mode_u << 1u)) >> 1u)
			^ (mode_w << 1u)) >> 1u)
			^ (mode_v << 1u);
		}
	};
}


#endif //MAGMATIC_MODELDATA_HPP
