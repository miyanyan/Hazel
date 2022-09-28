project (HazelInut)

find_package(unofficial-nativefiledialog CONFIG REQUIRED)

add_executable (${PROJECT_NAME} 
	src/HazelnutApp.cpp 
	src/EditorLayer.h 
	src/EditorLayer.cpp 
	src/Panels/SceneHierarchyPanel.h
	src/Panels/SceneHierarchyPanel.cpp
)

target_link_libraries(${PROJECT_NAME} Hazel)
target_link_libraries(${PROJECT_NAME} unofficial::nativefiledialog::nfd)
