#ifndef STRUCT_2009
#define STRUCT_2009
#include <string>
#include <cstdint>
#include <memory>
#include <vector>

#pragma pack(push, 1) 

//����Ŀ¼
const int WorkDirCount = 8;
const std::string strWorkDir[] = { "\\WorkDir", "\\IQ", "\\Audio", "\\Estimate", "\\Capture", "\\Bit" ,"\\Ddc", "\\Para" };

//���� command ֵ���£�
#define		A_TEST			0xabcdef01
#define		A_OKAY			0xabcdef02
#define		A_START			0xabcdef03
#define		A_RESULT		0xabcdef04
#define		MAX_PAYLOAD		244

#define		MAX_TURN_COUNT			 1
//��Ŀ����
// enum eProjectName
// {
// 	E_CPC2018 = 0,		
// 	E_CPC2024,
// 	E_CPC2031,
// 	E_CPC2032,
// 	E_CPC2108,
// 	E_CPC2026,
// 	E_CPC2028
// };
//��������
enum eCtrlCmd
{
	CMD_DEVIINFO = 0,		//��ȡ�豸��Ϣ
	CMD_URL,				//��ȡֱ�����ŵ�ַ
	CMD_KEYREAD,			//����Ӫ����Ϣ
	CMD_EXITTEST,			//�˳�����ģʽ
	CMD_SETTIME,			//����ʱ��
	CMD_KEYWRITE,			//д��Ӫ����Ϣ
	CMD_SETDEFAULT,			//�ָ���������
	CMD_WHITELIGHT,			//���ذ׹��
	CMD_IRLIGHT,			//���غ����
	CMD_IRCUT,				//����IRCUT
	CMD_NETLED,				//��������LED
	CMD_SPEAKERTEST,		//����������
	CMD_MICTEST,			//������˷�
	CMD_IRNVTEST,			//���Ժ���ҹ��
	CMD_FULLCOLORNV,		//����ȫ��ҹ��
	CMD_KEYTEST,			//����Rest����
	CMD_TFTEST,				//����TF��
	CMD_LDRTEST,			//���Թ���
	CMD_COUNT
};

const std::string strTcpCmd[] = {
	"��ȡ�豸��Ϣ","��ȡֱ�����ŵ�ַ","����Ӫ����Ϣ","�˳�����ģʽ","����ʱ��","д��Ӫ����Ϣ",
	"�ָ���������","���ذ׹��","���غ����","����IRCUT","��������LED","����������",
	"������˷�","���Ժ���ҹ��","����ȫ��ҹ��","����Rest����","����TF��","���Թ���"
};
//���ӷ�����״̬
enum eSerConnState
{
	E_CONN_OK = 0,		//���ӳɹ�
	E_CONN_DIS			//�Ͽ�����
};

//��¼״̬
enum eLoginState
{
	E_Login_OK = 0,     //�ɹ�
	E_Login_Name_Err,	//�û���������
	E_Login_Psw_Err,	//�������
	E_Login_Power_Err,	//Ȩ�޲���
	E_Login_Two_Err		//�˻��ѵ�¼
};
//type
enum eType{
	R_Get = 0x01,
	R_Set,
	R_AutoTest,
	R_ManTest
};
//result
enum eResult
{
	Result_Void=0x00,
	Result_Pass,
	Result_False,
	Result_Timeout,
	Result_Testing
};

const std::string strLogin[] = {
	"��¼�ɹ�", "�û���������", "�������", "Ȩ�޲���", "�˻��ѵ�¼"
};

//ͨ�ò���
typedef struct st_common_info {
	std::string     strWorkPath;                //����Ŀ¼
	std::string     strremotepath;              //Զ�˹���Ŀ¼
	uint8_t			uProjectType;               //��Ŀ����
	std::string     strUsername;                //�û���
	std::string     strPsw;                     //����
	uint8_t         iAccess;                    //Ȩ��
	std::string		iResCode;					//��Դ����
	std::string		strLocalIP;					//����IP��ַ 0:tcp 1:udp
	uint32_t		uLocalPort;					//���ض˿� 0:tcp 1:udp
	uint32_t		uRemotePort;				//����˶˿� 0:tcp 1:udp
	std::string		strRemoteIP;				//�����IP��ַ 0:tcp 1:udp
	uint8_t         iAutoLogin;                 //�Զ���¼
	std::string		strconnect;					//����״̬
	uint8_t         itcpConn;                   //TCP���ӳɹ� 1���ɹ� 0��ʧ��
	uint8_t         iudpConn;                   //UDP���ӳɹ� 1���ɹ� 0��ʧ��
	uint32_t		iLostPack;					//��������
	std::string		iWebHostPort;				//web��Ϣ
	std::string		iUrl;						//ֱ�����ŵ�ַ
	st_common_info() :itcpConn(0), iudpConn(0){};
}st_common_info, *pst_common_info;

