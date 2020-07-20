#ifndef MAGMATIC_ENDSYSTEM_HPP
#define MAGMATIC_ENDSYSTEM_HPP

#include "ecs/System.hpp"
#include "Events.hpp"

class EndSystem : public magmatic::ecs::System
{
public:
	virtual void configure(magmatic::ecs::EventRelay& event_relay) override
	{
		event_relay_ = &event_relay;
	};

	virtual void update(
			const std::chrono::duration<int64_t, std::micro>& delta,
			magmatic::ecs::ComponentManager& component_manager
	) override
	{
		if(time_to_death_.count() < 0)
		{
			event_relay_->emit<ShouldQuit>();
		}
		time_to_death_ -= delta;
	};
private:
	magmatic::ecs::EventRelay* event_relay_;
	std::chrono::duration<int64_t, std::micro> time_to_death_ = std::chrono::seconds(10);
};


#endif //MAGMATIC_ENDSYSTEM_HPP
