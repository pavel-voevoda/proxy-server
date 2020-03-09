CC=g++
CFLAGS=-c -Wall -pedantic -Werror -O3 -std=c++14

all: proxy-server

proxy-server: ProxyHandler.o ProxyServer.o main.o 
	$(CC) ProxyHandler.o ProxyServer.o main.o  -o proxy-server -lboost_thread -lpthread

ProxyHandler.o: ProxyHandler.cpp
	$(CC) $(CFLAGS) ProxyHandler.cpp

ProxyServer.o: ProxyServer.cpp
	$(CC) $(CFLAGS) ProxyServer.cpp
	
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm -rf *.o proxy-server
