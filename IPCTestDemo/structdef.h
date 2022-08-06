#ifndef STRUCT_2009
#define STRUCT_2009
#include <string>
#include <cstdint>
#include <memory>
#include <vector>

#pragma pack(push, 1) 

//工作目录
const int WorkDirCount = 8;
const std::string strWorkDir[] = { "\\WorkDir", "\\IQ", "\\Audio", "\\Estimate", "\\Capture", "\\Bit" ,"\\Ddc", "\\Para" };

//其中 command 值如下：
#define		A_TEST			0xabcdef01
#define		A_OKAY			0xabcdef02
#define		A_START			0xabcdef03
#define		A_RESULT		0xabcdef04
#define		MAX_PAYLOAD		244

#define		MAX_TURN_COUNT			 1
//项目名称
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
//测试任务
enum eCtrlCmd
{
	CMD_DEVIINFO = 0,		//获取设备信息
	CMD_URL,				//获取直播播放地址
	CMD_KEYREAD,			//读运营商信息
	CMD_EXITTEST,			//退出厂测模式
	CMD_SETTIME,			//设置时间
	CMD_KEYWRITE,			//写运营商信息
	CMD_SETDEFAULT,			//恢复出厂设置
	CMD_WHITELIGHT,			//开关白光灯
	CMD_IRLIGHT,			//开关红外灯
	CMD_IRCUT,				//开关IRCUT
	CMD_NETLED,				//开关网络LED
	CMD_SPEAKERTEST,		//测试扬声器
	CMD_MICTEST,			//测试麦克风
	CMD_IRNVTEST,			//测试红外夜视
	CMD_FULLCOLORNV,		//测试全彩夜视
	CMD_KEYTEST,			//测试Rest按键
	CMD_TFTEST,				//测试TF卡
	CMD_LDRTEST,			//测试光敏
	CMD_COUNT
};

const std::string strTcpCmd[] = {
	"获取设备信息","获取直播播放地址","读运营商信息","退出厂测模式","设置时间","写运营商信息",
	"恢复出厂设置","开关白光灯","开关红外灯","开关IRCUT","开关网络LED","测试扬声器",
	"测试麦克风","测试红外夜视","测试全彩夜视","测试Rest按键","测试TF卡","测试光敏"
};
//连接服务器状态
enum eSerConnState
{
	E_CONN_OK = 0,		//连接成功
	E_CONN_DIS			//断开连接
};

//登录状态
enum eLoginState
{
	E_Login_OK = 0,     //成功
	E_Login_Name_Err,	//用户名不存在
	E_Login_Psw_Err,	//密码错误
	E_Login_Power_Err,	//权限不够
	E_Login_Two_Err		//账户已登录
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
	"登录成功", "用户名不存在", "密码错误", "权限不够", "账户已登录"
};

//通用参数
typedef struct st_common_info {
	std::string     strWorkPath;                //工作目录
	std::string     strremotepath;              //远端工作目录
	uint8_t			uProjectType;               //项目类型
	std::string     strUsername;                //用户名
	std::string     strPsw;                     //密码
	uint8_t         iAccess;                    //权限
	std::string		iResCode;					//资源代码
	std::string		strLocalIP;					//本地IP地址 0:tcp 1:udp
	uint32_t		uLocalPort;					//本地端口 0:tcp 1:udp
	uint32_t		uRemotePort;				//服务端端口 0:tcp 1:udp
	std::string		strRemoteIP;				//服务端IP地址 0:tcp 1:udp
	uint8_t         iAutoLogin;                 //自动登录
	std::string		strconnect;					//连接状态
	uint8_t         itcpConn;                   //TCP连接成功 1：成功 0：失败
	uint8_t         iudpConn;                   //UDP连接成功 1：成功 0：失败
	uint32_t		iLostPack;					//丢包数量
	std::string		iWebHostPort;				//web信息
	std::string		iUrl;						//直播播放地址
	st_common_info() :itcpConn(0), iudpConn(0){};
}st_common_info, *pst_common_info;

//通讯
typedef struct st_comm_info {
	std::string		strIP;				//IP地址
	uint32_t		uPort;				//转发端口
	bool            bstart;             //转发控制
}st_comm_info, *pst_comm_info;

//登录信息
typedef struct st_login_info {
	std::string				dbIP;
	std::string				dbuser;
	std::string				dbpasswd;
	std::string				dbname;
	uint16_t				dbport;
	st_login_info() :dbIP("192.168.1.49"), dbuser("root"), dbpasswd("Aa123456"), dbname("db_hys"), dbport(3306) {}
}st_login_info, *pst_login_info;
//登录mes回复信息
typedef struct st_login_result {
	std::string				UserId;//用户ID
	std::string				UserCode;//用户代码
	std::string				UserName;//用户名称
	std::string				ResId;//资源ID
	std::string				ResCode;//资源代码
	std::string				ResName;//资源名称
	std::string				LineId;//线别ID
	std::string				LineCode;//线别代码
	std::string				LineName;//线别名称
	std::string				ShiftTypeId;//班制ID
	std::string				ShiftTypeCode;//班制代码
	std::string				ShiftTypeName;//班制名称
	std::string				OPId;//工序Id
	std::string				OPCode;//工序代码
	std::string				OPName;//工序名称
	std::string				ReturnValue;//执行结果  0：表示执行失败   1：表示执行成功
	std::string				ReturnMessage;//执行成功或失败时的返回信息
}st_login_result, *pst_login_result;
//mes20接口回复信息
typedef struct st_20_result {
	std::string				LotId;//批次条码ID
	std::string				MOId;//批次条码对应的工单Id
	std::string				MOCode;//批次条码对应的工单编号
	std::string				ReturnValue;//执行结果  0：表示执行失败   1：表示执行成功
	std::string				ReturnMessage;//执行成功或失败时的返回信息
}st_20_result, *pst_20_result;
//mes40接口回复信息
typedef struct st_40_result {
	std::string				WholeDeviceCode;//WholeDeviceCode
	std::string				IMEI;//主IMEI
	std::string				SubIMEI;//副IMEI
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
	std::string				AreaCode;//地区码
	std::string				ReturnValue;//执行结果  0：表示执行失败   1：表示执行成功
	std::string				ReturnMessage;//执行成功或失败时的返回信息

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