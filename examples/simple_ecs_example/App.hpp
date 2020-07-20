#ifndef MAGMATIC_APP_HPP
#define MAGMATIC_APP_HPP

#include "ecs/ECS.hpp"

class App
{
public:
	App();

	void start();
private:
	void loop();

	magmatic::ecs::ECS ecs_;

	bool should_quit_ = false;
};


#endif //MAGMATIC_APP_HPP
