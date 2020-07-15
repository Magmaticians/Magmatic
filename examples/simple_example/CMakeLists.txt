target_sources(
		simple_example
		PRIVATE
		main.cpp
		)

target_link_libraries(simple_example magmatic-render magmatic-ecs magmatic-core)