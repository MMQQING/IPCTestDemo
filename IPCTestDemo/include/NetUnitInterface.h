#pragma once
#include <memory>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>  
#ifdef DLL_EXPORTS_NET_BOOST
#define EXPORT_BOOST_NET _declspec(dllexport)
#else
#define EXPORT_BOOST_NET _declspec(dllimport)
#endif

#pragma warning(disable:4290)
namespace ptool{namespace exception{class ptool_exception;}}
using namespace ptool::exception;
using namespace ptool;
//using namespace boost;

namespace 
ptool{
	//对创建类型进行枚举
	//Cpc-公司内部使用，发送时固定加包头信息
	//Third_party-第三方库使用
	enum esys_type{Cpc = 0, Third_party};
	//
	namespace 
	net{
		//udp
		namespace
		udp{
			//说明：1、异步处理数据使用
			//	   2、不要处理太多操作，避免影响收数
			//p:接收数据的缓冲区
			//len：接收数据的长度
			//endpoint：接收数据的远端信息
			typedef std::function<void(char* p, std::size_t& len, boost::asio::ip::udp::endpoint& ep)> f_data_udp;
			using namespace boost::asio;
			class Iudp
				{
				public:
					virtual ~Iudp(){}
					//设置数据接收回调函数
					virtual void set_func_data(f_data_udp f) = 0;
					//设置发送目的endpoint
					virtual void set_remote_point(ip::udp::endpoint& ep) throw() = 0;
					//绑定本地接收endpoint,如果为null，则自动绑定
					virtual int set_local_receive(ip::udp::endpoint* pep = NULL) 
						throw(ptool_exception) = 0;
					//绑定本地本地发送endpoint,如果为null，则自动绑定
					//如果不调用此函数，则系统默认设置
					virtual int set_local_send(ip::udp::endpoint* pep = NULL) 
						throw(ptool_exception) = 0;
					//启动数据接收
					virtual int	start() = 0;
					//获取绑定后的本地接收point信息
					virtual ip::udp::endpoint get_local_receive() const = 0;

					//获取绑定后的本地发送point信息
					virtual ip::udp::endpoint get_local_send() const = 0;
					//同步发送数据
					//@data:send data
					//@len: send data lenth
					//@ep:如果为NULL则按照SetSendEndpoint绑定的地址发送，否则按照ep发送
					//@return: 返回发送成功的数据长度，单位byte, -1 失败 
					//错误码，参见ptool_exception里面的错误码
					virtual int send_sync(char* data, int len, ip::udp::endpoint* pep = NULL) 
						throw (ptool_exception) = 0;
					//异步发送数据
					//@return:0-成功，-1失败
					virtual int send_async(const char* data, int len, ip::udp::endpoint* pep = NULL) 
						throw (ptool_exception) = 0;
					//同步接收数据
					//len输入为缓冲区长度，返回为接收数据的长度
					virtual void get_data_sync(char* p, int len, ip::udp::endpoint& ep) 
						throw (ptool_exception) = 0;
				};
		}

  		namespace
  			tcp{
				//para: int  参见错误代码，string对返回的代码进行描述, ep:远端地址信息, int 对象id
				typedef std::function<void(int, std::string&, std::vector<boost::asio::ip::tcp::endpoint>&, int)> func_message;
				typedef std::function<void(char* p, std::size_t& len, boost::asio::ip::tcp::endpoint& ep, int)> f_data_tcp;
  				//tcp server
  				class 
  					Itcpserver
  				{
  				public:
  					virtual ~Itcpserver(){};
					//descrip:注册回调函数
					virtual void set_func(f_data_tcp func_d, func_message func_m) throw() = 0 ;
  					//设置监听endpoint
  					virtual void setlisen(boost::asio::ip::tcp::endpoint* pep = NULL) 
						throw(ptool_exception) = 0;
					//启动监听
					virtual void start() = 0;
					//发送数据
					virtual void send(char* p, int len, boost::asio::ip::tcp::endpoint& ep) 
						throw(ptool_exception) = 0;
					//设置超时时间检测
					virtual void set_time_out(int itime) = 0;
					//设置心跳包
					virtual void set_heart(char* p, int lenth, int time_misc) = 0;
					//断开连接
					virtual void disconnect() = 0;
				};

				class 
					Itcpclient
				{
				public:
					virtual ~Itcpclient(){};
					//descrip:注册回调函数
					virtual void set_func(f_data_tcp func_d, func_message func_m) throw() = 0 ;
					//设置本地监听endpoint
					virtual void set_local_addr(boost::asio::ip::tcp::endpoint* pep = NULL) = 0;
					//设置远端连接endpoint
					virtual void set_remote_addr(boost::asio::ip::tcp::endpoint& ep) = 0;
					//启动监听
					virtual void start() = 0;
					//断开连接
					virtual void disconnect() = 0;
					//发送数据
					virtual void send(char* p, int len) = 0;
					//设置超时时间检测 
					//@itime:-1表示不检测超时,默认40秒
					virtual void set_time_out(int itime) = 0;
					//设置心跳包
					//@lenth:长度为0表示不发送心跳包
					virtual void set_heart(char* p, int lenth, int time_misc) = 0;
				};
  		}
	}
}
//bufLenth:缓冲区的char个数
using namespace ptool::net;
EXPORT_BOOST_NET boost::shared_ptr<udp::Iudp> create_udp();
EXPORT_BOOST_NET boost::shared_ptr<tcp::Itcpserver> create_tcp_server(int bufcount, int bufsize, int ty = ptool::esys_type::Cpc);
EXPORT_BOOST_NET boost::shared_ptr<tcp::Itcpclient> create_tcp_client(int bufcount, int bufsize, int id, int ty = ptool::esys_type::Cpc);

