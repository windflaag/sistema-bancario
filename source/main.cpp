#include "server/ServerPoolFactory.hpp"
#include "custom/Config.hpp"
#include "singleton/Singleton.hpp"

void init_folly (int argc, char **argv) {
    folly::init(&argc, &argv, true);
}

int main(int argc, char **argv) {
    init_folly(argc, argv);

    singleton::instance()->attachConfig(new custom::Config("config.json"));
    singleton::instance()->attachServerPool(
        server::ServerPoolFactory::fromSingletonConfig());
    
    singleton::instance()->getServerPool()->run();
    return 0;
}
