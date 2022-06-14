cmake_minimum_required(VERSION 3.10)

project(SistemaBancario)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

add_executable(main
    ./singleton/Singleton.cpp
    ./politik/ControllerFactory.cpp
    ./politik/Detection.cpp
    ./utility/Utility.cpp
    ./rest/RestEndpointController.cpp
    ./rest/ApiTransferController.cpp
    ./rest/ApiAccountController.cpp
    ./rest/WrongPathController.cpp
    ./rest/Validation.cpp
    ./static_engine/StaticController.cpp
    ./static_engine/Validation.cpp
    ./main.cpp
    ./server/ServerPool.cpp
    ./server/ServerPoolFactory.cpp
    ./custom/Config.cpp    
    ./database/Database.cpp
    ./codec/Codec.cpp
)

target_link_libraries(
    main PUBLIC sqlite3 ssl proxygen::proxygen proxygen::proxygenhttpserver jsoncpp)
