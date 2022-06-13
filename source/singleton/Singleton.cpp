#include "Singleton.hpp"

singleton::Singleton::Singleton() {
    // 
}

singleton::Singleton::~Singleton() {
    if (this->configuration)
        delete this->configuration;
    if (this->serverpool)
        delete this->serverpool;
}

void singleton::Singleton::attachConfig(custom::Config* configuration) {
    if (this->configuration)
        delete this->configuration;
    this->configuration = configuration;
}

void singleton::Singleton::attachServerPool(server::ServerPool* serverpool) {
    if (this->serverpool)
        delete this->serverpool;
    this->serverpool = serverpool;
}

custom::Config* singleton::Singleton::getConfig() {
    return this->configuration;
}

server::ServerPool* singleton::Singleton::getServerPool() {
    return this->serverpool;
}

singleton::Singleton* singleton::instance() {
    return singleton::Singleton::internal;
}

singleton::Singleton* singleton::Singleton::internal = new singleton::Singleton();
