#ifndef __SINGLETON__
#define __SINGLETON__

#include "../custom/Config.hpp"
#include "../server/ServerPool.hpp"
#include <random>

namespace singleton {
    class Singleton {
        private:
            custom::Config* configuration = NULL;
            server::ServerPool* serverpool = NULL;
            std::mt19937_64* randomizer = NULL;
        
        public:
            static Singleton* internal;

            Singleton();
            ~Singleton();

            void attachConfig(custom::Config* configuration);
            void attachServerPool(server::ServerPool* serverpool);
            void attachRandomizer(std::mt19937_64* randomizer);

            custom::Config* getConfig();
            server::ServerPool* getServerPool();
            std::mt19937_64* getRandomizer();
    };

    Singleton* instance();
}

#endif
