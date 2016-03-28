cmake_minimum_required(VERSION 3.0)


# C++11 check
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()




# Target device
set(TargetDevice "linpc" CACHE STRING "Build type")
set_property(CACHE TargetDevice PROPERTY STRINGS linpc tpro win RPi)


macro(installDefault)
install(
	TARGETS ${ARGV0}
	DESTINATION Default/${ARGV1}
	COMPONENT Default
)
endmacro()

macro(servicefile)
install(
	DIRECTORY . DESTINATION Default/services FILES_MATCHING PATTERN "*.service"
)
endmacro()

if(${TargetDevice} STREQUAL "linpc")

	add_definitions(-D__UNIX__)
	message(STATUS "Targeting linpc")

elseif(${TargetDevice} STREQUAL "tpro")

	add_definitions(-D__UNIX__ -DTBS_DEVICE -DTBS_TPRO)
	message(STATUS "Targeting tpro")

elseif(${TargetDevice} STREQUAL "win")

	add_definitions(-DTBS_WIN -D_WIN32)
	message(STATUS "Targeting win32")

elseif(${TargetDevice} STREQUAL "RPi")
	
	add_definitions(-D__UNIX__ -DTBS_DEVICE)
	message(STATUS "Targeting RPi")
	
endif()

# Subdirectories


#add_subdirectory(libwebsockets)
#add_subdirectory(mosquitto-1.4)
#add_subdirectory(BB.MQTT2WS2)

add_subdirectory(TBS)
add_subdirectory(TBS.Services)

add_subdirectory(BB.Mosquitto)

add_subdirectory(BB.WWW)

add_subdirectory(BB.Services)
add_subdirectory(BB)


add_subdirectory(BB.Collector)
add_subdirectory(BB.Dump)
add_subdirectory(BB.Forecast)
add_subdirectory(BB.HttpServer)
add_subdirectory(BB.Motion)
add_subdirectory(BB.RemoteClient)
add_subdirectory(BB.SerialCollector)
add_subdirectory(BB.Status)
add_subdirectory(BB.Test)
add_subdirectory(BB.History)

add_subdirectory(BB.Actions)
add_subdirectory(BB.Network)
add_subdirectory(BB.Time)


add_subdirectory(BB.Temperature)
add_subdirectory(BB.New.Dummy)
add_subdirectory(BB.New.Arduino)
add_subdirectory(BB.Rules)

add_subdirectory(BB.Master)
add_subdirectory(BB.HW)
add_subdirectory(BB.Relay)
add_subdirectory(BB.Camera)

add_subdirectory(BB.PWM)

add_subdirectory(www)

install(
	DIRECTORY Build/helpers/ 
	DESTINATION Default 
	COMPONENT Default
	FILES_MATCHING PATTERN "*.sh"
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
	            GROUP_EXECUTE GROUP_READ
)



