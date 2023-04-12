#include "ParsingCmd.h"
#include "GlobleVar.h"
#include <time.h>
#include <ctime> 
#include <qDebug>
#include <QtCore/QProcess>
ParsingCmd::ParsingCmd()
{
	init();
}

ParsingCmd::~ParsingCmd()
{
	if(m_stCommonInfo.itcpConn == 1)
		m_tcpClient->disConnect();
}

void ParsingCmd::init()
{
	//m_ptrGloble = GlobleVar::GetInstance();
	qRegisterMetaType<QMap<QString ,QString>>("QMap<QString ,QString>");
}

void ParsingCmd::tcpConnect(int index)
{
	m_tcpClient = std::shared_ptr<TCPClient>(new TCPClient(index));
	m_tcpClient->m_ptrTcpObject->set_func(boost::bind(&ParsingCmd::UnParsCmd, this, _1, _2, _3), boost::bind(&ParsingCmd::getMsg, this, _1, _2, _3));
	//std::string ss = "1111";
	//m_tcpClient->m_ptrTcpObject->set_heart(const_cast<char*>(ss.c_str()), ss.length(), 1000);
	std::string remoteIP = m_tcpClient->get_remoteIP();
	QString strArg = "arp -d "+ QString::fromStdString(remoteIP);
	 int exitCode = QProcess::execute(strArg);
	 qDebug() << "process rst :" << exitCode;
	m_tcpClient->connect();
}

void ParsingCmd::tcpDisConnect()
{
	if (m_stCommonInfo.itcpConn == 1)
		m_tcpClient->disConnect();
}

void ParsingCmd::ParsCmd(uint32_t testname, uint32_t command, uint32_t value)
{
	std::string str;
	if (testname == CMD_VIDEO && command == A_TEST)
	{
		TS_Video();
		return;
	}
	if (testname == CMD_VIDEO && command == A_RESULT)
	{
		RS_Video(value);
		return;
	}
	if(command == A_TEST){
		switch (testname)
		{
		case CMD_DEVIINFO:				str = TS_GetDeviceInfo();			break;
		case CMD_URL:					str = TS_GetURLInfo();				break;
		case CMD_KEYREAD:				str = TS_GetOperatorsInfo();		break;
		case CMD_EXITTEST:				str = TS_SetTestMode();				break;
		case CMD_SETTIME:				str = TS_TimeWrite();				break;
		case CMD_KEYWRITE:				str = TS_SetOperatorsInfo();		break;
		case CMD_SETDEFAULT:			str = TS_SetDefault();				break;
		case CMD_WHITELIGHT:			str = TS_WhiteLight();				break;
		case CMD_IRLIGHT:				str = TS_InfraredLamp();			break;
		case CMD_IRCUT:					str = TS_IRCUT();					break;
		case CMD_NETLED:				str = TS_LEDLights();				break;
		case CMD_SPEAKERTEST:			str = TS_Speaker();					break;
		case CMD_MICTEST:				str = TS_MIC();						break;
		case CMD_IRNVTEST:				str = TS_InfraredVison();			break;
		case CMD_FULLCOLORNV:			str = TS_FullcolorVison();			break;
		case CMD_KEYTEST:				str = TS_RestButton();				break;
		case CMD_VOIPKEYTEST:			str = TS_CallButton();				break;
		case CMD_TFTEST:				str = TS_TFCard();					break;
		case CMD_LDRTEST:				str = TS_Photosensitive();			break;
		case CMD_PTZTEST:				str = TS_PtzMotor();				break;
		case CMD_WIFISCANRESULT:		str = TS_WiFiScanResult();			break;
		case CMD_RESET:					str = TS_ReSet();					break;
		default:
			break;
		}
	}
	else if(command == A_RESULT){
		switch (testname)
		{
		case CMD_WHITELIGHT:	str = RS_WhiteLight(value);			break;
		case CMD_IRLIGHT:		str = RS_InfraredLamp(value);		break;
		case CMD_IRCUT:			str = RS_IRCUT(value);				break;
		case CMD_NETLED:		str = RS_LEDLights(value);			break;
		case CMD_SPEAKERTEST:	str = RS_Speaker(value);			break;
		case CMD_MICTEST:		str = RS_MIC(value);				break;
		case CMD_IRNVTEST:		str = RS_InfraredVison(value);		break;
		case CMD_FULLCOLORNV:	str = RS_FullcolorVison(value);		break;
		case CMD_KEYTEST:		str = RS_RestButton();				break;
		case CMD_VOIPKEYTEST:	str = RS_CallButton();				break;
		case CMD_TFTEST:		str = RS_TFCard();					break;
		case CMD_LDRTEST:		str = RS_Photosensitive();			break;
		case CMD_PTZTEST:		str = RS_PtzMotor(value);			break;
		default:
			break;
		}
	}
	else if(command == A_CONTROL){
		switch (testname)
		{
		case ptz_up:			str = PTZ_UP();						break;
		case ptz_down:			str = PTZ_DOWN();					break;
		case ptz_left:			str = PTZ_LEFT();					break;
		case ptz_right:			str = PTZ_RIGHT();					break;
		case ptz_zero:			str = PTZ_GetLogo();				break;
		default:
			break;
		}
	}
	if (m_tcpClient != nullptr) {
		m_tcpClient->send_cmd(const_cast<char*>(str.c_str()), str.length());
	}
//日志
		IM::Apacket apacketinfo;
		if(apacketinfo.ParseFromString(str)){
			QString str = QString("command:%1\ntestname:%2\nversion:%3\ntype:%4\nresult:%5\ntimeout:%6\nnum:%7")
				.arg(apacketinfo.msg().command())
				.arg(apacketinfo.msg().testname().c_str())
				.arg(apacketinfo.msg().version().c_str())
				.arg(apacketinfo.msg().type())
				.arg(apacketinfo.msg().result())
				.arg(apacketinfo.msg().timeout())
				.arg(apacketinfo.data().num());
			for (int i = 0; i < apacketinfo.data().num(); i++)
			{
				QString ss = QString("\n%1:%2")
					.arg(apacketinfo.data().value(i).name().c_str())
					.arg(apacketinfo.data().value(i).value().c_str());
				str += ss;
			}
			CPCLOG_INFO << "\n******* Output *********\n"<< str.toStdString();
		}
//
}

