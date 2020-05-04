#include "render/model/Model.hpp"
#include "render/model/ModelData.hpp"
#include "render/VertexBuffer.hpp"
#include "render/IndexBuffer.hpp"
#include "render/LogicalDevice.hpp"
#include "render/Texture.hpp"

magmatic::render::Model::Model(
		const LogicalDevice& l_device, const CommandPool& pool,
		const std::shared_ptr<ModelData>& data
)
{
	vertex_buffer = std::make_unique<VertexBuffer>(l_device, data->vertices, pool);
	index_buffer = std::make_unique<IndexBuffer>(l_device, data->indices, pool);
	std::copy(data->nodes.begin(), data->nodes.end(), std::back_inserter(nodes));
	textures.reserve(data->texture_bitmaps.size());
	std::transform(data->texture_bitmaps.begin(), data->texture_bitmaps.end(), std::back_inserter(textures),
	               [&l_device, &pool](const Bitmap &bitmap) {
		               return std::make_shared<Texture>(l_device, bitmap, pool);
	               });
	material_data = data->material_data;
	texture_data = data->texture_data;
}
