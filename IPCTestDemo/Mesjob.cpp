#include "Mesjob.h"
#include "GlobleVar.h"
#include <QCoreApplication>

Mesjob::Mesjob()
{
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
	auto scheme_host_port = "https://localhost:5003";
#else
	auto scheme_host_port = gVar->m_stCommonInfo.iWebHostPort;
#endif
	cli = std::shared_ptr<httplib::Client>(new httplib::Client(scheme_host_port));
	qRegisterMetaType<QMap<QString, QString>>("QMap<QString ,QString>");
}

Mesjob::~Mesjob()
{

}

boost::property_tree::ptree Mesjob::parse_res(std::string str)
{
	boost::property_tree::ptree  pt;
	std::stringstream str_stream(str);
	try {
		read_xml(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error& e) {
		CPCLOG_ERROR << e.what();
		return pt;
	}
	return pt;
}

std::string Mesjob::solve(std::string &str1, std::string &str2)
{
	if (str2.size() == 0)
		return str1;
	for (int i = 0; i < (volatile int)(str1.size() - str2.size() + 1);)
	{
		if (str1.substr(i, (volatile int)str2.size()) == str2)
		{
			str1.erase(i, (volatile int)str2.size());
			if (i > 0)
				i--;
		}
		else
			i++;
	}
	return str1;
}

std::string Mesjob::UTF8_To_String(std::string str)
{
	if (str.empty())
	{
		return std::string();
	}

	std::wstring result;

	int n = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t * buffer = new wchar_t[n];

	::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, n);

	result = buffer;
	delete[] buffer;

	std::string result2;
	int len = WideCharToMultiByte(CP_ACP, 0, result.c_str(), result.size(), NULL, 0, NULL, NULL);
	char* buffer2 = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, result.c_str(), result.size(), buffer2, len, NULL, NULL);
	buffer2[len] = '\0';
	result2.append(buffer2);
	delete[] buffer2;

	return result2;
}

bool Mesjob::common_login()
{
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	using namespace boost::property_tree;
	ptree pt;
	pt.put<std::string>("document.UserCode", ptr->m_stCommonInfo.strUsername);
	pt.put<std::string>("document.UserPwd", ptr->m_stCommonInfo.strPsw);
	pt.put<std::string>("document.ResCode", ptr->m_stCommonInfo.iResCode);
	xml_writer_settings <std::string > settings('\t', 1);
	std::stringstream str_stream;
	write_xml(str_stream, pt);
	std::string str = str_stream.str();
	std::string str_head("<?xml version=\"1.0\" encoding=\"utf - 8\"?>");
	//std::string str_head = R"(<?xml version="1.0" encoding="utf-8"?>)";
	str.erase(0, str_head.size() - 1);

	//str = str.replace(str.begin(), str.find_first_of(">"), "");
	//json
	boost::property_tree::ptree pItem;
	pItem.put("InterfaceNo",std::to_string(10));
	pItem.put("Params", str);
	pItem.put("AttachParams", "");

	std::stringstream stream;
	write_json(stream, pItem);

	//发送给web
	std::string body = stream.str();
	std::string path;
	if(gVar->m_stCommonInfo.blMes20)
		path = "/api/common/call";
	else
		path = "/common/call";
	if (auto res = cli->Post(path.c_str(), body, "application/json")) {	
		if (res->status == 200) {
			std::cout << res->body << std::endl;
			std::string m_str = UTF8_To_String(res->body);
			boost::property_tree::ptree pt = parse_res(m_str);
			std::string str = pt.get<std::string>("document.ReturnValue");
			if (!atoi(str.c_str())) {
				str = pt.get<std::string>("document.ReturnMessage"); 
				emit mesConn(false, QString::fromLocal8Bit(str.c_str()));
				CPCLOG_ERROR << str;
				return false;
			}
			else
			{
				ptr->m_stLoginInfo.LineId = pt.get<std::string>("document.LineId", "123");
				ptr->m_stLoginInfo.ResId = pt.get<std::string>("document.ResId", "123");
				ptr->m_stLoginInfo.ShiftTypeId = pt.get<std::string>("document.ShiftTypeId", "123");
				ptr->m_stLoginInfo.UserId = pt.get<std::string>("document.UserId", "123");
				ptr->m_stLoginInfo.OPId = pt.get<std::string>("document.OPId", "123");
			//TEST
				emit mesConn(true,QString::fromLocal8Bit("连接成功"));
			return true;
 			}
		}
	}
	else {
		emit mesConn(false, QString::fromLocal8Bit("请修改MES地址..."));
		CPCLOG_ERROR << QString::fromLocal8Bit("请修改MES地址...").toStdString();
		return false;
	}
	return false;
}

