//
// Created by user on 13.04.2020.
//

#ifndef MAGMATIC_SEMAPHORETYPE_HPP
#define MAGMATIC_SEMAPHORETYPE_HPP


namespace magmatic {
	enum class SemaphoreType : uint8_t {
		ImageAvailableSemaphore = 0,
		RenderFinishedSemaphore = 1
	};
}


#endif //MAGMATIC_SEMAPHORETYPE_HPP
