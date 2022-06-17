#include "server/ServerPoolFactory.hpp"
#include "custom/Config.hpp"
#include "singleton/Singleton.hpp"
#include "database/Database.hpp"
#include "utility/Utility.hpp"

void init_folly (int argc, char **argv) {
  folly::init(&argc, &argv, true);
}

int main(int argc, char **argv) {
  init_folly(argc, argv);

  // read configuration file
  singleton::instance()
    ->attachConfig(new custom::Config("config.json"));

  singleton::instance()
    ->attachLogger(std::make_shared<logging::Logger>());

  // create server pool
  singleton::instance()
    ->attachServerPool(server::ServerPoolFactory::fromSingletonConfig());

  // init Random Number Generator
  auto randomizer = new std::mt19937_64();
  randomizer->seed(utility::getCurrentTimeStampSeed());
  singleton::instance()->attachRandomizer(randomizer);
    
  // init database if not exists already
  try {
    database::createIfNotExists();
  } catch(std::runtime_error &err) {
    std::cerr << "FATAL: Cannot create database, " << err.what();
    return 1;
  }

  // finally start server
  return singleton::instance()->getServerPool()->run();
}
