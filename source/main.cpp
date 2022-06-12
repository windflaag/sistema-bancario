#include "server/ServerPool.hpp"

DEFINE_int32(HTTP_PORT, 8080, "Porta su cui il server usa HTTP");
DEFINE_string(SERVER_ADDRESS, "0.0.0.0", "IP o Hostname a cui il server si aggancia");
DEFINE_int32(SERVER_THREADS, 16,
    "concorenza in thread della server pool"
    "con un valore <= 0 si usera' il numero di thread del computer");

int main(int argc, char* argv[]) {
    folly::init(&argc, &argv, true);
    
    std::printf("creating server pool ...");
    ServerPool* pool = new ServerPool(FLAGS_HTTP_PORT, FLAGS_SERVER_ADDRESS, FLAGS_SERVER_THREADS);
    std::printf("done\n");
    pool->run();
    
    return 0;
}
