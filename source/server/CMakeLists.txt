add_library(
    serverpool
    ServerPool.cpp
    ServerPoolFactory.cpp
    StaticController.cpp
    StaticControllerFactory.cpp
  )

#target_link_libraries(
#    serverpool
#    PUBLIC
#    proxygen::proxygen
#    proxygen::proxygenhttpserver
#)
