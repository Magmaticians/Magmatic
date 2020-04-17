target_sources(
		sound_test
		PRIVATE
		main.cpp
)
target_link_libraries(sound_test PRIVATE magmatic-sound)
