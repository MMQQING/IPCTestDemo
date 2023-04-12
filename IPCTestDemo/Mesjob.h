#pragma once
#include <QString>
#include <QObject>
#include "include/httplib.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "structdef.h"
#include <QVariant>
class Mesjob : public QObject
{
	Q_OBJECT
public:
	Mesjob();
	~Mesjob();
private:
	boost::property_tree::ptree parse_res(std::string str);
	std::string solve(std::string &str1, std::string &str2);
	std::string UTF8_To_String(std::string str);
public:
	bool common_login();
	//void common_get20info(QString SNstr);
	void common_get20info(QString);
	void common_get40info();
	void common_send30info(int bl, QString str, QString errstr);
signals:
	void mesConn(bool b, QString str);				//Mes��¼���ӳɹ���Ϣ
	void mes20Res(bool, QString, QVariant);			//Mes20������Ϣ
	void mes30Res(bool, QString);					//Mes30������Ϣ
	void mes40Res(bool, QString, QVariant);			//Mes40������Ϣ
private:
	std::shared_ptr<httplib::Client>	cli;
	st_20_result						m_st20Info;						//mes20�ӿڻظ���Ϣ
	st_40_result						m_st40Info;						//mes40�ӿڻظ���Ϣ
};