//通过CMEI查询
void Mesjob::common_get20info(QString CMEIstr)
{
	QVariant DataVar;
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	using namespace boost::property_tree;
	ptree pt;
	pt.put<std::string>("document.IsTransferMO", std::to_string(0));
	pt.put<std::string>("document.MOId", "");
	pt.put<std::string>("document.LotSNType", "CMEI");
	pt.put<std::string>("document.LotSN", CMEIstr.toStdString());
	pt.put<std::string>("document.LineId", ptr->m_stLoginInfo.LineId);
	pt.put<std::string>("document.ResId", ptr->m_stLoginInfo.ResId);
	pt.put<std::string>("document.UserId", ptr->m_stLoginInfo.UserId);
	pt.put<std::string>("document.OPId", ptr->m_stLoginInfo.OPId);
	xml_writer_settings <std::string > settings('\t', 1);
	std::stringstream str_stream;
	write_xml(str_stream, pt);
	std::string str = str_stream.str();
	std::string str_head("<?xml version=\"1.0\" encoding=\"utf - 8\"?>");
	str.erase(0, str_head.size() - 1);
	//json
	boost::property_tree::ptree pItem;
	pItem.put("InterfaceNo", std::to_string(20));
	pItem.put("Params", str);
	pItem.put("AttachParams", "");

	std::stringstream stream;
	write_json(stream, pItem);
	std::string str1 = stream.str();
	//遍历结构体代码test
//	st_20_result* P_Member = &m_st20Info;			//指向结构体的指针
//	std::string* P = (std::string*)P_Member;		//结构体指针强制转换为string类型

	//发送给web
	std::string body = stream.str();
	std::string path;
	if (gVar->m_stCommonInfo.blMes20)
		path = "/api/common/call";
	else
		path = "/common/call";
	if (auto res = cli->Post(path.c_str(), body, "application/json")){
		if (res->status == 200) {
			std::cout << res->body << std::endl;
			std::string m_str = UTF8_To_String(res->body);
			boost::property_tree::ptree pt = parse_res(m_str);
			std::string str = pt.get<std::string>("document.ReturnValue");
			if (!atoi(str.c_str())) {
				str = pt.get<std::string>("document.ReturnMessage");
				emit mes20Res(false, QString::fromLocal8Bit(str.c_str()).simplified(), DataVar);
				CPCLOG_ERROR << QString::fromLocal8Bit(str.c_str()).toStdString();
				return;
			}
			else
			{
// 				auto child = pt.get_child("document");
// 				for (auto i = child.begin(); i != child.end(); ++i) {
// 					(*P++) = i->second.get_value<std::string>();
// 				}
				//TEST
				QMap<QString, QString> _map;
				m_st20Info.MOCode	= pt.get<std::string>("document.MOCode", "123");
				m_st20Info.LotId	= pt.get<std::string>("document.LotId", "123");

				_map.insert("MOCode", QString::fromStdString(m_st20Info.MOCode));
				DataVar.setValue(_map);
				emit mes20Res(true, QString::fromLocal8Bit("校验成功"), DataVar);
				common_get40info();
				return;
			}
		}
	}
	else {
		emit mes20Res(false, QString::fromLocal8Bit("请修改MES地址..."), DataVar);
		CPCLOG_ERROR << QString::fromLocal8Bit("请修改MES地址...").toStdString();
		return;
	}
	return;
}

void Mesjob::common_get40info()
{
	QVariant DataVar;
	using namespace boost::property_tree;
	ptree pt;
	pt.put<std::string>("document.LotId", m_st20Info.LotId);
	xml_writer_settings <std::string > settings('\t', 1);
	std::stringstream str_stream;
	write_xml(str_stream, pt);
	std::string str = str_stream.str();
	std::string str_head("<?xml version=\"1.0\" encoding=\"utf - 8\"?>");
	str.erase(0, str_head.size() - 1);
	//std::string str_head = R"(<?xml version="1.0" encoding="utf-8"?>)";
	//str = str.replace(str.begin(), str.begin() + sizeof(str_head) - 1, "");
	//json
	boost::property_tree::ptree pItem;
	pItem.put("InterfaceNo", std::to_string(40));
	pItem.put("Params", str);
	pItem.put("AttachParams", "");

	std::stringstream stream;
	write_json(stream, pItem);
	//	std::string str1 = stream.str();
	//遍历结构体代码test
//	st_40_result* P_Member = &m_st40Info;		//指向结构体的指针
//	std::string* P = (std::string*)P_Member;		//结构体指针强制转换为string类型

	//发送给web
	std::string body = stream.str();
	std::string path;
	if (gVar->m_stCommonInfo.blMes20)
		path = "/api/common/call";
	else
		path = "/common/call";
	if (auto res = cli->Post(path.c_str(), body, "application/json")) {
		if (res->status == 200) {
			std::cout << res->body << std::endl;
			std::string m_str = UTF8_To_String(res->body);
			boost::property_tree::ptree pt = parse_res(m_str);
			std::string str = pt.get<std::string>("document.ReturnValue");
			if (!atoi(str.c_str())) {
				str = pt.get<std::string>("document.ReturnMessage");
				emit mes40Res(false, QString::fromLocal8Bit(str.c_str()).simplified(), DataVar);
				CPCLOG_ERROR << str;
				return;
			}
			else
			{
// 				auto child = pt.get_child("document");
// 				for (auto i = child.begin(); i != child.end(); ++i) {
// 					(*P++) = i->second.get_value<std::string>();
// 				}

				QMap<QString, QString> _map;

				m_st40Info.Mac					= pt.get<std::string>("document.Mac", "123");
				m_st40Info.SN					= pt.get<std::string>("document.SN", "123");
				m_st40Info.DeviceSerialNumber	= pt.get<std::string>("document.DeviceSerialNumber", "123");
				m_st40Info.CiphertextPassword	= pt.get<std::string>("document.CiphertextPassword", "123");
				m_st40Info.CMEI					= pt.get<std::string>("document.CMEI", "123");
				m_st40Info.UserPass				= pt.get<std::string>("document.UserPass", "123");
				m_st40Info.DevKey				= pt.get<std::string>("document.DevKey", "123");
				m_st40Info.PCBASN				= pt.get<std::string>("document.PCBASN", "123");

				_map.insert("Mac", QString::fromStdString(m_st40Info.Mac));
				_map.insert("SN", QString::fromStdString(m_st40Info.SN));
				_map.insert("DeviceSerialNumber", QString::fromStdString(m_st40Info.DeviceSerialNumber));
				_map.insert("CiphertextPassword", QString::fromStdString(m_st40Info.CiphertextPassword));
				_map.insert("CMEI", QString::fromStdString(m_st40Info.CMEI));
				_map.insert("UserPass", QString::fromStdString(m_st40Info.UserPass));
				_map.insert("DevKey", QString::fromStdString(m_st40Info.DevKey));
				_map.insert("PCBASN", QString::fromStdString(m_st40Info.PCBASN));
				
				DataVar.setValue(_map);
				//TEST
				emit mes40Res(true, QString::fromLocal8Bit("信息获取成功"), DataVar);
				return;
			}
		}
	}
	else {
		emit mes40Res(false, QString::fromLocal8Bit("请修改MES地址..."), DataVar);
		CPCLOG_ERROR << QString::fromLocal8Bit("请修改MES地址...").toStdString();
		return;
	}
	return;
}

