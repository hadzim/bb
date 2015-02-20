cmake_minimum_required(VERSION 3.0)
project(TBS.Services)

file(GLOB_RECURSE ${PROJECT_NAME}.Sources "src/*.cpp")
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}.Sources})

target_include_directories(${PROJECT_NAME}
	PUBLIC include
)

target_link_libraries(${PROJECT_NAME}
	TBS
	PocoFoundation
  PocoNet
  PocoCrypto
	PocoNetSSL
)

if(${TargetDevice} STREQUAL "win")
    
    target_link_libraries(${PROJECT_NAME}
    
    #### SSL libs
    
    ${SSL_LIBS}  
    
    #### serial line libs
    setupapi 
    advapi32
    )
    
else()
    
    target_link_libraries(${PROJECT_NAME}
    	dbus-1
    )
    
endif()


installDefault(${PROJECT_NAME} lib)