cmake_minimum_required(VERSION 3.5)

project(http_redis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
endif(MSVC)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

# add log support
find_package(spdlog CONFIG REQUIRED)
INCLUDE_DIRECTORIES(${spdlog_include_dirs})

# add json support
find_package(nlohmann_json CONFIG REQUIRED)

# add enum support
find_package(magic_enum CONFIG REQUIRED)
INCLUDE_DIRECTORIES(${magic_enum_include_dirs})

find_package(task_channel CONFIG REQUIRED)
INCLUDE_DIRECTORIES(${task_channel_include_dirs})

find_package(http_utils CONFIG REQUIRED)
INCLUDE_DIRECTORIES(${http_utils_include_dirs})

find_package(hiredis CONFIG REQUIRED)
INCLUDE_DIRECTORIES(${hiredis_include_dirs})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})
link_directories(${OPENSSL_LIBRARY_DIR})


find_package(Threads REQUIRED)

find_package(Boost 1.71.0 REQUIRED COMPONENTS  system date_time)



file(GLOB_RECURSE COMMAND_SRC_FILES "${PROJECT_SOURCE_DIR}/src/command/*.cpp")
file(GLOB_RECURSE WORKER_SRC_FILES "${PROJECT_SOURCE_DIR}/src/worker/*.cpp")
file(GLOB_RECURSE SERVER_SRC_FILES "${PROJECT_SOURCE_DIR}/src/server/*.cpp")

add_library(redis_command ${COMMAND_SRC_FILES})
add_library(redis_worker ${WORKER_SRC_FILES})
add_library(redis_server ${SERVER_SRC_FILES})
target_link_libraries(redis_worker PUBLIC nlohmann_json::nlohmann_json task_channel hiredis::hiredis spdlog::spdlog)
target_link_libraries(redis_server PUBLIC  http_utils::http_server redis_worker)

add_executable(server_test ${PROJECT_SOURCE_DIR}/test/test_server.cpp)
target_link_libraries(server_test redis_server)


foreach(p LIB BIN INCLUDE)
	set(var CMAKE_INSTALL_${p}DIR)
	if(NOT IS_ABSOLUTE "${${var}}")
		string(TOLOWER ${p} lower_p)
        set(${var} "${CMAKE_INSTALL_PREFIX}/${lower_p}")
	endif()
endforeach()

export(TARGETS redis_command redis_worker redis_server 
     FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Targets.cmake)

file(WRITE
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    "include(\${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake)\n")

install(FILES
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install(TARGETS redis_command redis_worker redis_server EXPORT ${PROJECT_NAME}Targets
   ARCHIVE  DESTINATION lib
   LIBRARY DESTINATION lib
   RUNTIME DESTINATION bin
  )

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}Targets
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})