void ParsingCmd::UnParsCmd(char* p, std::size_t& len, boost::asio::ip::tcp::endpoint& ep)
{
	std::string str(p, len);
	IM::Apacket apacketinfo;
	if (apacketinfo.ParseFromString(str)) {
		//日志
		{
			QString str = QString("command:%1\ntestname:%2\nversion:%3\ntype:%4\nresult:%5\ntimeout:%6\nnum:%7")
				.arg(apacketinfo.msg().command())
				.arg(apacketinfo.msg().testname().c_str())
				.arg(apacketinfo.msg().version().c_str())
				.arg(apacketinfo.msg().type())
				.arg(apacketinfo.msg().result())
				.arg(apacketinfo.msg().timeout())
				.arg(apacketinfo.data().num());
			for (int i = 0; i < apacketinfo.data().num(); i++)
			{
				QString ss = QString("\n%1:%2")
					.arg(apacketinfo.data().value(i).name().c_str())
					.arg(apacketinfo.data().value(i).value().c_str());
				str += ss;
			}
			CPCLOG_INFO << "\n******* Input *********\n" << str.toStdString();
		}
		//
		switch (apacketinfo.msg().type())
		{
		case R_Get:			Rec_GetInfo(apacketinfo);			break;
		case R_Set:			Rec_SetInfo(apacketinfo);			break;
		case R_AutoTest:	Rec_AutoTest(apacketinfo);			break;
		case R_ManTest:		Rec_ManTest(apacketinfo);			break;
		default:
			break;
		}
	}
}

