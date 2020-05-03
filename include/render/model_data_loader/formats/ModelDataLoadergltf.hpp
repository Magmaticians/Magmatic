#ifndef MAGMATIC_MODELDATALOADERGLTF_HPP
#define MAGMATIC_MODELDATALOADERGLTF_HPP

#include "render/model_data_loader/formats/ModelDataLoaderConcrete.hpp"
#include "render/ModelData.hpp"
#include <memory>
#include <vector>

namespace tinygltf
{
	struct Node;
	struct Model;
}

namespace magmatic::render
{
	class ModelDataLoadergltf : public ModelDataLoaderConcrete
	{
		static constexpr const char* ASCII_ext = ".gltf";
		static constexpr const char* binary_ext = ".glb";
	public:
		using NodeData = ModelData::NodeData;
		using PNodeData = NodeData::self_pointer;

		std::shared_ptr<ModelData> load(const std::filesystem::path &file_path) override;

		std::shared_ptr<NodeData> loadNode(
				const PNodeData& parent,
				const tinygltf::Node& node,
				const tinygltf::Model& model,
				std::vector<Vertex>& vertices,
				std::vector<uint32_t>& indices
				);

		static std::string factoryName()
		{
			return "GLTF";
		}

	private:
		static bool registered_;

		void loadSamplers(std::vector<ModelData::SamplerSettings> &samplers, const tinygltf::Model &model);

		vk::Filter decodeFilterMode(int filter);

		vk::SamplerAddressMode decodeAddressMode(int wrap_mode);

		void loadMaterials(std::vector<ModelData::MaterialData> &materials, const tinygltf::Model &model);

		void loadTextures(std::vector<ModelData::TextureData> &textures, const tinygltf::Model &model);
	};
}


#endif //MAGMATIC_MODELDATALOADERGLTF_HPP
