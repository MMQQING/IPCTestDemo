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
	//�Դ������ͽ���ö��
	//Cpc-��˾�ڲ�ʹ�ã�����ʱ�̶��Ӱ�ͷ��Ϣ
	//Third_party-��������ʹ��
	enum esys_type{Cpc = 0, Third_party};
	//
	namespace 
	net{
		//udp
		namespace
		udp{
			//˵����1���첽��������ʹ��
			//	   2����Ҫ����̫�����������Ӱ������
			//p:�������ݵĻ�����
			//len���������ݵĳ���
			//endpoint���������ݵ�Զ����Ϣ
			typedef std::function<void(char* p, std::size_t& len, boost::asio::ip::udp::endpoint& ep)> f_data_udp;
			using namespace boost::asio;
			class Iudp
				{
				public:
					virtual ~Iudp(){}
					//�������ݽ��ջص�����
					virtual void set_func_data(f_data_udp f) = 0;
					//���÷���Ŀ��endpoint
					virtual void set_remote_point(ip::udp::endpoint& ep) throw() = 0;
					//�󶨱��ؽ���endpoint,���Ϊnull�����Զ���
					virtual int set_local_receive(ip::udp::endpoint* pep = NULL) 
						throw(ptool_exception) = 0;
					//�󶨱��ر��ط���endpoint,���Ϊnull�����Զ���
					//��������ô˺�������ϵͳĬ������
					virtual int set_local_send(ip::udp::endpoint* pep = NULL) 
						throw(ptool_exception) = 0;
					//�������ݽ���
					virtual int	start() = 0;
					//��ȡ�󶨺�ı��ؽ���point��Ϣ
					virtual ip::udp::endpoint get_local_receive() const = 0;

					//��ȡ�󶨺�ı��ط���point��Ϣ
					virtual ip::udp::endpoint get_local_send() const = 0;
					//ͬ����������
					//@data:send data
					//@len: send data lenth
					//@ep:���ΪNULL����SetSendEndpoint�󶨵ĵ�ַ���ͣ�������ep����
					//@return: ���ط��ͳɹ������ݳ��ȣ���λbyte, -1 ʧ�� 
					//�����룬�μ�ptool_exception����Ĵ�����
					virtual int send_sync(char* data, int len, ip::udp::endpoint* pep = NULL) 
						throw (ptool_exception) = 0;
					//�첽��������
					//@return:0-�ɹ���-1ʧ��
					virtual int send_async(const char* data, int len, ip::udp::endpoint* pep = NULL) 
						throw (ptool_exception) = 0;
					//ͬ����������
					//len����Ϊ���������ȣ�����Ϊ�������ݵĳ���
					virtual void get_data_sync(char* p, int len, ip::udp::endpoint& ep) 
						throw (ptool_exception) = 0;
				};
		}

  		namespace
  			tcp{
				//para: int  �μ�������룬string�Է��صĴ����������, ep:Զ�˵�ַ��Ϣ, int ����id
				typedef std::function<void(int, std::string&, std::vector<boost::asio::ip::tcp::endpoint>&, int)> func_message;
				typedef std::function<void(char* p, std::size_t& len, boost::asio::ip::tcp::endpoint& ep, int)> f_data_tcp;
  				//tcp server
  				class 
  					Itcpserver
  				{
  				public:
  					virtual ~Itcpserver(){};
					//descrip:ע��ص�����
					virtual void set_func(f_data_tcp func_d, func_message func_m) throw() = 0 ;
  					//���ü���endpoint
  					virtual void setlisen(boost::asio::ip::tcp::endpoint* pep = NULL) 
						throw(ptool_exception) = 0;
					//��������
					virtual void start() = 0;
					//��������
					virtual void send(char* p, int len, boost::asio::ip::tcp::endpoint& ep) 
						throw(ptool_exception) = 0;
					//���ó�ʱʱ����
					virtual void set_time_out(int itime) = 0;
					//����������
					virtual void set_heart(char* p, int lenth, int time_misc) = 0;
					//�Ͽ�����
					virtual void disconnect() = 0;
				};

				class 
					Itcpclient
				{
				public:
					virtual ~Itcpclient(){};
					//descrip:ע��ص�����
					virtual void set_func(f_data_tcp func_d, func_message func_m) throw() = 0 ;
					//���ñ��ؼ���endpoint
					virtual void set_local_addr(boost::asio::ip::tcp::endpoint* pep = NULL) = 0;
					//����Զ������endpoint
					virtual void set_remote_addr(boost::asio::ip::tcp::endpoint& ep) = 0;
					//��������
					virtual void start() = 0;
					//�Ͽ�����
					virtual void disconnect() = 0;
					//��������
					virtual void send(char* p, int len) = 0;
					//���ó�ʱʱ���� 
					//@itime:-1��ʾ����ⳬʱ,Ĭ��40��
					virtual void set_time_out(int itime) = 0;
					//����������
					//@lenth:����Ϊ0��ʾ������������
					virtual void set_heart(char* p, int lenth, int time_misc) = 0;
				};
  		}
	}
}
//bufLenth:��������char����
using namespace ptool::net;
EXPORT_BOOST_NET boost::shared_ptr<udp::Iudp> create_udp();
EXPORT_BOOST_NET boost::shared_ptr<tcp::Itcpserver> create_tcp_server(int bufcount, int bufsize, int ty = ptool::esys_type::Cpc);
EXPORT_BOOST_NET boost::shared_ptr<tcp::Itcpclient> create_tcp_client(int bufcount, int bufsize, int id, int ty = ptool::esys_type::Cpc);

