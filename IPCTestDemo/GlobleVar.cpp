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
		return QString::fromLocal8Bit("��ȡ�豸��Ϣ");
	}
	else if (str.compare("url") == 0) {
		return QString::fromLocal8Bit("��ȡֱ�����ŵ�ַ");
	}
	else if (str.compare("keyread") == 0) {
		return QString::fromLocal8Bit("��key");
	}
	else if (str.compare("exittest") == 0) {
		return QString::fromLocal8Bit("�˳�����ģʽ");
	}
	else if (str.compare("keywrite") == 0) {
		return QString::fromLocal8Bit("дkey");
	}
	else if (str.compare("setdefault") == 0) {
		return QString::fromLocal8Bit("�ָ���������");
	}
	else if (str.compare("whitelight") == 0) {
		return QString::fromLocal8Bit("���ذ׹��");
	}
	else if (str.compare("irlight") == 0) {
		return QString::fromLocal8Bit("���غ����");
	}
	else if (str.compare("ircut") == 0) {
		return QString::fromLocal8Bit("����IRCUT");
	}
	else if (str.compare("netled") == 0) {
		return QString::fromLocal8Bit("��������LED��");
	}
	else if (str.compare("speakertest") == 0) {
		return QString::fromLocal8Bit("����������");
	}
	else if (str.compare("mictest") == 0) {
		return QString::fromLocal8Bit("������˷�");
	}
	else if (str.compare("irnvtest") == 0) {
		return QString::fromLocal8Bit("���Ժ���ҹ��");
	}
	else if (str.compare("fullcolornv") == 0) {
		return QString::fromLocal8Bit("����ȫ��ҹ��");
	}
	else if (str.compare("keytest") == 0) {
		return QString::fromLocal8Bit("����rest����");
	}
	else if (str.compare("voipkeytest") == 0) {
		return QString::fromLocal8Bit("����ͨ������");
	}
	else if (str.compare("tftest") == 0) {
		return QString::fromLocal8Bit("����TF��");
	}
	else if (str.compare("ldrtest") == 0) {
		return QString::fromLocal8Bit("���Թ���");
	}
	else if (str.compare("ptztest") == 0) {
		return QString::fromLocal8Bit("ptz��︴λ");
	}
	else if (str.compare("video") == 0) {
		return QString::fromLocal8Bit("ͼ�������ȼ����ȷ��");
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
