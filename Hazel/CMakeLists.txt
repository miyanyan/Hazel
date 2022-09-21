project (Hazel)

find_package (spdlog REQUIRED)
find_package (glad REQUIRED)
find_package (glfw3 REQUIRED)
find_package (imgui REQUIRED)
find_package (glm REQUIRED)
find_package (EnTT REQUIRED)

file (GLOB_RECURSE source CONFIGURE_DEPENDS src/*.h src/*.cpp)

add_library (${PROJECT_NAME} STATIC ${source})

target_compile_definitions(${PROJECT_NAME} PUBLIC ENABLE_OPENGL_CHECK)
#target_compile_definitions (${PROJECT_NAME} PRIVATE HZ_BUILD_DLL)
target_include_directories (${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/external)
target_link_libraries (${PROJECT_NAME} PUBLIC 
	spdlog::spdlog_header_only
	glad::glad 
	glfw
	imgui::imgui
	glm::glm
	EnTT::EnTT
)
