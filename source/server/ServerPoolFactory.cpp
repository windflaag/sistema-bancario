#include "ServerPoolFactory.hpp"
#include "../singleton/Singleton.hpp"

server::ServerPool* server::ServerPoolFactory::fromConfig(custom::Config* configuration) {
  server::ServerPool* serverpool = NULL;
  Json::Value serverConfig = configuration->get("ServerPool");

  serverpool = new server::ServerPool (
				       serverConfig.get("serverPort", DEFAULT_PORT).asInt(),
				       serverConfig.get("serverAddress", DEFAULT_ADDRESS).asString(),
				       serverConfig.get("serverThreads", DEFAULT_THREADS).asInt()
				       );

  return serverpool;
}

server::ServerPool* server::ServerPoolFactory::fromSingletonConfig() {
  return server::ServerPoolFactory::fromConfig(
					       singleton::instance()->getConfig());
}
