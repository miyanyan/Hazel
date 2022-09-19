project (HazelInut)

add_executable (${PROJECT_NAME} src/HazelnutApp.cpp src/EditorLayer.h src/EditorLayer.cpp)

target_link_libraries(${PROJECT_NAME} Hazel)
