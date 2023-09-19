#include "XmlClass.h"
#include "GlobleVar.h"
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>

XmlClass::XmlClass(std::string str)
{
	GenerateFile(str);
}

XmlClass::~XmlClass()
{
}

void XmlClass::ReadParaXml()
{
	using namespace boost::property_tree;
	ptree pt;
	if (strfilename == "")
		return;
	
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	read_xml(strfilename, pt); 

	ptr->m_stCommonInfo.strUsername = pt.get<std::string>("root.login.para.<xmlattr>.account", "123");
	ptr->m_stCommonInfo.strPsw = pt.get<std::string>("root.login.para.<xmlattr>.psw", "123456");
	ptr->m_stCommonInfo.iAccess = pt.get<uint8_t>("root.login.para.<xmlattr>.access", 0);
	ptr->m_stCommonInfo.iAutoLogin = pt.get<uint8_t>("root.login.para.<xmlattr>.autologin", 0);
	ptr->m_stCommonInfo.iResCode = pt.get<std::string>("root.login.para.<xmlattr>.rescode", "123456");
	ptr->m_stCommonInfo.firmwareVersion = pt.get<std::string>("root.Firmware.firmware.<xmlattr>.firmwareVersion", "");

	ptr->m_stCommonInfo.strLocalIP = pt.get<std::string>("root.Comm.tcppara.<xmlattr>.lip", "192.168.1.51");
	ptr->m_stCommonInfo.strRemoteIP = pt.get<std::string>("root.Comm.tcppara.<xmlattr>.rip", "192.168.1.51");
	ptr->m_stCommonInfo.uLocalPort = pt.get<uint32_t>("root.Comm.tcppara.<xmlattr>.lport", 8053);
	ptr->m_stCommonInfo.uRemotePort = pt.get<uint32_t>("root.Comm.tcppara.<xmlattr>.rport", 8052);
	ptr->m_stCommonInfo.iUrl = pt.get<std::string>("root.Comm.url.<xmlattr>.ipcurl", "http://172.16.6.20:8069");

	ptr->m_stCommonInfo.wifiName = pt.get<std::string>("root.Wifi.wifi.<xmlattr>.wifiname", "");
	ptr->m_stCommonInfo.wifiPassWD = pt.get<std::string>("root.Wifi.wifi.<xmlattr>.wifipasswd", ""); 
	ptr->m_stCommonInfo.SignalStrength = pt.get<int>("root.Wifi.wifi.<xmlattr>.dbm", 0);

	ptr->m_stCommonInfo.iWebHostPort = pt.get<std::string>("root.webPara.web.<xmlattr>.hostport", "http://172.16.6.20:8069");
	ptr->m_stCommonInfo.blMes20 = pt.get<int>("root.webPara.web.<xmlattr>.blMes20", 0);
}

void XmlClass::WriteParaXml()
{
	using namespace boost::property_tree;
	ptree pt;
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();

	pt.put<std::string>("root.login.para.<xmlattr>.account", ptr->m_stCommonInfo.strUsername);
	pt.put<std::string>("root.login.para.<xmlattr>.psw", ptr->m_stCommonInfo.strPsw);
	pt.put<uint8_t>("root.login.para.<xmlattr>.autolog", ptr->m_stCommonInfo.iAutoLogin);
	pt.put<uint8_t>("root.login.para.<xmlattr>.access", ptr->m_stCommonInfo.iAccess);
	pt.put<std::string>("root.login.para.<xmlattr>.rescode", ptr->m_stCommonInfo.iResCode);
	pt.put<std::string>("root.Firmware.firmware.<xmlattr>.firmwareVersion", ptr->m_stCommonInfo.firmwareVersion);

	pt.put<std::string>("root.Comm.tcppara.<xmlattr>.lip", ptr->m_stCommonInfo.strLocalIP);
	pt.put<std::string>("root.Comm.tcppara.<xmlattr>.rip", ptr->m_stCommonInfo.strRemoteIP);
	pt.put<uint32_t>("root.Comm.tcppara.<xmlattr>.lport", ptr->m_stCommonInfo.uLocalPort);
	pt.put<uint32_t>("root.Comm.tcppara.<xmlattr>.rport", ptr->m_stCommonInfo.uRemotePort);
	pt.put<std::string>("root.Comm.url.<xmlattr>.ipcurl", ptr->m_stCommonInfo.iUrl);

	pt.put<std::string>("root.Wifi.wifi.<xmlattr>.wifiname", ptr->m_stCommonInfo.wifiName);
	pt.put<std::string>("root.Wifi.wifi.<xmlattr>.wifipasswd", ptr->m_stCommonInfo.wifiPassWD);
	pt.put<int>("root.Wifi.wifi.<xmlattr>.dbm", ptr->m_stCommonInfo.SignalStrength);

	pt.put<std::string>("root.webPara.web.<xmlattr>.hostport", ptr->m_stCommonInfo.iWebHostPort);
	pt.put<int>("root.webPara.web.<xmlattr>.blMes20", ptr->m_stCommonInfo.blMes20);

	xml_writer_settings <std::string > settings('\t', 1);
	write_xml(strfilename, pt, std::locale(), settings);
}

void XmlClass::ReadListXml()
{
	using namespace boost::property_tree;
	ptree pt;
	if (strfilename == "")
		return;

	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	read_xml(strfilename, pt);
	try{
		pt.get_child("root.modulesA");
	}
	catch (boost::property_tree::ptree_bad_path& e) {
		// variable_order is optional
		return;
	}
	try {
		pt.get_child("root.modulesB");
	}
	catch (boost::property_tree::ptree_bad_path& e) {
		// variable_order is optional
		return;
	}
	
	BOOST_FOREACH(ptree::value_type &v, pt.get_child("root.modulesA"))
	{
		std::cout << v.first << " " << v.second.data() << std::endl;
		ptr->m_testListA.push_back(v.second.data());
	}
	BOOST_FOREACH(ptree::value_type &v, pt.get_child("root.modulesB"))
	{
		std::cout << v.first << " " << v.second.data() << std::endl;
		ptr->m_testListB.push_back(v.second.data());
	}
}


void XmlClass::WriteListXml()
{
	using namespace boost::property_tree;
	ptree pt;
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	
	size_t n =  ptr->m_testListA.size();
	BOOST_FOREACH(const std::string &name, ptr->m_testListA)
		pt.add("root.modulesA.module", name);

	BOOST_FOREACH(const std::string &name1, ptr->m_testListB)
		pt.add("root.modulesB.module", name1);

	xml_writer_settings <std::string > settings('\t', 1);
	write_xml(strfilename, pt, std::locale(), settings);
}

void XmlClass::GenerateFile(std::string str)
{
	using namespace boost::filesystem;
// 	if (!exists("./WorkDir/Para"))        //工作目录不存在，默认为当前工作目录
// 	{
// 		boost::filesystem::path pt("./WorkDir/Para");
// 		create_directory(pt);
// 	}
	try {
		file_size(str);
		strfilename = str;
	}
	catch (filesystem_error& e)
	{
		FILE* file = fopen(str.c_str(), "wb+");
		if (file)
			fclose(file);
		CPCLOG_ERROR << "xml:" << e.what();
	}
}