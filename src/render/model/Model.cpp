#include "render/model/Model.h"
#include "render/model/ModelData.hpp"
#include "render/VertexBuffer.hpp"
#include "render/IndexBuffer.hpp"
#include "render/LogicalDevice.hpp"

magmatic::render::Model::Model(
		const LogicalDevice& l_device, const CommandPool& pool,
		const std::shared_ptr<ModelData>& data
)
{
	vertex_buffer = std::make_unique<VertexBuffer>(l_device, data->vertices, pool);
	index_buffer = std::make_unique<IndexBuffer>(l_device, data->indices, pool);
	std::copy(data->nodes.begin(), data->nodes.end(), std::back_inserter(nodes));
}
