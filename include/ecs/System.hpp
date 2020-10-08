#ifndef MAGMATIC_SYSTEM_HPP
#define MAGMATIC_SYSTEM_HPP

#include "ECSTypes.hpp"
#include "EventRelay.hpp"
#include <set>
#include <chrono>


namespace magmatic::ecs
{
	class System
	{
	public:
		virtual void configure(EventRelay& event_relay) {};

		virtual void run(
				const std::chrono::duration<int64_t, std::micro>& delta
				) = 0;

		virtual ~System() = default;
	};
}

#endif //MAGMATIC_SYSTEM_HPP
