#pragma once
#include <iostream>
#include "Log.hpp"
#include <vector>
#include <string>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "UserManager.hpp"
#include <sstream>
#include "json/json.h"
#define BACKLOG 5
#define MESSAGE_SIZE 1024
class Request{
		public:
				std::string method; //R LIN  LOUT
				std::string content_length;//
				std::string blank;//空格的作用，如果以后要增加，可以知道什么时候结束？
				std::string text;//为什么接收方也要构造一个request
		public:
				Request():blank("\n")
			    {}
				~Request()
				{}


};
class Util{
	public:
		static bool RegisterEnter(std::string &n_,std::string &s_,std::string &passwd)
		{
			std::cout << "Please Enter Nick Name: ";
			std::cin >> n_;
			std::cout << "Please Enter School: ";
		    std::cin>> s_;
			std::cout << "Please Enter Passwd: ";
			std::cin >> passwd;
			std::string again;
			std::cout << "Please Enter Passwd Again: ";
			std::cin >> passwd;
			if(passwd == again)
			{
				return true;
	    	}
				return false;
			}
		static bool LoginEnter(unsigned int &id, std::string &passwd)
			{
				std::cout <<"Please Enter Your ID";
				std::cin >> id;
				std::cout <<"Please Enter Your Passwd: ";
				std:: cin >> passwd;
			}
			static void Seralizer(Json::Value &root,std::string outString)
			{
				Json::FastWriter w;
				outString = w.write(root);
			}
			static void UnSeralizer(std::string inString,Json::Value & root)
			{
					Json::Reader r;
					r.parse(inString,root,false);
			}
			static std::string IntToString(int x)
			{
				std::stringstream ss;
				ss << x;
				return ss.str();
			}
			static int StringToInt(std::string str)
			{
					int x;
					std::stringstream ss(str);
					ss >> x;
					return x;
 			}
			static void RecvOneLine(int sock, std::string outString)
			{
					char c = 'x'; //dingyi cheng qita de keyima ?
					ssize_t s = recv(sock, &c, 1, 0); // weishenme yong s jiehsou na outSring shi shenme ?
					while(c != '\0')
					{
						if(s > 0)
						{
						if(c == '\n'){ //wei shen me zhege difanghaiyao zai panduanyci 
							break;
						}
						outString.push_back(c);
						}
						else{
							break;
						}
					}
				}
			static void RecvRequest(int sock, Request& rq)
			{
					RecvOneLine(sock,rq.method);
					RecvOneLine(sock,rq.content_length);
					RecvOneLine(sock,rq.blank);

					std::string &cl = rq.content_length;
					std::size_t pos = cl.find(": ");
					if(std::string::npos == pos)
					{
						return;
					}
					std::string sub = cl.substr(pos+2);
					int size = StringToInt(sub);
					char c;
					for(auto i = 0; i < size; i++)
					{
						recv(sock,&c,1,0); //zheli de sock ? cong sock du dao czhong 
						(rq.text).push_back(c);
					}
			}

			static void SendRequest(int tcp_sock, Request &rq)
			{
					std::string &m_ = rq.method;
					std::string &cl_ = rq.content_length;
					std::string &text_ = rq.text;
					std::string &b_ = rq.blank;
					send(tcp_sock, m_.c_str(), m_.size(), 0);
					send(tcp_sock, cl_.c_str(), cl_.size(), 0);
					send(tcp_sock, b_.c_str(), b_.size(), 0);
					send(tcp_sock, text_.c_str(), text_.size(), 0);

			}
			//UDP 
			static void RecvMessage(int sock, std::string &message, struct sockaddr_in &peer)
			{
					char msg[MESSAGE_SIZE];
					socklen_t len = sizeof(peer);
					ssize_t s = recvfrom(sock, msg, sizeof(msg)-1, 0,(struct sockaddr*)&peer, &len);
					if(s <= 0)
					{
						LOG("recvfrom message error",WARNING);
					}
					else{
							message = msg;
					}
			}

			static void SendMessage(int sock, const std::string &message, struct sockaddr_in &peer)
			{
					sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr*)&peer, sizeof(peer));
			}
            static void addUser(std::vector<std::string>&online, std::string &f)
			{	
				for(auto it = online.begin(); it != online.end(); it++){
				if(*it == f){
					return;
			    	}
		    	}
				online.push_back(f);
	    	}
};

class SocketApi{
		public:
				static int Socket(int type)
				{
						int sock = socket(AF_INET,type ,0);
						if(sock < 0)
						{
								LOG("socked error!",ERROR);
								exit(2);
						}
				}
				static void Bind(int sock, int port)
				{
						struct sockaddr_in local;
						local.sin_family = AF_INET;
						local.sin_addr.s_addr = htonl(INADDR_ANY);
						local.sin_port = htons(port);

						if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
						{
								LOG("socket error",ERROR);
								exit(3);
						}
				}
				static void Listen(int sock)
				{
						if(listen(sock,BACKLOG) < 0)
						{
								LOG("Listen error",ERROR);
								exit(4);
						}
				}
				static int Accept(int listen_sock, std::string &out_ip,int &out_port)
				{
						struct sockaddr_in peer;
						socklen_t len = sizeof(peer);
						int sock = accept(listen_sock,(struct sockaddr*)&peer, &len);
						if(sock < 0){
								LOG("ACCEPT error",WARNING);
								return -1;
						}
						out_ip = inet_ntoa(peer.sin_addr);
						out_port = htons(peer.sin_port);
						return sock;

				}
				static bool Connect(const int &sock, std::string peer_ip,const int &port)
				{
						struct sockaddr_in peer;
						peer.sin_family = AF_INET;
						peer.sin_addr.s_addr = inet_addr(peer_ip.c_str());
						peer.sin_port = htons(port);

						if(connect(sock,(struct sockaddr*)&peer,sizeof(peer)) < 0)
						{
								LOG("connect error",WARNING);
								return false;
						}
						return true;
				}

};

