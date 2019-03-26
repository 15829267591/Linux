#pragma once

class SockApi{
	
	public:
		static int Sock(int type)
        int sock = socket(AF_INET, type, 0);
		if(sock < 0)
		{
			std::cout << Sock Errno << std::endl;
		}
		static int Bind(int sock, int port)
		{
			struct sockaddr_in local;
			local.sin_family = AF_INET;
			locat.sin
		}
};
