project (Hazel)

file(GLOB_RECURSE source CONFIGURE_DEPENDS src/*.h src/*.cpp)

add_library (${PROJECT_NAME} SHARED ${source})

target_compile_definitions (${PROJECT_NAME} PRIVATE HZ_BUILD_DLL)
target_include_directories (${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)