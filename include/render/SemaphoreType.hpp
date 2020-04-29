#ifndef MAGMATIC_SEMAPHORETYPE_HPP
#define MAGMATIC_SEMAPHORETYPE_HPP


namespace magmatic::render {
	enum class SemaphoreType : uint8_t {
		ImageAvailableSemaphore = 0,
		RenderFinishedSemaphore = 1
	};
}


#endif //MAGMATIC_SEMAPHORETYPE_HPP
