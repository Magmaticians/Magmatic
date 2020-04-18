#ifndef MAGMATIC_UNIFORMBUFFEROBJECT_HPP
#define MAGMATIC_UNIFORMBUFFEROBJECT_HPP

#include <glm/glm.hpp>

namespace magmatic {
	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};
}

#endif //MAGMATIC_UNIFORMBUFFEROBJECT_HPP
