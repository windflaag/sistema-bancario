#include "server/ServerPoolFactory.hpp"
#include "custom/Config.hpp"
#include "singleton/Singleton.hpp"
#include "database/Database.hpp"

void init_folly (int argc, char **argv) {
    folly::init(&argc, &argv, true);
}

int main(int argc, char **argv) {
    init_folly(argc, argv);

    // read configuration file
    singleton::instance()
      ->attachConfig(new custom::Config("config.json"));

    // create server pool
    singleton::instance()
      ->attachServerPool(server::ServerPoolFactory::fromSingletonConfig());

    // init UUIDv4 Generator
    singleton::instance()
      ->attachRandomizer(
        new std::mt19937_64          
    );
    
    // init database if not exists already
    try {
        database::Database::createIfNotExists();
    } catch(std::runtime_error &err) {
        std::cerr << "FATAL: Cannot create database, " << err.what();
        return 1;
    }

    // finally start server
    return singleton::instance()->getServerPool()->run();
}
