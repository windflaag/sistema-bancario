#ifndef __SERVER_POOL__
#define __SERVER_POOL__

#include <folly/Memory.h>
#include <folly/executors/CPUThreadPoolExecutor.h>
#include <folly/executors/GlobalExecutor.h>
#include <folly/init/Init.h>
#include <folly/io/async/EventBaseManager.h>
#include <folly/portability/GFlags.h>
#include <folly/portability/Unistd.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>

#define DEFAULT_PORT 4242
#define DEFAULT_ADDRESS "0.0.0.0"
#define DEFAULT_THREADS 0

namespace server {

  class ServerPool {
  private:
    std::vector<proxygen::HTTPServer::IPConfig>* serverAddresses = NULL;
    int concurrency = 1;
    proxygen::HTTPServer* server = NULL;

    void setupAddress(std::string ADDRESS, int PORT);
    void setupConcurrency(int THREADS_NUMBER);
    void setupServerPool();
    void setup(int HTTP_PORT, std::string HTTP_ADDRESS, int SERVER_THREADS);
  public:

    ServerPool(int HTTP_PORT, std::string HTTP_ADDRESS, int SERVER_THREADS);
    ServerPool(int SERVER_THREADS);
    ~ServerPool();

    int run();
  };

}

#endif
