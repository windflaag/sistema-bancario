#include "Singleton.hpp"

singleton::Singleton::Singleton() {
  // 
}

singleton::Singleton::~Singleton() {
  if (this->configuration)
    delete this->configuration;
  if (this->serverpool)
    delete this->serverpool;
  if (this->randomizer)
    delete this->randomizer;
}

void singleton::Singleton::attachConfig(custom::Config* configuration) {
  if (this->configuration)
    delete this->configuration;
  this->configuration = configuration;
}

void singleton::Singleton::attachLogger(std::shared_ptr<logging::Logger> logger) {
  this->logger = logger;
}

void singleton::Singleton::attachServerPool(server::ServerPool* serverpool) {
  if (this->serverpool)
    delete this->serverpool;
  this->serverpool = serverpool;
}

void singleton::Singleton::attachRandomizer(std::mt19937_64* randomizer) {
  if (this->randomizer)
    delete this->randomizer;
  this->randomizer = randomizer;
}

custom::Config* singleton::Singleton::getConfig() {
  return this->configuration;
}

server::ServerPool* singleton::Singleton::getServerPool() {
  return this->serverpool;
}

std::mt19937_64* singleton::Singleton::getRandomizer() {
  return this->randomizer;
}

std::shared_ptr<logging::Logger> singleton::Singleton::getLogger() {
  return this->logger;
}

singleton::Singleton* singleton::instance() {
  return singleton::Singleton::internal;
}

singleton::Singleton* singleton::Singleton::internal = new singleton::Singleton();


