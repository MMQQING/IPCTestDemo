#include "GlobleVar.h"
#include "XmlClass.h"

GlobleVar::GlobleVar()
{

}

void GlobleVar::init()
{
	m_stCommonInfo.iLostPack = 0;
	//m_iLogin = 7;
	m_strXmlCfgPath = "./config/Para.xml";
	m_strXmlListPath = "./config/TestList.xml";
	loadpara(m_strXmlCfgPath.toStdString());
	loadlist(m_strXmlListPath.toStdString());
}

int GlobleVar::comparTestname(QString str)
{
	if (str.compare("devinfo") == 0) {
		return CMD_DEVIINFO;
	}
	else if (str.compare("url") == 0) {
		return CMD_URL;
	}
	else if (str.compare("keyread") == 0) {
		return CMD_KEYREAD;
	}
	else if (str.compare("exittest") == 0) {
		return CMD_EXITTEST;
	}
	else if (str.compare("keywrite") == 0) {
		return CMD_KEYWRITE;
	}
	else if (str.compare("setdefault") == 0) {
		return CMD_SETDEFAULT;
	}
	else if (str.compare("whitelight") == 0) {
		return CMD_WHITELIGHT;
	}
	else if (str.compare("irlight") == 0) {
		return CMD_IRLIGHT;
	}
	else if (str.compare("ircut") == 0) {
		return CMD_IRCUT;
	}
	else if (str.compare("netled") == 0) {
		return CMD_NETLED;
	}
	else if (str.compare("speakertest") == 0) {
		return CMD_SPEAKERTEST;
	}
	else if (str.compare("mictest") == 0) {
		return CMD_MICTEST;
	}
	else if (str.compare("irnvtest") == 0) {
		return CMD_IRNVTEST;
	}
	else if (str.compare("fullcolornv") == 0) {
		return CMD_FULLCOLORNV;
	}
	else if (str.compare("keytest") == 0) {
		return CMD_KEYTEST;
	}
	else if (str.compare("voipkeytest") == 0) {
		return CMD_VOIPKEYTEST;
	}
	else if (str.compare("tftest") == 0) {
		return CMD_TFTEST;
	}
	else if (str.compare("ldrtest") == 0) {
		return CMD_LDRTEST;
	}
	else if (str.compare("ptztest") == 0) {
		return CMD_PTZTEST;
	}
	else if (str.compare("wifiscanresults") == 0) {
		return CMD_WIFISCANRESULT;
	}
	else if (str.compare("video") == 0) {
		return CMD_VIDEO;
	}
	else
	{
		return -1;
	}
}

QString GlobleVar::TestProjectName(QString str)
{
	if (str.compare("devinfo") == 0) {
		return QString::fromLocal8Bit("获取设备信息");
	}
	else if (str.compare("url") == 0) {
		return QString::fromLocal8Bit("获取直播播放地址");
	}
	else if (str.compare("keyread") == 0) {
		return QString::fromLocal8Bit("读key");
	}
	else if (str.compare("exittest") == 0) {
		return QString::fromLocal8Bit("退出厂测模式");
	}
	else if (str.compare("keywrite") == 0) {
		return QString::fromLocal8Bit("写key");
	}
	else if (str.compare("setdefault") == 0) {
		return QString::fromLocal8Bit("恢复出厂设置");
	}
	else if (str.compare("whitelight") == 0) {
		return QString::fromLocal8Bit("开关白光灯");
	}
	else if (str.compare("irlight") == 0) {
		return QString::fromLocal8Bit("开关红外灯");
	}
	else if (str.compare("ircut") == 0) {
		return QString::fromLocal8Bit("开关IRCUT");
	}
	else if (str.compare("netled") == 0) {
		return QString::fromLocal8Bit("开关网络LED灯");
	}
	else if (str.compare("speakertest") == 0) {
		return QString::fromLocal8Bit("测试扬声器");
	}
	else if (str.compare("mictest") == 0) {
		return QString::fromLocal8Bit("测试麦克风");
	}
	else if (str.compare("irnvtest") == 0) {
		return QString::fromLocal8Bit("测试红外夜视");
	}
	else if (str.compare("fullcolornv") == 0) {
		return QString::fromLocal8Bit("测试全彩夜视");
	}
	else if (str.compare("keytest") == 0) {
		return QString::fromLocal8Bit("测试rest按键");
	}
	else if (str.compare("voipkeytest") == 0) {
		return QString::fromLocal8Bit("测试通话按键");
	}
	else if (str.compare("tftest") == 0) {
		return QString::fromLocal8Bit("测试TF卡");
	}
	else if (str.compare("ldrtest") == 0) {
		return QString::fromLocal8Bit("测试光敏");
	}
	else if (str.compare("ptztest") == 0) {
		return QString::fromLocal8Bit("ptz马达复位");
	}
	else if (str.compare("video") == 0) {
		return QString::fromLocal8Bit("图像清晰度及脏点确认");
	}
	else
	{
		return "";
	}
}

void GlobleVar::setCmdObject(std::shared_ptr<ParsingCmd> cmd)
{
	m_cmdObject = cmd;
}

void GlobleVar::savepara(std::string str)
{
	XmlClass xml(str);
	xml.WriteParaXml();
}

void GlobleVar::savelist(std::string str)
{
	XmlClass xml(str);
	xml.WriteListXml();
}

void GlobleVar::loadpara(std::string str)
{
	XmlClass xml(str);
	xml.ReadParaXml();
}

void GlobleVar::loadlist(std::string str)
{
	XmlClass xml(str);
	xml.ReadListXml();
}
