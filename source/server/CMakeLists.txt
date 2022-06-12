add_library(
    serverpool
    ServerPool.cpp
    StaticController.cpp
    StaticControllerFactory.cpp
  )

target_link_libraries(
    serverpool
    PUBLIC
    proxygen::proxygen
    proxygen::proxygenhttpserver
)
