#pragma once
#include <memory>
#include <QString>
#include "structdef.h"
#include "Cpclog.h"
#include "ParsingCmd.h"
#include "circular_buffer.hpp"
#include "condition_variable.hpp"

//#define MES

const QString  Normal = "#64A600";
const QString  UNNormal = "#A23400";
const QString  InitColor = "#535353";

using namespace ptool;
typedef struct st_buffer_data {
	st_buffer_data(uint32_t iLen) {
		ptrData = std::shared_ptr<char>(new char[iLen]);
		len = 0;
	}
	std::shared_ptr<char>  ptrData;
	int32_t				   len;
}st_buffer_data, *pst_buffer_data;
typedef boost::shared_ptr<buffer::circular_buffer<st_buffer_data>>				datacircular_ptr;
typedef boost::shared_ptr<st_buffer_data>										dataelement_ptr;

class ParsingCmd;
 class GlobleVar
 {
 private:
	 typedef GlobleVar								this_type;
	 typedef std::shared_ptr<this_type>				ptr_this;
	 GlobleVar();
	 void init();

 public:
	 //创建单例对象
	 static ptr_this  GetInstance() {
		 static ptr_this m_ptrInstance = nullptr;
		 if (nullptr == m_ptrInstance) {
			 m_ptrInstance = std::shared_ptr<GlobleVar>(new GlobleVar());
			 m_ptrInstance->init();
		 }
		 return m_ptrInstance;
	 }
	 void setCmdObject(std::shared_ptr<ParsingCmd> cmd);
	 void savepara(std::string str);
	 void savelist(std::string str);
	 void loadpara(std::string str);
	 void loadlist(std::string str);
	 int comparTestname(QString str);
	 QString TestProjectName(QString str);

	 std::shared_ptr<ParsingCmd>	    m_cmdObject;
	 st_common_info						m_stCommonInfo;                 //通用信息
	 
	 QString                            m_strXmlCfgPath;                //即时配置文件路径
	 QString                            m_strXmlListPath;               //测试项配置文件路径
	 st_login_result					m_stLoginInfo;					//登录回复信息
	 
	 
	std::vector<std::string>			m_testListA;					//测试项集合
	std::vector<std::string>			m_testListB;					//未测试项集合
 };
#define  gVar GlobleVar::GetInstance()