#ifndef MAGMATIC_MODEL_HPP
#define MAGMATIC_MODEL_HPP

#include "render/Buffer.hpp"
#include "render/Vertex.hpp"
#include "render/LogicalDevice.hpp"

namespace magmatic::render
{
	class Model
	{
	public:
		Buffer vertex_buffer;
		Buffer index_buffer;
	public:
		Model(Buffer vertex, Buffer index): vertex_buffer(std::move(vertex)), index_buffer(std::move(index)) {};
	};
}


#endif //MAGMATIC_MODEL_HPP
