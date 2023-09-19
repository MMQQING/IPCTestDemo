#pragma once
#include "TCPClient.h"
#include "IPCMessage.pb.h"
#include "threadpool/ThreadPool.h"
#include <QMap>
#include <QObject>
#include <QVariant>
#include <QMetaType>
#include "structdef.h"
//class GlobleVar;
class ParsingCmd : public QObject
{
		Q_OBJECT
public:
	ParsingCmd();
	~ParsingCmd();

	void init();
	void tcpConnect(int);
	void tcpDisConnect();
	//void mesConnect();
	//发送命令
	void ParsCmd(uint32_t testname, uint32_t command, uint32_t value = 0);
	//数据接收的回调
	void UnParsCmd(char* p, std::size_t& len, boost::asio::ip::tcp::endpoint& ep);
	//指令接收的回调（返回指令执行成功/失败，失败同时返回失败具体描述）
	void getMsg(int id, std::string& str, std::vector<boost::asio::ip::tcp::endpoint>& vEp);
	//设备连接到WiFi站点
	void setWiFi(QString, QString);
	void getWiFi();
private:

private:
///测试项发送指令给IPC
	//进入厂测模式
	//void SetTestMode();
	//获取设备信息
	std::string TS_GetDeviceInfo();
	//获取直播播放地址
	std::string TS_GetURLInfo();
	//开关白光灯
	std::string TS_WhiteLight();
	//开关红外灯
	std::string TS_InfraredLamp();
	//开关IRCUT
	std::string TS_IRCUT();
	//开关网络LED灯
	std::string TS_LEDLights();
	//测试扬声器
	std::string TS_Speaker();
	//测试麦克风
	std::string TS_MIC();
	//测试Rest按键
	std::string TS_RestButton();
	//测试通话按键
	std::string TS_CallButton();
	//测试TF卡
	std::string TS_TFCard();
	//测试光敏
	std::string TS_Photosensitive();
	//测试红外夜视
	std::string TS_InfraredVison();
	//测试全彩夜视
	std::string TS_FullcolorVison();
	//读写运营商信息
	std::string TS_SetOperatorsInfo();
	std::string TS_GetOperatorsInfo();
	//恢复出厂设置
	std::string TS_SetDefault();
	//设置时间
	std::string TS_TimeWrite();
	//退出厂测模式
	std::string TS_SetTestMode();
	//PTZ马达测试
	std::string TS_PtzMotor();
	//flash数据校验
	std::string TS_FlashVerify();
	//测试跑马灯
	std::string TS_Marquee();
	//PTZ马达移动
	std::string PTZ_UP();
	std::string	PTZ_DOWN();
	std::string	PTZ_LEFT();
	std::string	PTZ_RIGHT();
	std::string	PTZ_GetLogo();
	//获取wifi站点测试
	std::string TS_WiFiScanResult();
	//影像确认项（人工查看图像）
	void TS_Video();
	//退出批量升级模式
	std::string TS_ReSet();
private:
///测试项回复IPC(手动)
	std::string RS_WhiteLight(uint32_t value);
	//开关红外灯
	std::string RS_InfraredLamp(uint32_t value);
	//开关IRCUT
	std::string RS_IRCUT(uint32_t value);
	//开关网络LED灯
	std::string RS_LEDLights(uint32_t value);
	//测试扬声器
	std::string RS_Speaker(uint32_t value);
	//测试麦克风
	std::string RS_MIC(uint32_t value);
	//测试红外夜视
	std::string RS_InfraredVison(uint32_t value);
	//测试全彩夜视
	std::string RS_FullcolorVison(uint32_t value);
	//测试跑马灯
	std::string RS_Marquee(uint32_t value);
///测试项回复IPC(自动)
	//测试Rest按键
	std::string RS_RestButton();
	//测试通话按键
	std::string RS_CallButton();
	//测试TF卡
	std::string RS_TFCard();
	//flash数据校验
	std::string RS_FlashVerify();
	//测试光敏
	std::string RS_Photosensitive();
	//PTZ马达测试
	//std::string RS_PtzMotor(uint32_t value);
	std::string RS_PtzMotor();
	//影像确认项（人工查看图像）
	void RS_Video(uint32_t value);
private:
	void Rec_GetInfo(IM::Apacket apacketinfo);
	void Rec_SetInfo(IM::Apacket apacketinfo);
	void Rec_ManTest(IM::Apacket apacketinfo);
	void Rec_AutoTest(IM::Apacket apacketinfo);
private:
///IPC回复测试结果给PC
	//读取
	void IPC_RS_Devinfo(IM::Apacket apacketinfo);
	void IPC_RS_Url(IM::Apacket apacketinfo);
	void IPC_RS_Keyread(IM::Apacket apacketinfo);
	void IPC_RS_Exittest(IM::Apacket apacketinfo);
	void IPC_RS_PTZatlogo(IM::Apacket apacketinfo);
	void IPC_RS_GetLogo(IM::Apacket apacketinfo);
	void IPC_SetLogo(QMap<QString, QString> map);
	void IPC_RS_GetWiFiSite(IM::Apacket apacketinfo);
	void IPC_RS_GetWiFiInfo(IM::Apacket apacketinfo);

signals:
	void updateConn(bool);						//TCP连接状态
	void tcpConn(bool, QString);				//登录前，TCP连接, 用于登录界面显示连接成功与否
	void loginStatus(int istatus);
	///IPC结果返回GUI
	//todo.....
	void manTimeout(QString testname, int time);
	void manResult(QString testname, int result);
	void autoTimeout(QString testname,int time);
	void autoResult(QString testname, int result);

	void myResult(QString testname, int result);
	void message(QVariant, QString);
	void showMovingWidget();
	void WiFiSiteInfo(QVariant);
	void RsWiFiInfo(QVariant);
public slots:
	void updateWriteInfo(QVariant);
private:
	std::shared_ptr<TCPClient>					m_tcpClient;
	//std::shared_ptr<GlobleVar>					m_ptrGloble;
	st_common_info								m_stCommonInfo;                 //通用信息
	uint8_t										m_iLogin;                       //是否登录成功
	st_40_result								m_writeInfo;					//写入信息
};