void ParsingCmd::getMsg(int id, std::string& str, std::vector<boost::asio::ip::tcp::endpoint>& vEp)
{
	std::string r_str;
	if (vEp.size() == 0) {
		return;
	}
	boost::asio::ip::tcp::endpoint  p = vEp[0];
	m_stCommonInfo.strRemoteIP = p.address().to_string();
	r_str.append(m_stCommonInfo.strRemoteIP);
	if (0 == id)
	{
		r_str.append("已成功连接");
		m_stCommonInfo.iLostPack = 0;
		m_stCommonInfo.itcpConn = true;													//TCP连接成功
		emit tcpConn(true, QString::fromLocal8Bit(r_str.c_str()));						//连接成功，更新登录界面<连接>背景色
	}
	else if (4 == id)
	{
		m_stCommonInfo.itcpConn = false;				//TCP连接失败
		r_str.append("已断开连接");
		//if (GlobleVar::GetInstance()->m_iLogin)										//未登录
		emit tcpConn(false, QString::fromLocal8Bit(r_str.c_str()));
	}
	else if (6 == id)
	{
		m_stCommonInfo.itcpConn = false;				//TCP连接失败
// 		r_str.append("连接超时，正重连...");
// 		m_tcpClient->connect();
	}
	else r_str.append(str);
	
	//emit updateConn(m_stCommonInfo.itcpConn);
	m_stCommonInfo.strconnect = r_str;
}

void ParsingCmd::setWiFi(QString wifiname, QString passwd)
{
	uint32_t command = A_TEST;
	std::string testname = "wifiset";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	IM::deviceinfo* data = new IM::deviceinfo();
	data->set_num(2);
	IM::info_s *value1 = data->add_value();
	value1->set_name("ssid");
	value1->set_value(wifiname.toStdString());

	IM::info_s *value2 = data->add_value();
	value2->set_name("psk");
	value2->set_value(passwd.toStdString());

	apacket.set_allocated_data(data);

	std::string str;
	apacket.SerializeToString(&str);
	if (m_tcpClient != nullptr) {
		m_tcpClient->send_cmd(const_cast<char*>(str.c_str()), str.length());
	}
	Sleep(100);
	//getWiFi();
	return;
}

void ParsingCmd::getWiFi()
{
	uint32_t command = A_TEST;
	std::string testname = "wifiget";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	if (m_tcpClient != nullptr) {
		m_tcpClient->send_cmd(const_cast<char*>(str.c_str()), str.length());
	}
	return;
}

