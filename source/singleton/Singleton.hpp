#ifndef __SINGLETON__
#define __SINGLETON__

#include "../custom/Config.hpp"
#include "../server/ServerPool.hpp"

namespace singleton {
    class Singleton {
        private:
            custom::Config* configuration = NULL;
            server::ServerPool* serverpool = NULL;
        
        public:
            static Singleton* internal;

            Singleton();
            ~Singleton();

            void attachConfig(custom::Config* configuration);
            void attachServerPool(server::ServerPool* serverpool);

            custom::Config* getConfig();
            server::ServerPool* getServerPool();
    };

    Singleton* instance();
}

#endif
