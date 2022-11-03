#pragma once
#include "TCPClient.h"
#include "IPCMessage.pb.h"
#include "threadpool/ThreadPool.h"
#include <QMap>
#include <QObject>
#include "Mesjob.h"
#include <QVariant>
#include <QMetaType>

class GlobleVar;
class ParsingCmd : public QObject
{
		Q_OBJECT
public:
	ParsingCmd();
	~ParsingCmd();

	void init();
	void tcpConnect();
	void tcpDisConnect();
	void mesConnect();
	//发送命令
	void ParsCmd(uint32_t testname, uint32_t command, uint32_t value = 0);
	//数据接收的回调
	void UnParsCmd(char* p, std::size_t& len, boost::asio::ip::tcp::endpoint& ep);
	//指令接收的回调（返回指令执行成功/失败，失败同时返回失败具体描述）
	void getMsg(int id, std::string& str, std::vector<boost::asio::ip::tcp::endpoint>& vEp);

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
	//测试PTZ马达	（暂不添加）
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
///测试项回复IPC(自动)
	//测试Rest按键
	std::string RS_RestButton();
	//测试TF卡
	std::string RS_TFCard();
	//测试光敏
	std::string RS_Photosensitive();
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
	//设置
	void IPC_RS_Keywrite(IM::Apacket apacketinfo);
	void IPC_RS_Setdefault(IM::Apacket apacketinfo);
	//手动测试
	void IPC_RS_Whitelight(IM::Apacket apacketinfo);
	void IPC_RS_Irlight(IM::Apacket apacketinfo);
	void IPC_RS_Ircut(IM::Apacket apacketinfo);
	void IPC_RS_Netled(IM::Apacket apacketinfo);
	void IPC_RS_Speakertest(IM::Apacket apacketinfo);
	void IPC_RS_Mictest(IM::Apacket apacketinfo);
	void IPC_RS_Irnvtest(IM::Apacket apacketinfo);
	void IPC_RS_Fullcolornv(IM::Apacket apacketinfo);
	//自动测试
	void IPC_RS_Keytest(IM::Apacket apacketinfo);
	void IPC_RS_TFtest(IM::Apacket apacketinfo);
	void IPC_RS_Ldrtest(IM::Apacket apacketinfo);
signals:
	void updateConn();			//TCP连接成功消息用于重连后发送命令
	void tcpConn(bool b);       //登录前，TCP连接, 用于登录界面显示连接成功与否
	void loginStatus(int istatus);
	void mesloginStatus(bool istatus);//mes登录状态
	///IPC结果返回GUI
	//todo.....
	void manTimeout(QString testname, int time);
	void manResult(QString testname, int result);
	void autoTimeout(QString testname,int time);
	void autoResult(QString testname, int result);

	void myResult(QString testname, int result);

	void message(QVariant, QString);
public:
	std::shared_ptr<Mesjob>						m_ptrMes;
private:
	std::shared_ptr<TCPClient>					m_tcpClient;
	std::shared_ptr<GlobleVar>					m_ptrGloble;
};
