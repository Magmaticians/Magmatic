#ifndef MAGMATIC_MODEL_H
#define MAGMATIC_MODEL_H

#include "ModelData.hpp"
#include <memory>


namespace magmatic::render
{
	//forward declaration
	class LogicalDevice;
	class VertexBuffer;
	class IndexBuffer;
	class CommandPool;
	class Texture;

	class Model
	{
	public:
		std::unique_ptr<VertexBuffer> vertex_buffer;
		std::unique_ptr<IndexBuffer> index_buffer;
		std::vector<Texture> textures;
		std::vector<ModelData::NodeData::self_pointer> nodes;

		std::vector<ModelData::SamplerSettings> sampler_settings;

		std::vector<ModelData::MaterialData> material_data;
		std::vector<ModelData::TextureData> texture_data;

		Model(
				const LogicalDevice &l_device, const CommandPool &pool, const std::shared_ptr<ModelData> &data
		);
	};
}

#endif //MAGMATIC_MODEL_H
