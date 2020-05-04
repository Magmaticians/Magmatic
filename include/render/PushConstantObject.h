#ifndef MAGMATIC_PUSHCONSTANTOBJECT_H
#define MAGMATIC_PUSHCONSTANTOBJECT_H

#include <glm/glm.hpp>

namespace magmatic::render
{
	struct PushConstantObject
	{
		uint8_t texture_index;
		uint8_t sampler_index;
		glm::mat4 matrix;
	};
}

#endif //MAGMATIC_PUSHCONSTANTOBJECT_H
