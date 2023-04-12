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
		explicit TCPClient(int);
		~TCPClient();

		void connect();
		void disConnect();

		//�ͻ��˷��Ϳ�������
		void send_cmd(char* pData, int len);
		std::string get_remoteIP();

	private:
		//��ʼ��tcp���񲢿��ٻ���ռ�
		void init();
		//ָ����յĻص�������ָ��ִ�гɹ�/ʧ�ܣ�ʧ��ͬʱ����ʧ�ܾ���������
		void getMsg(int id, std::string& str, std::vector<boost::asio::ip::tcp::endpoint>& vEp);
		//���ݽ��յĻص�
		void getData(char* p, std::size_t& len, boost::asio::ip::tcp::endpoint& ep);

	private:		
		int										_index;
		std::shared_ptr<char>					m_ptrSendBuf;
		std::string									m_RemoteIP;

	public:
		boost::shared_ptr<tcp::Itcpclient>		m_ptrTcpObject;
};