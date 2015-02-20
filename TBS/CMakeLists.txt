cmake_minimum_required(VERSION 3.0)
project(TBS)

file(GLOB_RECURSE ${PROJECT_NAME}.Sources "src/*.cpp")
add_library(${PROJECT_NAME}	SHARED ${${PROJECT_NAME}.Sources})
add_library(${PROJECT_NAME}_Static	STATIC ${${PROJECT_NAME}.Sources})

target_include_directories(${PROJECT_NAME} 
	PUBLIC include
)
target_include_directories(${PROJECT_NAME}_Static
	PUBLIC include
)

target_link_libraries(${PROJECT_NAME}
	PocoFoundation
	PocoUtil
	PocoData
)
target_link_libraries(${PROJECT_NAME}_Static
	PocoFoundation
	PocoUtil
	PocoData
)

installDefault(${PROJECT_NAME} lib)

