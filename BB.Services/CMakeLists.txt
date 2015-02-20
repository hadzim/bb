cmake_minimum_required(VERSION 3.0)
project(BB.Services)

file(GLOB_RECURSE ${PROJECT_NAME}.Sources "src/*.cpp")
add_library(${PROJECT_NAME}	SHARED ${${PROJECT_NAME}.Sources})

target_include_directories(${PROJECT_NAME}
	PUBLIC include
	PRIVATE src
	PRIVATE ${PROJECT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME}
	TBS
	TBS.Services
)

installDefault(${PROJECT_NAME} lib)