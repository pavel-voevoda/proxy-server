#include <boost/lexical_cast.hpp>
#include <iostream>

#include "ProxyServer.hpp"

int main(int argc, char** argv) {
    if(argc !=5) {
        std::cout << "Please, check arguments: proxy-server <source ip> <port> <destination ip> <port>" << std::endl;
        return -1;
    }
	std::string sourceIp = argv[1];
    unsigned sourcePort = boost::lexical_cast<int>(argv[2]);
    std::string destIp = argv[3];
    unsigned destPort = boost::lexical_cast<int>(argv[4]);
    ProxyServer server(sourceIp, sourcePort, destIp, destPort);
    server.start();

    return 0;
}
