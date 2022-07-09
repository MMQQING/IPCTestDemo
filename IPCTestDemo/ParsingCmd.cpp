#include "ParsingCmd.h"
#include "GlobleVar.h"

ParsingCmd::ParsingCmd()
{
	init();
}

ParsingCmd::~ParsingCmd()
{
	if(m_ptrGloble->m_stCommonInfo.itcpConn == 1)
		m_tcpClient->disConnect();
}

void ParsingCmd::init()
{
	m_ptrGloble = GlobleVar::GetInstance();
}

void ParsingCmd::tcpConnect()
{
	m_tcpClient = std::shared_ptr<TCPClient>(new TCPClient());
	m_tcpClient->m_ptrTcpObject->set_func(boost::bind(&ParsingCmd::UnParsCmd, this, _1, _2, _3), boost::bind(&ParsingCmd::getMsg, this, _1, _2, _3));
	m_tcpClient->connect();
}

void ParsingCmd::mesConnect()
{
	//todo......
// 	if(���ӳɹ�){
// 		emit loginStatus(true);
// 	}
}

void ParsingCmd::ParsCmd(uint32_t testname, uint32_t command, uint32_t value)
{
	std::string str;
	if(command == A_TEST){
		switch (testname)
		{
		case CMD_DEVIINFO:		str = TS_GetDeviceInfo();			break;
		case CMD_URL:			str = TS_GetURLInfo();				break;
		case CMD_KEYREAD:		str = TS_SetOperatorsInfo();		break;
		case CMD_EXITTEST:		str = TS_SetTestMode();				break;
		case CMD_KEYWRITE:		str = TS_GetOperatorsInfo();		break;
		case CMD_SETDEFAULT:	str = TS_SetDefault();				break;
		case CMD_WHITELIGHT:	str = TS_WhiteLight();				break;
		case CMD_IRLIGHT:		str = TS_InfraredLamp();			break;
		case CMD_IRCUT:			str = TS_IRCUT();					break;
		case CMD_NETLED:		str = TS_LEDLights();				break;
		case CMD_SPEAKERTEST:	str = TS_Speaker();					break;
		case CMD_MICTEST:		str = TS_MIC();						break;
		case CMD_IRNVTEST:		str = TS_InfraredVison();			break;
		case CMD_FULLCOLORNV:	str = TS_FullcolorVison();			break;
		case CMD_KEYTEST:		str = TS_RestButton();				break;
		case CMD_TFTEST:		str = TS_TFCard();					break;
		case CMD_LDRTEST:		str = TS_Photosensitive();			break;
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
		case CMD_TFTEST:		str = RS_TFCard();					break;
		case CMD_LDRTEST:		str = RS_Photosensitive();			break;
		default:
			break;
		}
	}
	if (m_tcpClient != nullptr) {
		m_tcpClient->send_cmd(const_cast<char*>(str.c_str()), str.length());
	}
	// 		//�������ݷ����л�
// 		IM::Apacket apacketinfo;
// 		bool bl = apacketinfo.ParseFromString(str);
// 
// 		if (apacketinfo.msg().result() == "")
// 		{
// 			int m = 0;
// 		}
// 		if(apacketinfo.has_data()){
// 		qDebug("command = %x, testname = %s ,result = %s,num = %d ,name = %s,value = %s", 
// 				apacketinfo.msg().command(),apacketinfo.msg().testname().c_str(),
// 				apacketinfo.msg().result().c_str(),apacketinfo.data().num(),
// 				apacketinfo.data().value(0).name().c_str(),
// 				apacketinfo.data().value(0).value().c_str()
// 			);
// 		}
}

