#pragma once
#include <memory>
#include <string>

using namespace std;
class XmlClass
{
public:
	XmlClass(std::string str);
	~XmlClass();

	//���ÿͻ��˲���
	void ReadParaXml();
	void WriteParaXml();

private:
	std::string strfilename;

private:
	void GenerateFile(std::string str);
};

