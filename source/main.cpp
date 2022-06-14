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

    // init database if not exists already
    database::createIfNotExists();

    // start server
    singleton::instance()->getServerPool()->run();
    return 0;
}
