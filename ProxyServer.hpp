#ifndef PROXYSERVER_HPP
#define PROXYSERVER_HPP

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/system/error_code.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

#include "ProxyHandler.hpp"

using namespace boost::asio;
using namespace std;

const unsigned CPU_MULTIPLIER = 2;

class ProxyServer {
public:
    ProxyServer(string sourceIpAddress, unsigned sourcePortNum, string destIpAddress, unsigned destPortNum);
    ProxyServer(const ProxyServer& orig) = delete;
    ProxyServer operator = (ProxyServer orig) = delete;
    virtual ~ProxyServer();
    void start();
private:
    vector<boost::thread> threads;
    io_context ioContext;
    unsigned sourcePort;
	unsigned destPort;
    string sourceIp;
    string destIp;
    unsigned threadsCount;     
    ip::tcp::acceptor acptr;

    void asyncHandler(const boost::system::error_code &error, boost::shared_ptr<ProxyHandler> handler);
    void startAcceptor();
    void signalHandler(const boost::system::error_code &error, int signal);
};

#endif /* PROXYSERVER_HPP */

