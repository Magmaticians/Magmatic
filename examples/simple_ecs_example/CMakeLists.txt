target_sources(
		simple_example
		PRIVATE
		main.cpp
		App.cpp
		App.hpp
		Events.hpp
		EndSystem.hpp
		)

target_link_libraries(simple_example magmatic-render magmatic-ecs magmatic-core)