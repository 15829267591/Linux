#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	struct ifaddrs* addrs;
	getifaddrs(&addrs);
	while(addrs != NULL){
		
		addrs = addrs->ifa->next;
	}
	return 0
	
}
