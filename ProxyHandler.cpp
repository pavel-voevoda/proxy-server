#include "ProxyHandler.hpp"

atomic_ullong ProxyHandler::serverDataSize(0);
atomic_ullong ProxyHandler::clientDataSize(0);

ProxyHandler::ProxyHandler(io_context &ioContext, string destIp, unsigned port) : 
    serverSocket(ioContext), clientSocket(ioContext), destinationIp(destIp), port(port) {
}

ProxyHandler::~ProxyHandler() {
    serverSocket.close();
    clientSocket.close();
}

//preparing to get data from server/client
void ProxyHandler::readStart() {
    clientSocket.async_read_some(boost::asio::buffer(clientBuffer),
        boost::bind(&ProxyHandler::readClientDataHandler, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
     
    serverSocket.async_read_some(boost::asio::buffer(serverBuffer),
        boost::bind(&ProxyHandler::readServerDataHandler, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
 }


void ProxyHandler::asyncConnect() {
    clientSocket.async_connect(ip::tcp::endpoint(
        ip::address::from_string(destinationIp), port),
        boost::bind(&ProxyHandler::readStart, shared_from_this())
    );
}

//send data from server to client
void ProxyHandler::readServerDataHandler(const boost::system::error_code &error, unsigned size) {
    if(error){
        return;
    }
    ProxyHandler::serverDataSize += size;
    boost::asio::async_write(clientSocket, 
        boost::asio::buffer(serverBuffer, size),
        boost::bind(&ProxyHandler::writeServerDataHandler, shared_from_this(),
        boost::asio::placeholders::error));     
}

//server sent data and is waiting for confirmation
void ProxyHandler::writeServerDataHandler( const boost::system::error_code &error) {
    if(error) {
        return;
    }
    serverSocket.async_read_some(
        boost::asio::buffer(serverBuffer, BUFFER_SIZE),
        boost::bind(&ProxyHandler::readServerDataHandler, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));      
}
 
//client accepts that data were get and sends confirmation to server
void ProxyHandler::readClientDataHandler(const boost::system::error_code &error, unsigned size) {
    if(error) {
        return;
    }
    ProxyHandler::clientDataSize += size;
    boost::asio::async_write(serverSocket, 
        boost::asio::buffer(clientBuffer, size),
        boost::bind(&ProxyHandler::writeClientDataHandler, shared_from_this(),
            boost::asio::placeholders::error));
}
 
//client is waiting to get confirmation from server
void ProxyHandler::writeClientDataHandler(const boost::system::error_code &error){
    if(error) {
       return;
    }
    clientSocket.async_read_some(
        boost::asio::buffer(clientBuffer, BUFFER_SIZE),
        boost::bind(&ProxyHandler::readClientDataHandler, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
 }

 