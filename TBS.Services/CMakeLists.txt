cmake_minimum_required(VERSION 3.0)
project(TBS.Services)

file(GLOB_RECURSE ${PROJECT_NAME}.Sources "src/*.cpp")
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}.Sources})

if (MSVC)
	add_definitions(-DJSON_DLL -DGEN_SERVICE_EXPORT)
else()
	add_definitions(-DGEN_SERVICE_API_EMPTY)
endif()

target_include_directories(${PROJECT_NAME}
	PUBLIC include
)

target_link_libraries(${PROJECT_NAME}
	TBS
	PocoFoundation
	PocoNet
	PocoCrypto
	PocoNetSSL
	mosquitto
	mosquittopp
)

    
    target_link_libraries(${PROJECT_NAME}
    	dbus-1
    )
    


installDefault(${PROJECT_NAME} lib lib)
#installSpecific(${PROJECT_NAME} . Synchronizer)

