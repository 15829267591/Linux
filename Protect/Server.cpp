#include "httplib.h"
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <string>

using namespace httplib;
namespace bf = boost::filesystem;

#define SHARED_PATH "Shared"
#define LOG(...) std << cout << __VA_ARGS__
class P2PServer{
		private:
				Server _server;
		private:
				static void GetHostPair(const Request &req, Response & rsp)
				{
						rsp.status = 200;
				}
				static void GetFileList(const Request &req, Response &rsp)
				{
						bf::directory_iterator item_begin(SHARED_PATH);
						bf::directory_iterator item_end;
						std::string body;
						for(; item_begin != item_end; ++item_begin){
								//如果是目录 ，不处理
								if(bf::is_directory(item_begin->status())){
										continue;
								}
								//
								std::string path = item_begin->path().string();
								std::string filename = item_begin->path().filename().string();
								std::cerr << path <<std::endl;
								body += path + "\n";
						}
						rsp.status = 200;
						//rsp.set_content()
				}
				static void GetFileData(const Request &req, Response &rsp)
				{
						bf::path path(req.path);
						std::stringstream name;
						name <<  SHARED_PATH << "/" << path.filename().string();

						if(!bf::exists(name.str())){
								rsp.status = 404;
								return;
						}
						if(bf::is_directory(name.str())){

								rsp.status = 403;
								return;
						}
						int64_t fsize = bf::file_size(name.str()); //获取文件大小
						if(req.method == "HEAD"){
								rsp.status = 200;
								std::string len = std::to_string(fsize);
								rsp.set_header("Content-Length", len.c_str());
								return;
						}
						else{
								if(!req.has_header("Range")){
									rsp.status = 400;
									return;
								}
								std::string range_val;
								range_val ==req.get_header_value("Range");
								int64_t start, end, rlen;
								bool ret = RangeParse(range_val, start, rlen);
								if(ret == false){
									rsp.status = 400;
									return;
								}
								std::ifstream file(name.str(), std::ios::binary);//以二进制方打开
								//判断是否打开
								if(!file.is_open()){
										std::cerr << "open file" << name.str() << "failed\n";
										rsp.status = 404;
										return;
								}
								file.seekg(start, std::ios::beg);
								rsp.body.resize(rlen);
								//读取数据，首地址， 大小
								file.read(&rsp.body[0], rlen);
								if(!file.good()){
										std::cerr << "read file "<< name.str() << "body errno\n";
										rsp.status = 500;
										return;
								}
								file.close();
								rsp.status = 200;
								//HTTP/1/1 200 OK
								//Content-length 
								rsp.set_header("Content-Type", "application/octet-stream");
						}
				}
				static bool RangeParse(std::string& range_val, int64_t &start, int64_t &len){
					
								size_t pos1 = range_val.find("=");
								size_t pos2 = range_val.find("-");
								if(pos1 == std::string:: npos || pos2 == std::string::npos){
									std::cerr << "range" << range_val << "format error\n";
									return false;
								}
								int64_t end;
								std::string rstart;
								std::string rend;
								rstart = range_val.substr(pos1+1, pos2-1-pos1);
								rend = range_val.substr(pos2 +1);
								std::stringstream tmp;
								tmp << rstart;
								tmp >> start;
								tmp.clear();
								tmp << rend;
								tmp >> end;
								len = end -start +1;
								return true;
				}
		public:
				P2PServer(){
						if(!bf::exists(SHARED_PATH)){
								bf::create_directory(SHARED_PATH);
						}
				}

				bool	Start(uint16_t port){
						_server.Get("/hostpair", GetHostPair);
						_server.Get("/list", GetFileList);
						_server.Get("/list/(.*)", GetFileData);
						_server.listen("0.0.0.0", 9000);
				}
};


int main()
{
		P2PServer srv;
		srv.Start(9000);
		return 0;
}