void Mesjob::common_send30info(int bl, QString qstr, QString errstr)
{
	QString logFilepath = QCoreApplication::applicationDirPath() + "/logs";
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	using namespace boost::property_tree;
	ptree pt;
	pt.put<std::string>("document.IsTransferMO", std::to_string(0));
	pt.put<std::string>("document.MOId", "");
	pt.put<std::string>("document.LotId", m_st20Info.LotId);
	pt.put<std::string>("document.LineId", ptr->m_stLoginInfo.LineId);
	pt.put<std::string>("document.ResId", ptr->m_stLoginInfo.ResId);
	pt.put<std::string>("document.ShiftTypeId", ptr->m_stLoginInfo.ShiftTypeId);
	pt.put<std::string>("document.UserId", ptr->m_stLoginInfo.UserId);
	pt.put<std::string>("document.OPId", ptr->m_stLoginInfo.OPId);
	pt.put<std::string>("document.IsPass", std::to_string(bl));
	pt.put<std::string>("document.TestLog", qstr.toStdString());
	pt.put<std::string>("document.FilePath", logFilepath.toStdString());
	xml_writer_settings <std::string > settings('\t', 1);
	std::stringstream str_stream;
	write_xml(str_stream, pt);
	std::string str = str_stream.str();
	std::string str_head("<?xml version=\"1.0\" encoding=\"utf - 8\"?>");
	str.erase(0, str_head.size() - 1);
	//std::string str_head = R"(<?xml version="1.0" encoding="utf-8"?>)";
	//str = str.replace(str.begin(), str.begin() + sizeof(str_head) - 1, "");
	//json
	boost::property_tree::ptree pItem;
	pItem.put("InterfaceNo", std::to_string(30));
	pItem.put("Params", str);
	pItem.put("AttachParams", errstr.toStdString());

	std::stringstream stream;
	write_json(stream, pItem);
	std::string str1 = stream.str();

	//发送给web
	std::string body = stream.str();
	std::string path;
	if (gVar->m_stCommonInfo.blMes20)
		path = "/api/common/call";
	else
		path = "/common/call";
	if (auto res = cli->Post(path.c_str(), body, "application/json")) {
		if (res->status == 200) {
			std::cout << res->body << std::endl;
			std::string m_str = UTF8_To_String(res->body);
			boost::property_tree::ptree pt = parse_res(m_str);
			std::string str = pt.get<std::string>("document.ReturnValue");
			if (!atoi(str.c_str())) {
				str = pt.get<std::string>("document.ReturnMessage");
				emit mes30Res(false, QString::fromLocal8Bit(str.c_str()).simplified());
				CPCLOG_ERROR << str;
				return;
			}
			else
			{
				str = pt.get<std::string>("document.ReturnMessage");
				emit mes30Res(true, QString::fromLocal8Bit(str.c_str()).simplified());
				return;
			}
		}
	}
	else {
		emit mes30Res(false, QString::fromLocal8Bit("请修改MES地址..."));
		CPCLOG_ERROR << QString::fromLocal8Bit("请修改MES地址...").toStdString();
		return;
	}
}
