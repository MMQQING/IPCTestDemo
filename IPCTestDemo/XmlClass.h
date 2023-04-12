#pragma once
#include <memory>
#include <string>

using namespace std;
class XmlClass
{
public:
	XmlClass(std::string str);
	~XmlClass();

	//配置客户端参数
	void ReadParaXml();
	void WriteParaXml();
	void ReadListXml();
	void WriteListXml();

private:
	std::string strfilename;

private:
	void GenerateFile(std::string str);
};

