#include "json/json.h"
#include <iostream>
int main()
{
	std::string nick_name = "zhangsan";
	std::string school = "SUST";
	std::string passwd = "123456";

	Json::Value root;
	root["name"] = nick_name;
	root["school"] = school;
	root["passed"] = passwd;

	Json::FastWriter w;
	std::string s = w.write(root);
	std::cout << s << std::endl;
	
}
