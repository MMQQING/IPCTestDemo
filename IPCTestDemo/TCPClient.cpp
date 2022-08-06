#include "TCPClient.h"
#include "GlobleVar.h"

TCPClient::TCPClient()
{
	init();
}

TCPClient::~TCPClient()
{
}

void TCPClient::connect()
{
	std::shared_ptr<GlobleVar> p = GlobleVar::GetInstance();
	boost::asio::ip::address addr;
	addr = boost::asio::ip::address::from_string(p->m_stCommonInfo.strRemoteIP);
	boost::asio::ip::tcp::endpoint ep(addr, p->m_stCommonInfo.uRemotePort);
	m_ptrTcpObject->set_remote_addr(ep);
	m_ptrTcpObject->start(); 
}

void TCPClient::disConnect()
{
	//m_ptrTcpObject->disconnect();
}

void TCPClient::send_cmd(char* pData, int len)
{
	memcpy((char*)&(m_ptrSendBuf.get()[0]), pData, len);
	m_ptrTcpObject->send(m_ptrSendBuf.get(), len);
}

void TCPClient::init()
{
	m_ptrSendBuf = std::shared_ptr<char>(new char[8192]);
	m_ptrTcpObject = create_tcp_client(30, 65536, 0, ptool::esys_type::Third_party);
	std::shared_ptr<GlobleVar> p = GlobleVar::GetInstance();
	try {
		boost::asio::ip::address addr;
		addr = boost::asio::ip::address::from_string(p->m_stCommonInfo.strLocalIP);
		boost::asio::ip::tcp::endpoint ep(addr, p->m_stCommonInfo.uLocalPort);
		m_ptrTcpObject->set_local_addr(&ep);
	}
	catch (...)
	{
		m_ptrTcpObject->set_local_addr();
	}
}

void TCPClient::getMsg(int codeId, std::string& str, std::vector<boost::asio::ip::tcp::endpoint>& ep)
{
}

void TCPClient::getData(char* p, std::size_t& len, boost::asio::ip::tcp::endpoint& ep)
{
}