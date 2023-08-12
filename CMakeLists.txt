cmake_minimum_required(VERSION 3.22)
project(ENetLANChatServer C)

set (DEPS_DIR "deps")
set (SRC_DIR "src")

add_library(log ${DEPS_DIR}/log/log.c)
include_directories(${DEPS_DIR}/log)

if(MSVC)
	add_definitions(-W4 -WX -wd"4996" -wd"4204")
endif()

# Add ENet
set(ENet_LIBRARY enet)
if(WIN32)
    set(WINDOWS_ENET_DEPENDENCIES "ws2_32;winmm")
    set(ENet_LIBRARIES ${ENet_LIBRARY} ${WINDOWS_ENET_DEPENDENCIES})
else()
    set(ENet_LIBRARIES ${ENet_LIBRARY})
endif()
include_directories(${DEPS_DIR}/enet/include)
add_subdirectory(${DEPS_DIR}/enet)


set (COMMON_FILES 
    ${SRC_DIR}/common.c 
)

add_executable(server ${SRC_DIR}/server/server.c ${COMMON_FILES})
target_link_libraries(server ${ENet_LIBRARIES} log)

add_executable(client ${SRC_DIR}/client/client.c ${COMMON_FILES})
target_link_libraries(client ${ENet_LIBRARIES} log)