//ͨѶ
typedef struct st_comm_info {
	std::string		strIP;				//IP��ַ
	uint32_t		uPort;				//ת���˿�
	bool            bstart;             //ת������
}st_comm_info, *pst_comm_info;

//��¼��Ϣ
typedef struct st_login_info {
	std::string				dbIP;
	std::string				dbuser;
	std::string				dbpasswd;
	std::string				dbname;
	uint16_t				dbport;
	st_login_info() :dbIP("192.168.1.49"), dbuser("root"), dbpasswd("Aa123456"), dbname("db_hys"), dbport(3306) {}
}st_login_info, *pst_login_info;
//��¼mes�ظ���Ϣ
typedef struct st_login_result {
	std::string				UserId;//�û�ID
	std::string				UserCode;//�û�����
	std::string				UserName;//�û�����
	std::string				ResId;//��ԴID
	std::string				ResCode;//��Դ����
	std::string				ResName;//��Դ����
	std::string				LineId;//�߱�ID
	std::string				LineCode;//�߱����
	std::string				LineName;//�߱�����
	std::string				ShiftTypeId;//����ID
	std::string				ShiftTypeCode;//���ƴ���
	std::string				ShiftTypeName;//��������
	std::string				OPId;//����Id
	std::string				OPCode;//�������
	std::string				OPName;//��������
	std::string				ReturnValue;//ִ�н��  0����ʾִ��ʧ��   1����ʾִ�гɹ�
	std::string				ReturnMessage;//ִ�гɹ���ʧ��ʱ�ķ�����Ϣ
}st_login_result, *pst_login_result;
//mes20�ӿڻظ���Ϣ
typedef struct st_20_result {
	std::string				LotId;//��������ID
	std::string				MOId;//���������Ӧ�Ĺ���Id
	std::string				MOCode;//���������Ӧ�Ĺ������
	std::string				ReturnValue;//ִ�н��  0����ʾִ��ʧ��   1����ʾִ�гɹ�
	std::string				ReturnMessage;//ִ�гɹ���ʧ��ʱ�ķ�����Ϣ
}st_20_result, *pst_20_result;
//mes40�ӿڻظ���Ϣ
typedef struct st_40_result {
	std::string				WholeDeviceCode;//WholeDeviceCode
	std::string				IMEI;//��IMEI
	std::string				SubIMEI;//��IMEI
	std::string				Mac;//Mac
	std::string				MacStart;//MacStart
	std::string				MacEnd;//MacEnd
	std::string				DeviceSerialNumber;//DeviceSerialNumber
	std::string				LanIP;//LanIP
	std::string				DeviceSerialNumberPrint;//DeviceSerialNumberPrint
	std::string				CISN;//CISN
	std::string				OUI;//OUI
	std::string				GponSN;//GponSN
	std::string				DSN;//DSN
	std::string				UserName;//UserName
	std::string				UserPass;//UserPass
	std::string				WirelessNetName;//WirelessNetName
	std::string				WlanPass;//WlanPass
	std::string				WirelessNetName2;//WirelessNetName2
	std::string				WlanPass2;//WlanPass2
	std::string				WirelessNetName3;//WirelessNetName3
	std::string				WlanPass3;//WlanPass3
	std::string				WirelessNetName4;//WirelessNetName4
	std::string				WlanPass4;//WlanPass4
	std::string				TR069UserName;//TR069UserName
	std::string				PPPOEPassWord;//PPPOEPassWord
	std::string				OrderNoAllocation;//OrderNoAllocation
	std::string				STBNO;//STBNO
	std::string				MFRS;//MFRS
	std::string				SN;//SN
	std::string				CMEI;//CMEI
	std::string				QRCode;//QRCode
	std::string				DevKey;//DevKey
	std::string				AreaCode;//������
	std::string				ReturnValue;//ִ�н��  0����ʾִ��ʧ��   1����ʾִ�гɹ�
	std::string				ReturnMessage;//ִ�гɹ���ʧ��ʱ�ķ�����Ϣ

}st_40_result, *pst_40_result;
struct amessage {
	unsigned int command;      /* command identifier constant   */
	unsigned char testname[16];  /* usbtest/tftest/mictest....    */
	unsigned char version;       /* package version          */
	unsigned int type;         /* Get/Set/AutoTest/ManualTest   */
	unsigned char result;        /* Test result                   */
	unsigned int timeout;      /* timeout of test */
	unsigned int id;            /* not used now               */
	unsigned int data_length;    /* length of payload        */
	unsigned int data_check;    /* checksum of data payload  */
	unsigned int reserved;      /* reserved not used */
	unsigned int msg_check;    /* checksum of message      */
};

struct info_s {
	unsigned char name[12];
	unsigned char value[48];
};

struct deviceinfo {
	unsigned int num;  /* current info number   */
	struct info_s info[4];
};

typedef struct apacket
{
	amessage msg;
	deviceinfo data;
}communicate_apacket_info;
#pragma pack(pop)
#endif