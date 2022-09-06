project (Sandbox)

add_executable (${PROJECT_NAME} src/SandboxApp.cpp src/Sandbox2D.h src/Sandbox2D.cpp)

target_link_libraries(${PROJECT_NAME} Hazel)

add_custom_target(COPY_ASSETS 
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)

add_dependencies(${PROJECT_NAME} COPY_ASSETS)