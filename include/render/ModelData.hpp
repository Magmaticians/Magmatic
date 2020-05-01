#ifndef MAGMATIC_MODELDATA_HPP
#define MAGMATIC_MODELDATA_HPP

#include "render/Vertex.hpp"
#include "render/LogicalDevice.hpp"

#include <vector>

namespace magmatic::render
{
	class ModelData
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	public:
		ModelData(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices)
		: vertices(std::move(vertices)), indices(std::move(indices)) {};
	};
}


#endif //MAGMATIC_MODELDATA_HPP
