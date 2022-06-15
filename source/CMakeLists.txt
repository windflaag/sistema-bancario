cmake_minimum_required(VERSION 3.10)

project(SistemaBancario)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

set(CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
  "/usr/local/lib/cmake/proxygen"
  "/lib/cmake/proxygen"
  )

find_package(proxygen REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(gflags REQUIRED)

message(STATUS "proxygen lib path: ${proxygen_INCLUDE_DIRECTORIES}")

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

file(GLOB_RECURSE MY_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
message(STATUS "source files: ${MY_SOURCE_FILES}")
message(STATUS "pwd: ${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(main ${MY_SOURCE_FILES})

target_link_libraries(
    main PUBLIC sqlite3 ssl proxygen::proxygen proxygen::proxygenhttpserver jsoncpp)
