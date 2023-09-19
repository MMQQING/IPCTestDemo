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
	//��������
	void ParsCmd(uint32_t testname, uint32_t command, uint32_t value = 0);
	//���ݽ��յĻص�
	void UnParsCmd(char* p, std::size_t& len, boost::asio::ip::tcp::endpoint& ep);
	//ָ����յĻص�������ָ��ִ�гɹ�/ʧ�ܣ�ʧ��ͬʱ����ʧ�ܾ���������
	void getMsg(int id, std::string& str, std::vector<boost::asio::ip::tcp::endpoint>& vEp);
	//�豸���ӵ�WiFiվ��
	void setWiFi(QString, QString);
	void getWiFi();
private:

private:
///�������ָ���IPC
	//���볧��ģʽ
	//void SetTestMode();
	//��ȡ�豸��Ϣ
	std::string TS_GetDeviceInfo();
	//��ȡֱ�����ŵ�ַ
	std::string TS_GetURLInfo();
	//���ذ׹��
	std::string TS_WhiteLight();
	//���غ����
	std::string TS_InfraredLamp();
	//����IRCUT
	std::string TS_IRCUT();
	//��������LED��
	std::string TS_LEDLights();
	//����������
	std::string TS_Speaker();
	//������˷�
	std::string TS_MIC();
	//����Rest����
	std::string TS_RestButton();
	//����ͨ������
	std::string TS_CallButton();
	//����TF��
	std::string TS_TFCard();
	//���Թ���
	std::string TS_Photosensitive();
	//���Ժ���ҹ��
	std::string TS_InfraredVison();
	//����ȫ��ҹ��
	std::string TS_FullcolorVison();
	//��д��Ӫ����Ϣ
	std::string TS_SetOperatorsInfo();
	std::string TS_GetOperatorsInfo();
	//�ָ���������
	std::string TS_SetDefault();
	//����ʱ��
	std::string TS_TimeWrite();
	//�˳�����ģʽ
	std::string TS_SetTestMode();
	//PTZ������
	std::string TS_PtzMotor();
	//flash����У��
	std::string TS_FlashVerify();
	//���������
	std::string TS_Marquee();
	//PTZ����ƶ�
	std::string PTZ_UP();
	std::string	PTZ_DOWN();
	std::string	PTZ_LEFT();
	std::string	PTZ_RIGHT();
	std::string	PTZ_GetLogo();
	//��ȡwifiվ�����
	std::string TS_WiFiScanResult();
	//Ӱ��ȷ����˹��鿴ͼ��
	void TS_Video();
	//�˳���������ģʽ
	std::string TS_ReSet();
private:
///������ظ�IPC(�ֶ�)
	std::string RS_WhiteLight(uint32_t value);
	//���غ����
	std::string RS_InfraredLamp(uint32_t value);
	//����IRCUT
	std::string RS_IRCUT(uint32_t value);
	//��������LED��
	std::string RS_LEDLights(uint32_t value);
	//����������
	std::string RS_Speaker(uint32_t value);
	//������˷�
	std::string RS_MIC(uint32_t value);
	//���Ժ���ҹ��
	std::string RS_InfraredVison(uint32_t value);
	//����ȫ��ҹ��
	std::string RS_FullcolorVison(uint32_t value);
	//���������
	std::string RS_Marquee(uint32_t value);
///������ظ�IPC(�Զ�)
	//����Rest����
	std::string RS_RestButton();
	//����ͨ������
	std::string RS_CallButton();
	//����TF��
	std::string RS_TFCard();
	//flash����У��
	std::string RS_FlashVerify();
	//���Թ���
	std::string RS_Photosensitive();
	//PTZ������
	//std::string RS_PtzMotor(uint32_t value);
	std::string RS_PtzMotor();
	//Ӱ��ȷ����˹��鿴ͼ��
	void RS_Video(uint32_t value);
private:
	void Rec_GetInfo(IM::Apacket apacketinfo);
	void Rec_SetInfo(IM::Apacket apacketinfo);
	void Rec_ManTest(IM::Apacket apacketinfo);
	void Rec_AutoTest(IM::Apacket apacketinfo);
private:
///IPC�ظ����Խ����PC
	//��ȡ
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
	void updateConn(bool);						//TCP����״̬
	void tcpConn(bool, QString);				//��¼ǰ��TCP����, ���ڵ�¼������ʾ���ӳɹ����
	void loginStatus(int istatus);
	///IPC�������GUI
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
	st_common_info								m_stCommonInfo;                 //ͨ����Ϣ
	uint8_t										m_iLogin;                       //�Ƿ��¼�ɹ�
	st_40_result								m_writeInfo;					//д����Ϣ
};
