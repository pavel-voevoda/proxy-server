#include "ProxyServer.hpp"
#include "ProxyHandler.hpp"

ProxyServer::ProxyServer(string sourceIpAddress, unsigned sourcePortNum, string destIpAddress, unsigned destPortNum) :
    sourcePort(sourcePortNum), destPort(destPortNum), sourceIp(sourceIpAddress), destIp(destIpAddress), 
    threadsCount(boost::thread::hardware_concurrency() * CPU_MULTIPLIER),
    acptr(ioContext, ip::tcp::endpoint(ip::tcp::v4(), sourcePort)){  
}

ProxyServer::~ProxyServer() {
}

void ProxyServer::start(){
    startAcceptor();
    signal_set signal(ioContext, SIGINT, SIGTERM);
    signal.async_wait(boost::bind(&ProxyServer::signalHandler, this,
        boost::asio::placeholders::error, boost::asio::placeholders::signal_number));
    
    for(unsigned i=0; i<threadsCount; i++) {
        threads.push_back(boost::thread(boost::bind(&io_context::run, &ioContext)));
    }
    for(unsigned i=0; i<threadsCount; i++) {
        threads[i].join();
    } 
}

void ProxyServer::startAcceptor() {
    boost::shared_ptr<ProxyHandler> handler(new ProxyHandler(ioContext, destIp, destPort));
    acptr.async_accept(handler->getServerSocket(), 
        boost::bind(&ProxyServer::asyncHandler, this,
        boost::asio::placeholders::error, handler));
}

void ProxyServer::asyncHandler(const boost::system::error_code &error, boost::shared_ptr<ProxyHandler> handler) {
    cout<<"Connection from " << sourceIp << " port " << sourcePort << endl;
    handler->asyncConnect();
    startAcceptor();
}
 
void ProxyServer::signalHandler(const boost::system::error_code &error, int signal) {
    if(signal == SIGINT) {
        cout << "Press Enter to finish program" << endl;
        char c = getchar();
        while(c != '\n') {
            c = getchar();
        }
        cout << "Data from server: " << ProxyHandler::serverDataSize << " bytes" << endl;
        cout << "Data from client: " << ProxyHandler::clientDataSize << " bytes" << endl;
        exit(0);
    }
}
