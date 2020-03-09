#ifndef PROXYTASKER_HPP
#define PROXYTASKER_HPP

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <iostream>
#include <atomic>

using namespace boost::asio;
using namespace std;

const unsigned BUFFER_SIZE = 8192;

class ProxyHandler : public boost::enable_shared_from_this<ProxyHandler> {
public:
    ProxyHandler(io_context &ioContext, string destIp, unsigned port);
    ProxyHandler(const ProxyHandler& orig) = delete;
    ProxyHandler operator = (ProxyHandler orig) = delete;
    virtual ~ProxyHandler();
    void readStart();
    void asyncConnect();
    inline ip::tcp::socket& getServerSocket() { return serverSocket; }
    static atomic_ullong serverDataSize;
    static atomic_ullong clientDataSize;   
    
private:
    unsigned char serverBuffer[BUFFER_SIZE];
    unsigned char clientBuffer[BUFFER_SIZE];
    
    ip::tcp::socket serverSocket;
    ip::tcp::socket clientSocket;
    string destinationIp;
    unsigned port;
    
    void readServerDataHandler(const boost::system::error_code &error, unsigned size);
    void readClientDataHandler(const boost::system::error_code &error, unsigned size);
    void writeServerDataHandler(const boost::system::error_code &error);
    void writeClientDataHandler(const boost::system::error_code &error);
    
};

#endif /* PROXYTASKER_HPP */
