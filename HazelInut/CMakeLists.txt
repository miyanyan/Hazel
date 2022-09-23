project (HazelInut)

add_executable (${PROJECT_NAME} 
	src/HazelnutApp.cpp 
	src/EditorLayer.h 
	src/EditorLayer.cpp 
	src/Panels/SceneHierarchyPanel.h
	src/Panels/SceneHierarchyPanel.cpp
)

target_link_libraries(${PROJECT_NAME} Hazel)
