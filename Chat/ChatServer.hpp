#pragma once
#include <iostream>
#include "ProtocolUtil.hpp"
#include <pthread.h>
#include "UserManager.hpp"
#include "DataPool.hpp"
#include "Log.hpp"
#include "Message.hpp"
class ChatServer;
class Param{
    public:

    ChatServer *sp;
    int sock;
	std::string ip;
	int port;
    public:
	Param(ChatServer *sp_,int &sock_, const std::string &ip_, const int port_)
	      :sp(sp_),sock(sock_),ip(ip_),port(port_)
	{}
	~Param()
	{}
};

class ChatServer{
    private:
	int tcp_listen_sock;
	int tcp_port;
	int udp_work_sock;
	int udp_port;

	UserManager um;
	DataPool pool;
    public:
    ChatServer(int tcp_port_ = 8080,int udp_port_ = 8888):
    tcp_port(tcp_port_),
    tcp_listen_sock(-1),
    udp_port(udp_port_),
    udp_work_sock(-1)
    {}
    void InitServer()
    {
	tcp_listen_sock = SocketApi::Socket(SOCK_STREAM);	
	udp_work_sock = SocketApi::Socket(SOCK_DGRAM);
	SocketApi::Bind(tcp_listen_sock,tcp_port);
	SocketApi::Bind(udp_work_sock,udp_port);

	SocketApi::Listen(tcp_listen_sock);

	pthread_t tid;
    }
    
    unsigned int RegisterUser(const std::string &name, const std::string &school, const std::string &passwd)
	{
		return um.Insert(name, school, passwd);	
	}
	unsigned int LoginUser(const unsigned int &id, const std::string &passwd,\
							const std::string &ip, int port)
	{
		return um.Check(id, passwd);
		//unsigned int result = un.Check(id, passwd);
		//if(result >= 10000){
		//	un.MoveToOnline(id, ip, port);
		//}
		//return result;
	}
	//UDP 开始
	void Product()
	{
		std::string message;
		struct sockaddr_in peer;
		Util::RecvMessage(udp_work_sock, message,peer);
		std::cout << "debug: recv message" << message << std::endl;
		if(!message.empty())
		{
			
			Message m;
			m.ToRecvValue(message);
			if(m.Type() ==LOGIN_TYPE){
				
				um.AddOnlineUser(m.Id(), peer);
				std::string name_,school_;
				um.GetUserInfo(m.Id(), name_, school_);
				Message new_msg(name_, school_, m.Text(), m.Id(), m.Type());
				new_msg.ToSendString(message);
			}
			
			pool.PutMessage(message);

		}
       
	}

	void Consume()
	{
		std::string message;
		pool.GetMessage(message);
		//转发   给所有的online用户
		std::cout << "debug: send message" << message << std::endl;
		auto online = um.OnlineUser();
		for(auto it = online.begin(); it != online.end(); it++)
		{
			Util::SendMessage(udp_work_sock, message, it->second);
		}
		
	}
    static void *HandlerRequest(void*arg)  //static  表明函数内部不需要this指针 也就是arg是void*的
    {
		Param* p = (Param*)arg; //这个函数是创建就是为了处理，处理就要有socket，从socket先把数据都出来，读完之后在处理，处理完之后再修改this，或者server当中的用户信息，所以也要把server传进去，所以构建结构体
		int sock = p->sock;
		ChatServer *sp = p->sp;
		std::string ip = p->ip;
		int port = p->port;
		delete p;
		pthread_detach(pthread_self());
		//  haiyou yizhongxiefa shouji zhong de zhaopai 
		Request rq;                                                
		Util::RecvRequest(sock, rq); //client fa yige request zhe bian jieshou ranhou chuli 	
		Json::Value root;
		LOG(rq.text, NORMAL);
		Util::UnSeralizer(rq.text,root);
		if(rq.method == "REGISTER")
		{
	    
	    	std::string name =root["name"].asString();
	    	std::string school = root["school"].asString();
	    	std::string passwd = root["passwd"].asString();
        	unsigned int id = sp->RegisterUser(name, school, passwd);
			send(sock, &id, sizeof(id), 0);//已经拿到ID，响应 id 响应字节

		}else if(rq.method == "LOGIN"){
		unsigned int id = root["id"].asInt();
		std::string passwd = root["passwd"].asString();
		//check, move user to online
		unsigned int result = sp->LoginUser(id, passwd, ip, port);//用户进行登陆 
		send(sock, &result, sizeof(result), 0); //响应
		}else{
				//logout
		}



		//recv:sock
		//肯定有协议 any && handler fenxi chuli
		//response
	
	close(sock);
    }
    void Start()
    {

	std::string ip;
	int port;
	for(;;) //主线程上面的UserManager删除的那个 .主线程完成注册登陆
	{
	    int sock = SocketApi::Accept(tcp_listen_sock,ip, port);
	    if(sock > 0)
	    {
			std::cout << "get a new client" << ip << ":" << port << std::endl;
			Param *p = new Param(this,sock, ip, port);
            pthread_t tid;
	      	pthread_create(&tid,NULL,HandlerRequest,p);


	    }
	}
    }
    ~ChatServer()
    {}
};
