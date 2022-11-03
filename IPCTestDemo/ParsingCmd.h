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
	//��������
	void ParsCmd(uint32_t testname, uint32_t command, uint32_t value = 0);
	//���ݽ��յĻص�
	void UnParsCmd(char* p, std::size_t& len, boost::asio::ip::tcp::endpoint& ep);
	//ָ����յĻص�������ָ��ִ�гɹ�/ʧ�ܣ�ʧ��ͬʱ����ʧ�ܾ���������
	void getMsg(int id, std::string& str, std::vector<boost::asio::ip::tcp::endpoint>& vEp);

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
	//����PTZ���	���ݲ���ӣ�
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
///������ظ�IPC(�Զ�)
	//����Rest����
	std::string RS_RestButton();
	//����TF��
	std::string RS_TFCard();
	//���Թ���
	std::string RS_Photosensitive();
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
	//����
	void IPC_RS_Keywrite(IM::Apacket apacketinfo);
	void IPC_RS_Setdefault(IM::Apacket apacketinfo);
	//�ֶ�����
	void IPC_RS_Whitelight(IM::Apacket apacketinfo);
	void IPC_RS_Irlight(IM::Apacket apacketinfo);
	void IPC_RS_Ircut(IM::Apacket apacketinfo);
	void IPC_RS_Netled(IM::Apacket apacketinfo);
	void IPC_RS_Speakertest(IM::Apacket apacketinfo);
	void IPC_RS_Mictest(IM::Apacket apacketinfo);
	void IPC_RS_Irnvtest(IM::Apacket apacketinfo);
	void IPC_RS_Fullcolornv(IM::Apacket apacketinfo);
	//�Զ�����
	void IPC_RS_Keytest(IM::Apacket apacketinfo);
	void IPC_RS_TFtest(IM::Apacket apacketinfo);
	void IPC_RS_Ldrtest(IM::Apacket apacketinfo);
signals:
	void updateConn();			//TCP���ӳɹ���Ϣ����������������
	void tcpConn(bool b);       //��¼ǰ��TCP����, ���ڵ�¼������ʾ���ӳɹ����
	void loginStatus(int istatus);
	void mesloginStatus(bool istatus);//mes��¼״̬
	///IPC�������GUI
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
