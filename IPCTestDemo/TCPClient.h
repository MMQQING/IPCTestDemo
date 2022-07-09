#pragma once
#include <boost/asio/ip/address.hpp>
#include <boost/bind.hpp>

#include "load_dll.h"

class TCPClient
{
	enum
		{
			noError,
			hasError
		};
	public:
		TCPClient();
		~TCPClient();

		void connect();
		void disConnect();

		//客户端发送控制命令
		void send_cmd(char* pData, int len);

	private:
		//初始化tcp服务并开辟缓存空间
		void init();
		//指令接收的回调（返回指令执行成功/失败，失败同时返回失败具体描述）
		void getMsg(int id, std::string& str, std::vector<boost::asio::ip::tcp::endpoint>& vEp);
		//数据接收的回调
		void getData(char* p, std::size_t& len, boost::asio::ip::tcp::endpoint& ep);

	private:		
		std::shared_ptr<char>					m_ptrSendBuf;

	public:
		boost::shared_ptr<tcp::Itcpclient>		m_ptrTcpObject;
};