target_sources(main-test PRIVATE MainTest.cpp Application.hpp Application.cpp)

add_library(vertex_shader OBJECT "shader.vert")
add_library(fragment_shader OBJECT "shader.frag")

add_dependencies(main-test vertex_shader fragment_shader)