void ParsingCmd::UnParsCmd(char* p, std::size_t& len, boost::asio::ip::tcp::endpoint& ep)
{
	p[len] = '\0';
	std::string str = p;
	IM::Apacket apacketinfo;
	if (apacketinfo.ParseFromString(str)) {
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

	if (0 == id)
	{
		r_str.append("�ѳɹ�����");
		if (nullptr == m_ptrGloble)
			return;
		m_ptrGloble->m_stCommonInfo.iLostPack = 0;
		m_ptrGloble->m_stCommonInfo.itcpConn = 1;//TCP���ӳɹ�
		emit tcpConn(true);  //���ӳɹ������µ�¼����<����>����ɫ
		if (E_Login_OK == (eLoginState)(GlobleVar::GetInstance()->m_iLogin))
			emit updateConn();
	}
	else if (4 == id)
	{
		m_ptrGloble->m_stCommonInfo.itcpConn = 0;//TCP����ʧ��
		r_str.append("�ѶϿ�����");
		if (GlobleVar::GetInstance()->m_iLogin) //δ��¼
			emit tcpConn(false);
	}
	else if (6 == id)
	{
		m_ptrGloble->m_stCommonInfo.itcpConn = 0;//TCP����ʧ��
		r_str.append("���ӳ�ʱ��������...");
		m_tcpClient->connect();
	}
	else r_str.append(str);

	r_str.append(m_ptrGloble->m_stCommonInfo.strRemoteIP);
	m_ptrGloble->m_stCommonInfo.strconnect = r_str;
}

///�������ָ���IPC
///��ȡ��Ϣ
//��ȡ�豸��Ϣ
std::string ParsingCmd::TS_GetDeviceInfo()
{
	uint32_t command = A_TEST;
	std::string testname = "devinfo";
	
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();	
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);
//���Դ���
// 	IM::deviceinfo *_devinfo = new IM::deviceinfo();
// 	IM::info_s *_value = _devinfo->add_value();
// 	std::string _name = "SN";
// 	std::string _myvalue = "1181181";
// 	_value->set_name(_name);
// 	_value->set_value(_myvalue);
// 	_devinfo->set_num(1);
// 	apacket.set_allocated_data(_devinfo);
//���Թرմ��ڴ���
//	emit manTimeout(5);
	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//��ȡֱ�����ŵ�ַ
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
//����Ӫ����Ϣ
std::string ParsingCmd::TS_SetOperatorsInfo()
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
//�˳�����ģʽ
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

///������Ϣ
//д��Ӫ����Ϣ
std::string ParsingCmd::TS_GetOperatorsInfo()
{
	uint32_t command = A_TEST;
	std::string testname = "keywrite";
	IM::Apacket apacket;
	IM::Amessage *msg = new IM::Amessage();
	msg->set_command(command);
	msg->set_testname(testname);
	apacket.set_allocated_msg(msg);

	std::string str;
	apacket.SerializeToString(&str);
	return str;
}
//�ָ���������
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

///�ֶ�����
//���ذ׹��
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
//���غ����
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
//����IRCUT
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
//��������LED��
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
//����������
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
//������˷�
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
//���Ժ���ҹ��
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
//����ȫ��ҹ��
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

///�Զ�����
//����Rest����
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
//����TF��
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
//���Թ���
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
//���볧��ģʽ
//����PTZ���	���ݲ���ӣ�

///������ظ�IPC(�ֶ�)
std::string ParsingCmd::RS_WhiteLight(uint32_t value)
{
	uint32_t command = A_RESULT;
	std::string testname = "whitelight";
	std::string result = std::to_string(value);
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
	std::string result = std::to_string(value);
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
	std::string result = std::to_string(value);
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
	std::string result = std::to_string(value);
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
	std::string result = std::to_string(value);
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
	std::string result = std::to_string(value);
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
	std::string result = std::to_string(value);
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
	std::string result = std::to_string(value);
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

///������ظ�IPC(�Զ�)
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

///�ֽ����
//��ȡ��Ϣ
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
}
//������Ϣ
void ParsingCmd::Rec_SetInfo(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().testname() == "keywrite") {
		IPC_RS_Keywrite(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "setdefault") {
		IPC_RS_Setdefault(apacketinfo);
	}
}
//�ֶ�����
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
}
//�Զ�����
void ParsingCmd::Rec_AutoTest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().testname() == "keytest") {
		IPC_RS_Keytest(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "tftest") {
		IPC_RS_TFtest(apacketinfo);
	}
	else if (apacketinfo.msg().testname() == "ldrtest") {
		IPC_RS_Ldrtest(apacketinfo);
	}
}

///IPC�ظ����Խ����PC
//��ȡ
void ParsingCmd::IPC_RS_Devinfo(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		QMap<std::string, std::string> _map;
		if(apacketinfo.has_data()){
			for (int i = 0; i < apacketinfo.data().num(); i++) {
				_map.insert(apacketinfo.data().value(i).name(), apacketinfo.data().value(i).value());
			}
		}
		//todo... up to GUI
	}
}

