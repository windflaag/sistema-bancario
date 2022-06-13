#ifndef __SERVER_POOL_FACTORY__
#define __SERVER_POOL_FACTORY__

#include "ServerPool.hpp"
#include "../custom/Config.hpp"

namespace server {
    class ServerPoolFactory {
        public:
            static ServerPool* fromConfig(custom::Config* configuration);
            static ServerPool* fromSingletonConfig();
    };
}

#endif
