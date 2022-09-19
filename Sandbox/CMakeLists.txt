project (Sandbox)

add_executable (${PROJECT_NAME} src/SandboxApp.cpp src/Sandbox2D.h src/Sandbox2D.cpp)

target_link_libraries(${PROJECT_NAME} Hazel)