void ParsingCmd::IPC_RS_Url(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		QMap<std::string, std::string> _map;
		if (apacketinfo.has_data()) {
			for (int i = 0; i < apacketinfo.data().num(); i++) {
				_map.insert(apacketinfo.data().value(i).name(), apacketinfo.data().value(i).value());
			}
		}
		//todo... up to GUI
	}
}

void ParsingCmd::IPC_RS_Keyread(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		QMap<std::string, std::string> _map;
		if (apacketinfo.has_data()) {
			for (int i = 0; i < apacketinfo.data().num(); i++) {
				_map.insert(apacketinfo.data().value(i).name(), apacketinfo.data().value(i).value());
			}
		}
		//todo... up to GUI
	}
}

void ParsingCmd::IPC_RS_Exittest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		QMap<std::string, std::string> _map;
		if (apacketinfo.has_data()) {
			for (int i = 0; i < apacketinfo.data().num(); i++) {
				_map.insert(apacketinfo.data().value(i).name(), apacketinfo.data().value(i).value());
			}
		}
		//todo... up to GUI
	}
}
//����
void ParsingCmd::IPC_RS_Keywrite(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		QMap<std::string, std::string> _map;
		if (apacketinfo.has_data()) {
			for (int i = 0; i < apacketinfo.data().num(); i++) {
				_map.insert(apacketinfo.data().value(i).name(), apacketinfo.data().value(i).value());
			}
		}
		//todo... up to GUI
	}
}

void ParsingCmd::IPC_RS_Setdefault(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		QString result = QString::fromStdString(apacketinfo.msg().result());
		//todo... up to GUI
	}
}
//�ֶ�����
void ParsingCmd::IPC_RS_Whitelight(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != "") {
			//todo..up result to GUI
			emit manResult(QString::fromStdString(apacketinfo.msg().testname()), QString::fromStdString(apacketinfo.msg().result()));
		}
		else {
			//todo..up timeout to GUI
			emit manTimeout(QString::fromStdString(apacketinfo.msg().testname()), apacketinfo.msg().timeout());
		}
	}
}

void ParsingCmd::IPC_RS_Irlight(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != "") {
			//todo..up result to GUI
		}
		else {
			//todo..up timeout to GUI
		}
	}
}

void ParsingCmd::IPC_RS_Ircut(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != "") {
			//todo..up result to GUI
		}
		else {
			//todo..up timeout to GUI
		}
	}
}

void ParsingCmd::IPC_RS_Netled(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != "") {
			//todo..up result to GUI
		}
		else {
			//todo..up timeout to GUI
		}
	}
}

void ParsingCmd::IPC_RS_Speakertest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != "") {
			//todo..up result to GUI
		}
		else {
			//todo..up timeout to GUI
		}
	}
}

void ParsingCmd::IPC_RS_Mictest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != "") {
			//todo..up result to GUI
		}
		else {
			//todo..up timeout to GUI
		}
	}
}

void ParsingCmd::IPC_RS_Irnvtest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != "") {
			//todo..up result to GUI
		}
		else {
			//todo..up timeout to GUI
		}
	}
}

void ParsingCmd::IPC_RS_Fullcolornv(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		if (apacketinfo.msg().result() != "") {
			//todo..up result to GUI
		}
		else {
			//todo..up timeout to GUI
		}
	}
}
//�Զ�����
void ParsingCmd::IPC_RS_Keytest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		//todo..up timeout to GUI
		emit autoTimeout(QString::fromStdString(apacketinfo.msg().testname()),apacketinfo.msg().timeout());
	}
	else if(apacketinfo.msg().command() == A_RESULT){
		//todo..up result to GUI
		emit autoResult(QString::fromStdString(apacketinfo.msg().testname()), QString::fromStdString(apacketinfo.msg().result()));
	}
}

void ParsingCmd::IPC_RS_TFtest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		//todo..up timeout to GUI
	}
	else if (apacketinfo.msg().command() == A_RESULT) {
		//todo..up result to GUI
	}
}

void ParsingCmd::IPC_RS_Ldrtest(IM::Apacket apacketinfo)
{
	if (apacketinfo.msg().command() == A_OKAY) {
		//todo..up timeout to GUI
	}
	else if (apacketinfo.msg().command() == A_RESULT) {
		//todo..up result to GUI
	}
}