///测试项发送指令给IPC
///获取信息
//获取设备信息
std::string ParsingCmd::TS_GetDeviceInfo()
{
	uint32_t command = A_TEST;
	std::string testname = "devinfo";
	
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();	
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);
//测试代码
// 	IM::deviceinfo *_devinfo = new IM::deviceinfo();
// 	IM::info_s *_value = _devinfo->add_value();
// 	std::string _name = "SN";
// 	std::string _myvalue = "1181181";
// 	_value->set_name(_name);
// 	_value->set_value(_myvalue);
// 	_devinfo->set_num(1);
// 	apacket.set_allocated_data(_devinfo);
//测试关闭窗口代码
//	emit manTimeout(5);
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//获取直播播放地址
std::string ParsingCmd::TS_GetURLInfo()
{
	uint32_t command = A_TEST;
	std::string testname = "url";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//读运营商信息
std::string ParsingCmd::TS_GetOperatorsInfo()
{
	uint32_t command = A_TEST;
	std::string testname = "keyread";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//退出厂测模式
std::string ParsingCmd::TS_SetTestMode()
{
	uint32_t command = A_TEST;
	std::string testname = "exittest";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

///设置信息
//写运营商信息
std::string ParsingCmd::TS_SetOperatorsInfo()
{
	uint32_t command = A_TEST;
	std::string testname = "keywrite";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	IM::deviceinfo* data = new IM::deviceinfo();
	data->set_num(7);
	IM::info_s *value1 = data->add_value();
	value1->set_name("sn");
	value1->set_value(m_writeInfo.SN);

	IM::info_s *value2 = data->add_value();
	value2->set_name("mac");
	value2->set_value(m_writeInfo.Mac);

	IM::info_s *value3 = data->add_value();
	value3->set_name("cmei");
	value3->set_value(m_writeInfo.CMEI);

	IM::info_s *value4 = data->add_value();
	value4->set_name("deviceserialnumber");					//和SN一致
	value4->set_value(m_writeInfo.DeviceSerialNumber);

	IM::info_s *value5 = data->add_value();
	value5->set_name("devkey");
	value5->set_value(m_writeInfo.DevKey);

	IM::info_s *value6 = data->add_value();
	value6->set_name("userpass");
	value6->set_value(m_writeInfo.UserPass);

	IM::info_s *value7 = data->add_value();
	value7->set_name("CiphertextPassword");
	value7->set_value(m_writeInfo.CiphertextPassword);

	apacket.set_allocated_data(data);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//恢复出厂设置
std::string ParsingCmd::TS_SetDefault()
{
	uint32_t command = A_TEST;
	std::string testname = "setdefault";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//设置时间
std::string ParsingCmd::TS_TimeWrite()
{
	uint32_t command = A_TEST;
	std::string testname = "timewrite";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	IM::deviceinfo* data = new IM::deviceinfo();
	data->set_num(1);
	IM::info_s *value1 = data->add_value();
	value1->set_name("timestr");
	time_t tt = time(NULL);//这句返回的只是一个时间戳
	std::string str_time = "@" + std::to_string(tt);
	value1->set_value(str_time);
	apacket.set_allocated_data(data);
// 
 	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

///手动测试
//开关白光灯
std::string ParsingCmd::TS_WhiteLight()
{
	uint32_t command = A_TEST;
	std::string testname = "whitelight";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//开关红外灯
std::string ParsingCmd::TS_InfraredLamp()
{
	uint32_t command = A_TEST;
	std::string testname = "irlight";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//开关IRCUT
std::string ParsingCmd::TS_IRCUT()
{
	uint32_t command = A_TEST;
	std::string testname = "ircut";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//开关网络LED灯
std::string ParsingCmd::TS_LEDLights()
{
	uint32_t command = A_TEST;
	std::string testname = "netled";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//测试扬声器
std::string ParsingCmd::TS_Speaker()
{
	uint32_t command = A_TEST;
	std::string testname = "speakertest";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//测试麦克风
std::string ParsingCmd::TS_MIC()
{
	uint32_t command = A_TEST;
	std::string testname = "mictest";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//测试红外夜视
std::string ParsingCmd::TS_InfraredVison()
{
	uint32_t command = A_TEST;
	std::string testname = "irnvtest";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//测试全彩夜视
std::string ParsingCmd::TS_FullcolorVison()
{
	uint32_t command = A_TEST;
	std::string testname = "fullcolornv";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

///自动测试
//测试Rest按键
std::string ParsingCmd::TS_RestButton()
{
	uint32_t command = A_TEST;
	std::string testname = "keytest";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::TS_CallButton()
{
	uint32_t command = A_TEST;
	std::string testname = "voipkeytest";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

//测试TF卡
std::string ParsingCmd::TS_TFCard()
{
	uint32_t command = A_TEST;
	std::string testname = "tftest";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//测试光敏
std::string ParsingCmd::TS_Photosensitive()
{
	uint32_t command = A_TEST;
	std::string testname = "ldrtest";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

//进入厂测模式
//测试PTZ马达
std::string ParsingCmd::TS_PtzMotor()
{
	uint32_t command = A_TEST;
	std::string testname = "ptztest";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::PTZ_UP()
{
	uint32_t command = A_TEST;
	std::string testname = "ptzset";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	IM::deviceinfo* data = new IM::deviceinfo();
	data->set_num(3);
	IM::info_s *value1 = data->add_value();
	value1->set_name("is_offset");
	std::string is_offset = "1";
	value1->set_value(is_offset);

	IM::info_s *value2 = data->add_value();
	value2->set_name("y");
	std::string y = "-1";
	value2->set_value(y);

	IM::info_s *value3 = data->add_value();
	value3->set_name("x");
	std::string x = "0";
	value3->set_value(x);

	apacket.set_allocated_data(data);
	// 
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::PTZ_DOWN()
{
	uint32_t command = A_TEST;
	std::string testname = "ptzset";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	IM::deviceinfo* data = new IM::deviceinfo();
	data->set_num(3);
	IM::info_s *value1 = data->add_value();
	value1->set_name("is_offset");
	std::string is_offset = "1";
	value1->set_value(is_offset);

	IM::info_s *value2 = data->add_value();
	value2->set_name("y");
	std::string y = "1";
	value2->set_value(y);

	IM::info_s *value3 = data->add_value();
	value3->set_name("x");
	std::string x = "0";
	value3->set_value(x);

	apacket.set_allocated_data(data);
	// 
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::PTZ_LEFT()
{
	uint32_t command = A_TEST;
	std::string testname = "ptzset";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	IM::deviceinfo* data = new IM::deviceinfo();
	data->set_num(3);
	IM::info_s *value1 = data->add_value();
	value1->set_name("is_offset");
	std::string is_offset = "1";
	value1->set_value(is_offset);

	IM::info_s *value2 = data->add_value();
	value2->set_name("x");
	std::string x = "-1";
	value2->set_value(x);

	IM::info_s *value3 = data->add_value();
	value3->set_name("y");
	std::string y = "0";
	value3->set_value(y);

	apacket.set_allocated_data(data);
	// 
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::PTZ_RIGHT()
{
	uint32_t command = A_TEST;
	std::string testname = "ptzset";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	IM::deviceinfo* data = new IM::deviceinfo();
	data->set_num(3);
	IM::info_s *value1 = data->add_value();
	value1->set_name("is_offset");
	std::string is_offset = "1";
	value1->set_value(is_offset);

	IM::info_s *value2 = data->add_value();
	value2->set_name("x");
	std::string x = "1";
	value2->set_value(x);

	IM::info_s *value3 = data->add_value();
	value3->set_name("y");
	std::string y = "0";
	value3->set_value(y);

	apacket.set_allocated_data(data);
	// 
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::PTZ_GetLogo()
{
	uint32_t command = A_TEST;
	std::string testname = "ptzgetlogo";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::TS_WiFiScanResult()
{
	uint32_t command = A_TEST;
	std::string testname = "wifiscanresults";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

void ParsingCmd::TS_Video()
{
	emit manTimeout("video", 0);
}

std::string ParsingCmd::TS_ReSet()
{
	uint32_t command = A_TEST;
	std::string testname = "reset";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

///测试项回复IPC(手动)
std::string ParsingCmd::RS_WhiteLight(uint32_t value)
{
	uint32_t command = A_RESULT;
	std::string testname = "whitelight";
	uint32_t result = value;
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	msg->set_result(result);
	apacket.set_allocated_msg(msg);
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::RS_InfraredLamp(uint32_t value)
{
	uint32_t command = A_RESULT;
	std::string testname = "irlight";
	uint32_t result = value;
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	msg->set_result(result);
	apacket.set_allocated_msg(msg);
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::RS_IRCUT(uint32_t value)
{
	uint32_t command = A_RESULT;
	std::string testname = "ircut";
	uint32_t result = value;
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	msg->set_result(result);
	apacket.set_allocated_msg(msg);
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::RS_LEDLights(uint32_t value)
{
	uint32_t command = A_RESULT;
	std::string testname = "netled";
	uint32_t result = value;
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	msg->set_result(result);
	apacket.set_allocated_msg(msg);
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::RS_Speaker(uint32_t value)
{
	uint32_t command = A_RESULT;
	std::string testname = "speakertest";
	uint32_t result = value;
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	msg->set_result(result);
	apacket.set_allocated_msg(msg);
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::RS_MIC(uint32_t value)
{
	uint32_t command = A_RESULT;
	std::string testname = "mictest";
	uint32_t result = value;
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	msg->set_result(result);
	apacket.set_allocated_msg(msg);
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::RS_InfraredVison(uint32_t value)
{
	uint32_t command = A_RESULT;
	std::string testname = "irnvtest";
	uint32_t result = value;
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	msg->set_result(result);
	apacket.set_allocated_msg(msg);
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::RS_FullcolorVison(uint32_t value)
{
	uint32_t command = A_RESULT;
	std::string testname = "fullcolornv";
	uint32_t result = value;
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	msg->set_result(result);
	apacket.set_allocated_msg(msg);
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

///测试项回复IPC(自动)
std::string ParsingCmd::RS_RestButton()
{
	uint32_t command = A_RESULT;
	std::string testname = "keytest";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::RS_CallButton()
{
	uint32_t command = A_RESULT;
	std::string testname = "voipkeytest";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::RS_TFCard()
{
	uint32_t command = A_RESULT;
	std::string testname = "tftest";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

std::string ParsingCmd::RS_Photosensitive()
{
	uint32_t command = A_RESULT;
	std::string testname = "ldrtest";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

//PTZ马达测试
std::string ParsingCmd::RS_PtzMotor(uint32_t value)
{
	if(value == Result_False){
		emit manResult(QString::fromStdString("ptztest"), Result_False);
		emit myResult("ptztest", Result_False);
		return "";
	}
	uint32_t command = A_TEST;
	std::string testname = "ptzatlogo";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}

void ParsingCmd::RS_Video(uint32_t value)
{
	emit manResult("video", value);
}

///分解代码
//获取信息
void ParsingCmd::Rec_GetInfo(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().testname() == "devinfo") {
		IPC_RS_Devinfo(apacketinfo);
	}else if(apacketinfo.msg().testname() == "url"){
		IPC_RS_Url(apacketinfo);
	}else if (apacketinfo.msg().testname() == "keyread") {
		IPC_RS_Keyread(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "exittest") {
		IPC_RS_Exittest(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "ptzatlogo") {
		IPC_RS_PTZatlogo(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "ptzgetlogo") {
		IPC_RS_GetLogo(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "wifiscanresults") {
		IPC_RS_GetWiFiSite(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "wifiget") {
		IPC_RS_GetWiFiInfo(apacketinfo);
	}
}
//设置信息
void ParsingCmd::Rec_SetInfo(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().testname() == "keywrite") {
		IPC_RS_Keywrite(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "setdefault") {
		IPC_RS_Setdefault(apacketinfo);
	}
}
//手动测试
void ParsingCmd::Rec_ManTest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().testname() == "whitelight") {
		IPC_RS_Whitelight(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "irlight") {
		IPC_RS_Irlight(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "ircut") {
		IPC_RS_Ircut(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "netled") {
		IPC_RS_Netled(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "speakertest") {
		IPC_RS_Speakertest(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "mictest") {
		IPC_RS_Mictest(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "irnvtest") {
		IPC_RS_Irnvtest(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "fullcolornv") {
		IPC_RS_Fullcolornv(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "ptztest") {
		IPC_RS_PTZTest(apacketinfo);
	}
}
//自动测试
void ParsingCmd::Rec_AutoTest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().testname() == "keytest") {
		IPC_RS_Keytest(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "voipkeytest") {
		IPC_RS_VoipKeytest(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "tftest") {
		IPC_RS_TFtest(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "ldrtest") {
		IPC_RS_Ldrtest(apacketinfo);
	}
}

///IPC回复测试结果给PC
//读取
void ParsingCmd::IPC_RS_Devinfo(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		QMap<QString, QString> _map;
		if(apacketinfo.has_data()){
			for (int i = 0; i < apacketinfo.data().num(); i++) {
				_map.insert(QString::fromStdString(apacketinfo.data().value(i).name()), QString::fromStdString(apacketinfo.data().value(i).value()));
			}
		}
		QVariant DataVar;
		DataVar.setValue(_map);
		emit message(DataVar, "devinfo");
		emit myResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
	}
}

void ParsingCmd::IPC_RS_Url(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		QMap<QString, QString> _map;
		if (apacketinfo.has_data()) {
			for (int i = 0; i < apacketinfo.data().num(); i++) {
				_map.insert(QString::fromStdString(apacketinfo.data().value(i).name()), QString::fromStdString(apacketinfo.data().value(i).value()));
			}
		}
 		QVariant DataVar;
 		DataVar.setValue(_map);
 		emit message(DataVar,"url");
		emit myResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
	}
}

void ParsingCmd::IPC_RS_Keyread(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		QMap<QString, QString> _map;
		if (apacketinfo.has_data()) {
			for (int i = 0; i < apacketinfo.data().num(); i++) {
				_map.insert(QString::fromStdString(apacketinfo.data().value(i).name()), QString::fromStdString(apacketinfo.data().value(i).value()));
			}
		}
		QVariant DataVar;
		DataVar.setValue(_map);
		emit message(DataVar, "keyread");
		emit myResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
	}
}

void ParsingCmd::IPC_RS_Exittest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		uint32_t result = apacketinfo.msg().result();
		emit myResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
	}
}

void ParsingCmd::IPC_RS_PTZatlogo(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		QMap<QString, QString> _map;
		if (apacketinfo.has_data()) {
			for (int i = 0; i < apacketinfo.data().num(); i++) {
				_map.insert(QString::fromStdString(apacketinfo.data().value(i).name()), QString::fromStdString(apacketinfo.data().value(i).value()));
			}
		}
		QString str = _map.value("boolean");
		bool bl;
		if (str == "true")
			bl = true;
		else
			bl = false;
		if(bl){
			emit showMovingWidget();
		}else{
			emit manResult(QString::fromStdString("ptztest"), Result_False);
			emit myResult("ptztest", Result_False);
		}		
	}
}

void ParsingCmd::IPC_RS_GetLogo(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		QMap<QString, QString> _map;
		if (apacketinfo.has_data()) {
			for (int i = 0; i < apacketinfo.data().num(); i++) {
				_map.insert(QString::fromStdString(apacketinfo.data().value(i).name()), QString::fromStdString(apacketinfo.data().value(i).value()));
			}
		}
		IPC_SetLogo(_map);
	}
}

void ParsingCmd::IPC_SetLogo(QMap<QString, QString> map)
{
	uint32_t command = A_TEST;
	std::string testname = "ptzset";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	IM::deviceinfo* data = new IM::deviceinfo();
	data->set_num(8);
	IM::info_s *value1 = data->add_value();
	value1->set_name("is_offset");
	std::string is_offset = map.value("is_offset").toStdString();
	value1->set_value(is_offset);

	IM::info_s *value2 = data->add_value();
	value2->set_name("x");
	std::string x = map.value("x").toStdString();
	value2->set_value(x);

	IM::info_s *value3 = data->add_value();
	value3->set_name("y");
	std::string y = map.value("y").toStdString();
	value3->set_value(y);

	IM::info_s *value4 = data->add_value();
	value4->set_name("hang_sec");
	std::string hang_sec = map.value("hang_sec").toStdString();
	value4->set_value(hang_sec);

	IM::info_s *value5 = data->add_value();
	value5->set_name("abs_max_x");
	std::string abs_max_x = map.value("abs_max_x").toStdString();
	value5->set_value(abs_max_x);

	IM::info_s *value6 = data->add_value();
	value6->set_name("abs_max_y");
	std::string abs_max_y = map.value("abs_max_y").toStdString();
	value6->set_value(abs_max_y);

	IM::info_s *value7 = data->add_value();
	value7->set_name("step_x");
	std::string step_x = map.value("step_x").toStdString();
	value7->set_value(step_x);

	IM::info_s *value8 = data->add_value();
	value8->set_name("step_y");
	std::string step_y = map.value("step_y").toStdString();
	value8->set_value(step_y);

	apacket.set_allocated_data(data);
	// 
	std::string str;
	apacket.SerializeToString(&str);
	if (m_tcpClient != nullptr) {
		m_tcpClient->send_cmd(const_cast<char*>(str.c_str()), str.length());
	}
}

void ParsingCmd::IPC_RS_GetWiFiSite(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		QMap<QString, QString> _map;
		if (apacketinfo.has_data()) {
			for (int i = 0; i < apacketinfo.data().num(); i++) {
				_map.insert(QString::fromStdString(apacketinfo.data().value(i).name()), QString::fromStdString(apacketinfo.data().value(i).value()));
			}
		}
		QVariant DataVar;
		DataVar.setValue(_map);
		emit WiFiSiteInfo(DataVar);
	}
}

void ParsingCmd::IPC_RS_GetWiFiInfo(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		QMap<QString, QString> _map;
		if (apacketinfo.has_data()) {
			for (int i = 0; i < apacketinfo.data().num(); i++) {
				_map.insert(QString::fromStdString(apacketinfo.data().value(i).name()), QString::fromStdString(apacketinfo.data().value(i).value()));
			}
		}
		QVariant DataVar;
		DataVar.setValue(_map);
		emit RsWiFiInfo(DataVar);
	}
}

//设置
void ParsingCmd::IPC_RS_Keywrite(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		uint32_t result = apacketinfo.msg().result();
		emit myResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
	}
}

void ParsingCmd::IPC_RS_Setdefault(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		uint32_t result = apacketinfo.msg().result();
		emit myResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
	}
}
//手动测试
void ParsingCmd::IPC_RS_Whitelight(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != 0) {
			emit manResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
		}
		else {
			emit manTimeout(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().timeout());
		}
	}
}

void ParsingCmd::IPC_RS_Irlight(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != 0) {
			emit manResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
		}
		else {
			emit manTimeout(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().timeout());
		}
	}
}

void ParsingCmd::IPC_RS_Ircut(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != 0) {
			emit manResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
		}
		else {
			emit manTimeout(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().timeout());
		}
	}
}

void ParsingCmd::IPC_RS_Netled(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != 0) {
			emit manResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
		}
		else {
			emit manTimeout(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().timeout());
		}
	}
}

void ParsingCmd::IPC_RS_Speakertest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != 0) {
			emit manResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
		}
		else {
			emit manTimeout(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().timeout());
		}
	}
}

void ParsingCmd::IPC_RS_Mictest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != 0) {
			emit manResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
		}
		else {
			emit manTimeout(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().timeout());
		}
	}
}

void ParsingCmd::IPC_RS_Irnvtest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != 0) {
			emit manResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
		}
		else {
			emit manTimeout(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().timeout());
		}
	}
}

void ParsingCmd::IPC_RS_Fullcolornv(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != 0) {
			emit manResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
		}
		else {
			emit manTimeout(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().timeout());
		}
	}
}

void ParsingCmd::IPC_RS_PTZTest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != 0) {
			emit manResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
		}
		else {
			emit manTimeout(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().timeout());
		}
	}
}

//自动测试
void ParsingCmd::IPC_RS_Keytest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		emit autoTimeout(QString::fromStdString(apacketinfo.msg().testname()),apacketinfo.msg().timeout());
	}
	else if(apacketinfo.msg().command() == A_RESULT){
		emit autoResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
	}
}

void ParsingCmd::IPC_RS_VoipKeytest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		emit autoTimeout(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().timeout());
	}
	else if (apacketinfo.msg().command() == A_RESULT) {
		emit autoResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
	}
}

void ParsingCmd::IPC_RS_TFtest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		emit autoTimeout(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().timeout());
	}
	else if (apacketinfo.msg().command() == A_RESULT) {
		emit autoResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
	}
}

void ParsingCmd::IPC_RS_Ldrtest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		emit autoTimeout(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().timeout());
	}
	else if (apacketinfo.msg().command() == A_RESULT) {
		emit autoResult(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().result());
	}
}

void ParsingCmd::updateWriteInfo(QVariant var)
{
	QMap<QString, QString> _maps = var.value<QMap<QString, QString>>();
	m_writeInfo.CMEI = _maps.value("CMEI").toStdString();
	m_writeInfo.SN = _maps.value("SN").toStdString();
	m_writeInfo.Mac = _maps.value("Mac").toStdString();
	m_writeInfo.DevKey = _maps.value("DevKey").toStdString();
	m_writeInfo.UserPass = _maps.value("UserPass").toStdString();
	m_writeInfo.CiphertextPassword = _maps.value("CiphertextPassword").toStdString();
	m_writeInfo.DeviceSerialNumber = _maps.value("DeviceSerialNumber").toStdString();
}
