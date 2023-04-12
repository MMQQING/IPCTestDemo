#include "TCPClient.h"
#include "GlobleVar.h"

TCPClient::TCPClient(int index)
{
	_index = index;
	init();
}

TCPClient::~TCPClient()
{
}

void TCPClient::connect()
{
	std::shared_ptr<GlobleVar> p = GlobleVar::GetInstance();
	boost::asio::ip::address addr;
	
	//addr = boost::asio::ip::address::from_string(p->m_stCommonInfo.strRemoteIP);
	addr = boost::asio::ip::address::from_string(m_RemoteIP);
	boost::asio::ip::tcp::endpoint ep(addr, p->m_stCommonInfo.uRemotePort);
	m_ptrTcpObject->set_remote_addr(ep);
	m_ptrTcpObject->start(); 
}

void TCPClient::disConnect()
{
	m_ptrTcpObject->disconnect();
}

void TCPClient::send_cmd(char* pData, int len)
{
	memcpy((char*)&(m_ptrSendBuf.get()[0]), pData, len);
	m_ptrTcpObject->send(m_ptrSendBuf.get(), len);
}

std::string TCPClient::get_remoteIP()
{
	return m_RemoteIP;
}

void TCPClient::init()
{
	m_ptrSendBuf = std::shared_ptr<char>(new char[8192]);
	m_ptrTcpObject = create_tcp_client(30, 65536, 0, ptool::esys_type::Third_party);
	std::shared_ptr<GlobleVar> p = GlobleVar::GetInstance();
	try {
		uint32_t locPort = p->m_stCommonInfo.uLocalPort + _index;
		boost::asio::ip::address addr;
		addr = boost::asio::ip::address::from_string(p->m_stCommonInfo.strLocalIP);
		boost::asio::ip::tcp::endpoint ep(addr, locPort);
		m_ptrTcpObject->set_local_addr(&ep);
	}
	catch (...)
	{
		m_ptrTcpObject->set_local_addr();
	}
	QStringList list = QString::fromStdString(p->m_stCommonInfo.strRemoteIP).split(".");
	QString str = list.at(list.count() - 1);
	str = QString::number(str.toInt() + _index);
	list.replace(list.count() - 1, str);
	QString RemoteIP = list.join(".");
	m_RemoteIP = RemoteIP.toStdString();
}

void TCPClient::getMsg(int codeId, std::string& str, std::vector<boost::asio::ip::tcp::endpoint>& ep)
{
}

void TCPClient::getData(char* p, std::size_t& len, boost::asio::ip::tcp::endpoint& ep)
{
}