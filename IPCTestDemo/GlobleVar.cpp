#include "GlobleVar.h"
#include "XmlClass.h"

GlobleVar::GlobleVar()
{

}

void GlobleVar::init()
{
	m_stCommonInfo.iLostPack = 0;
	m_iLogin = 7;
	m_strXmlCfgPath = "./config/Para.xml";
	loadpara(m_strXmlCfgPath.toStdString());
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
	else if (str.compare("tftest") == 0) {
		return CMD_TFTEST;
	}
	else if (str.compare("ldrtest") == 0) {
		return CMD_LDRTEST;
	}
	else
	{
		return -1;
	}
}

void GlobleVar::setCmdObject(std::shared_ptr<ParsingCmd> cmd)
{
	m_cmdObject = cmd;
}

void GlobleVar::savepara(std::string str)
{
	XmlClass xml(m_strXmlCfgPath.toStdString());
	xml.WriteParaXml();
}

void GlobleVar::loadpara(std::string str)
{
	XmlClass xml(m_strXmlCfgPath.toStdString());
	xml.ReadParaXml();
}