#include "ServerPool.hpp"

ServerPool::ServerPool(int HTTP_PORT, std::string HTTP_ADDRESS, int SERVER_THREADS) {
    this->setupAddress(HTTP_ADDRESS, HTTP_PORT);
    this->setupConcurrency(SERVER_THREADS);
    this->setupServerPool();

}

ServerPool::ServerPool(int SERVER_THREADS) {
    this->setupAddress(DEFAULT_ADDRESS, DEFAULT_PORT);
    this->setupConcurrency(SERVER_THREADS);
    this->setupServerPool();

}

ServerPool::~ServerPool() {
    if (this->serverAddresses)
        delete this->serverAddresses;
}

void ServerPool::setupAddress(std::string ADDRESS, int PORT) {
    this->serverAddresses = new std::vector<proxygen::HTTPServer::IPConfig>();
    this->serverAddresses->push_back({folly::SocketAddress(ADDRESS, PORT, true), proxygen::HTTPServer::Protocol::HTTP});
}

void ServerPool::setupConcurrency(int THREADS_NUMBER) {
    if (THREADS_NUMBER <= 0) {
        THREADS_NUMBER = sysconf(_SC_NPROCESSORS_ONLN);
        CHECK_GT(THREADS_NUMBER, 0);
    }
}

void ServerPool::setupServerPool() {
    proxygen::HTTPServerOptions options;

    // setup thread options
    options.threads = static_cast<size_t>(this->concurrency);

    // when on idle for 60 seconds stop the server, TODO: overcome this feature
    options.idleTimeout = std::chrono::milliseconds(60000);

    // setup kernel signals of "oh f*ck i need to stop my self"
    options.shutdownOn = {SIGINT, SIGTERM};

    options.enableContentCompression = false;

    // setup controllers
    options.handlerFactories =
        proxygen::RequestHandlerChain()
            .addThen<StaticControllerFactory>()
            .build();
    options.h2cEnabled = true;

    // setup funny things
    auto diskIOThreadPool = std::make_shared<folly::CPUThreadPoolExecutor>(
        this->concurrency,
        std::make_shared<folly::NamedThreadFactory>("StaticDiskIOThread"));
    
    // i'm goin' to use this commented function call
    // folly::setUnsafeMutableGlobalCPUExecutor(diskIOThreadPool);
    folly::setCPUExecutor(diskIOThreadPool);

    // finally build the server
    this->server = new proxygen::HTTPServer(std::move(options));
    this->server->bind(*serverAddresses);

}

void ServerPool::run() {
    std::thread t([&]() { this->server->start(); });
    t.join();
}
