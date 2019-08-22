all:client server
client:Client.cpp
	g++ -std=c++11 $^ -o $@ -lboost_filesystem -lboost_system -lpthread -lboost_thread
server:Server.cpp
	g++ -std=c++11 $^ -o $@ -lboost_filesystem -lboost_system -lpthread
