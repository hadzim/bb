cmake_minimum_required(VERSION 3.0)
project(BB)

file(GLOB_RECURSE ${PROJECT_NAME}.Sources "src/*.cpp")
add_library(${PROJECT_NAME}	SHARED ${${PROJECT_NAME}.Sources})

find_package( OpenCV REQUIRED core imgproc highgui )


target_include_directories(${PROJECT_NAME}
	PUBLIC include
	PRIVATE src
	PRIVATE ${PROJECT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC TBS
	PUBLIC TBS.Services
	PUBLIC BB.Services
	${OpenCV_LIBS}
	PUBLIC mosquitto
	PUBLIC mosquittopp
	
)

if (${TargetDevice} STREQUAL "lindevice" OR ${TargetDevice} STREQUAL "tpro")
	
    target_link_libraries(${PROJECT_NAME} PUBLIC systemd-daemon)

elseif(${TargetDevice} STREQUAL "apalismx" OR ${TargetDevice} STREQUAL "RPi")
	  
    target_link_libraries(${PROJECT_NAME} PUBLIC systemd)

endif()    

installDefault(${PROJECT_NAME} lib)