#pragma once
#include <QString>
#include <QObject>
#include "include/httplib.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
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
	void mesConn(bool b, QString str);//Mes登录连接成功信息
	void mes20Res(bool b, QString str);//Mes20返回信息
	void mes40Res(bool b, QString str);//Mes40返回信息
private:
	std::shared_ptr<httplib::Client> cli;
};