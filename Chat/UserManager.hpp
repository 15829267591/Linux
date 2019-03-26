#pragma once
//用户管理
//首先得有用户信息，写一个User,简化属性，有一张用户表
#include <map>
#include <iostream>
#include <unordered_map>
#include <pthread.h>
#include <string>
class User{
    private:
	std::string nick_name;
	std::string school;
	std::string passwd;
    public:		    //为什么要用引用
		User(){}
		User(const std::string &n_,const std::string &s_,\
		const std::string pwd_):
		nick_name(n_),school(s_),passwd(pwd_)
		{}		
	bool IsPasswdOk(const std::string &passwd_)
	{
		return passwd == passwd_ ? true : false;
	}
	std::string &GetNickName()
	{
		return nick_name;
	}
	std::string &GetSchool()
	{
		return school;
	}
	~User()
	{}
};

class UserManager{
    private:
	unsigned int assign_id;//是要给用户分配的
	std::unordered_map<unsigned int, User> users;
	std::unordered_map<unsigned int,struct sockaddr_in>online_users; //不是所有人在线，在线才能收到消息。所以要提供在线用户列表，将来给服务线程用
	pthread_mutex_t lock;
	void Lock()
	{	
		pthread_mutex_lock(&lock);
	}
	void Unlock()
	{
		pthread_mutex_unlock(&lock);
	}
    public:
	UserManager():assign_id(10000)
	{
		pthread_mutex_init(&lock,NULL);
	}
	unsigned int Insert(const std::string &n_,const std::string &s_,\
						const std::string &p_)
	{
		Lock();
		unsigned int id = assign_id++;
		User u(n_, s_, p_);
		//插入前最好进行一下校验，但unsigned 42yi, 所以基本不会溢出
		//严谨一点，最好检验
		if(users.find(id) == users.end())
		{
			users.insert(std::make_pair(id, u));
			//users.insert({id, u});
		    Unlock();
			return id;
		}
		Unlock();
		return 1;
	}
	unsigned int Check(const unsigned int &id, const std::string &passwd)
	{
		Lock();
		auto it = users.find(id);
		if(it != users.end())
		{
			User &u = it->second;
			if(u.IsPasswdOk(passwd))
			{
				Unlock();
				return id;
			}
		}
		Unlock();
		return 2;
	}
	void AddOnlineUser(unsigned int id, sockaddr_in peer)
	{
		Lock();
		auto it = online_users.find(id);
		if(it == online_users.end())
		{
			online_users.insert({id, peer});
		}
		Unlock();
	}
	
	void GetUserInfo(const unsigned int &id, std::string &name_,\
					std::string &school_)
	{
		Lock();
		name_ = users[id].GetNickName();
		school_ = users[id].GetSchool();
		Unlock();
	}
	std::unordered_map<unsigned int, struct sockaddr_in>OnlineUser()
	{
		Lock();
		auto online = online_users;
		Unlock();
		return online;
	}
	~UserManager()
	{
		pthread_mutex_destroy(&lock);
	}
};




















