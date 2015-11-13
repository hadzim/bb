cmake_minimum_required(VERSION 3.0)
project(BB.History)

file(GLOB_RECURSE ${PROJECT_NAME}.Sources "src/*.cpp")
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}.Sources})

target_include_directories(${PROJECT_NAME}
	PUBLIC include
	PRIVATE src
	PRIVATE ${PROJECT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME}
	TBS
	BB
)

installDefault(${PROJECT_NAME} .)
servicefile()