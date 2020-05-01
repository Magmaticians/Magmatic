target_sources(
		model_info
		PRIVATE
		main.cpp
)

target_link_libraries(model_info PRIVATE magmatic-render)
