#include "IPCTestDemo.h"
#include "GlobleVar.h"
#include <QKeyEvent> //用于键盘事件
#include "RTSP_Player.h"
#include "MyHelper.h"
#include <QPainter>
#include <QMetaType>
#include <algorithm>
#include <QFormLayout>

//#define PCBA 
IPCTestDemo::IPCTestDemo(int index,QWidget *parent)
	: QWidget(parent),
	ui(new Ui::IPCTestDemo)
{
	_index = index;
	ui->setupUi(this);
	
	initParam();
	initPage();
	bindSinalSlot();
}

IPCTestDemo::~IPCTestDemo()
{

}

void IPCTestDemo::bindSinalSlot()
{
	bool bl = connect(&_ParCmd, SIGNAL(manTimeout(QString,int)), _Business, SLOT(slots_BusiManTest(QString,int)));
	bl = connect(&_ParCmd, SIGNAL(autoTimeout(QString, int)), _Business, SLOT(slots_showAutoTest(QString, int)));
	bl = connect(_Mesjob.get(), SIGNAL(mes40Res(bool, QString, QVariant)), this, SLOT(upDataToPage_40(bool, QString, QVariant)));
	bl = connect(_Mesjob.get(), SIGNAL(mes20Res(bool, QString, QVariant)), this, SLOT(upDataToPage_20(bool, QString, QVariant)));
	bl = connect(_Mesjob.get(), SIGNAL(mes30Res(bool, QString)), this, SLOT(upDataToPage_30(bool, QString)));
	bl = connect(&_ParCmd, SIGNAL(message(QVariant, QString)), this, SLOT(upDataFromIPC(QVariant, QString)));
	bl = connect(&_ParCmd, SIGNAL(tcpConn(bool, QString)), this, SLOT(slots_isConnectIPC(bool, QString)));
	bl = connect(&_ParCmd, SIGNAL(manResult(QString, int)), this, SLOT(slots_ManTest(QString, int)));
	bl = connect(&_ParCmd, SIGNAL(autoResult(QString, int)), this, SLOT(slots_AutoTest(QString, int)));
	bl = connect(&_ParCmd, SIGNAL(myResult(QString, int)), this, SLOT(slots_upReturn(QString, int)));
	bl = connect(ui->compar_Button, SIGNAL(clicked()), this, SLOT(SlotComparInfo())); 
	bl = connect(ui->write_Button, SIGNAL(clicked()), this, SLOT(SlotWriteInfo())); 
	bl = connect(ui->default_Button, SIGNAL(clicked()), this, SLOT(SlotDefault()));
	bl = connect(this, SIGNAL(writeInfo(QVariant)), &_ParCmd, SLOT(updateWriteInfo(QVariant)));
	bl = connect(&_ParCmd, SIGNAL(showMovingWidget()), _Business, SLOT(slot_showMovingWidget()));
	bl = connect(&_Business->_MyMovingWidget, SIGNAL(ptz_signal(QString, int)), this, SLOT(slots_ManTest(QString, int)));
	bl = connect(&_Business->_MyWiFiWidget, SIGNAL(wifi_signal(QString, int)), this, SLOT(slots_ManTest(QString, int)));
	bl = connect(&_ParCmd, SIGNAL(WiFiSiteInfo(QVariant)), _Business, SLOT(slot_showWifiWidget(QVariant)));
	bl = connect(&_ParCmd, SIGNAL(RsWiFiInfo(QVariant)), &_Business->_MyWiFiWidget, SLOT(slot_upWiFiInfo(QVariant)));
}

void IPCTestDemo::initPage()
{
	//std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	//设置主界面标题栏自绘
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	setWindowState(Qt::WindowMaximized);
	this->setFixedWidth(458);
	this->showMaximized();
	int m = ui->widget_4->width();
	ui->rtsp_label->setMinimumSize(m, m / 9 * 6);
	ui->label_2->setText(QString::number(_index + 1));
	ui->groupBox_3->setMaximumWidth(ui->groupBox_2->width());
	ui->groupBox_3->showMaximized();
	QVBoxLayout* layor = (QVBoxLayout*)ui->widget_4->layout();
	layor->setStretchFactor(ui->groupBox_2, 5);
	layor->setStretchFactor(ui->groupBox_3, 5);
	ui->horizontalLayout_3->setStretchFactor(ui->lineEdit_IP,7);
	ui->horizontalLayout_3->setStretchFactor(ui->pushButton_2,3);

	ui->lineEdit_IP->hide();
	ui->lineEdit_connect->setEnabled(false);
#ifdef MES
	ui->lineEdit_MOCode->setEnabled(false);
	ui->lineEdit_MAC->setEnabled(false);
	ui->lineEdit_SN->setEnabled(false);
	ui->lineEdit_CPassword->setEnabled(false);
	ui->lineEdit_DevKey->setEnabled(false);
	ui->lineEdit_UserPass->setEnabled(false);
#endif // !MES	
	ui->lineEdit_DeviceName->setEnabled(false);
	ui->lineEdit_AppVersion->setEnabled(false);
	ui->lineEdit_DeviceModel->setEnabled(false);

	//ui.lineEdit_SN->setFocusPolicy(Qt::StrongFocus);
	//qApp->installEventFilter(this); //为所有控件添加事件过滤器

	ui->rtsp_label->setStyleSheet("QLabel{border:2px solid green}");
	ui->rtsp_label->installEventFilter(this);
	edit_CMEI = new ButtonEdit(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
	QObject::connect(edit_CMEI, &ButtonEdit::buttonClicked, [&](bool) {edit_CMEI->setText(""); });
	auto layout = new QFormLayout;
	layout->addRow(edit_CMEI);
	layout->setMargin(0);
	ui->CMEI_widget->setLayout(layout);

	edit_PCBASN = new ButtonEdit(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
	QObject::connect(edit_PCBASN, &ButtonEdit::buttonClicked, [&](bool) {edit_PCBASN->setText(""); });
	auto layout_1 = new QFormLayout;
	layout_1->addRow(edit_PCBASN);
	layout_1->setMargin(0);
	ui->PCBASN_widget->setLayout(layout_1);
	
	//ui->lineEdit_firmwareVer->setText(QString::fromStdString(ptr->m_stCommonInfo.firmwareVersion));
#ifdef MES
	ui->write_Button->setEnabled(false);
#endif
	ui->compar_Button->setEnabled(false);
	ui->rtsp_label->setToolTip(QString::number(_index));

#ifdef PCBA
	ui->CMEI_widget->hide();
	ui->label_CMEI->hide();
#else
	ui->PCBASN_widget->hide();
	ui->label_PCBASN->hide();
#endif
}

void IPCTestDemo::initParam()
{
	blRtsp = false;
	_Mesjob = std::shared_ptr<Mesjob>(new Mesjob());
	IsConnectIPC = false;

	m_ffmpeg = new MyFFmpeg;
	_Business = new BusinessLogic(ui->groupBox_2);
	bool bl = connect(m_ffmpeg, SIGNAL(MyFFmpegSigGetOneFrame(QImage)), this, SLOT(SlotGetOneFrame(QImage)));
}

void IPCTestDemo::initStyle()
{
	QFile styleFile(":/IPCTestDemo/Resources/IPCTestDemo.qss");
	if (!styleFile.open(QIODevice::ReadOnly))
	{
		//CPCLOG_WARNING << "open style file error, style file path:" + styleFile.fileName().toStdString();
		return;
	}
	this->setStyleSheet(styleFile.readAll());
	//qApp->setStyleSheet(styleFile.readAll());
	styleFile.close();
}

void IPCTestDemo::initTestItems(QVector<QString> vec)
{
	QVBoxLayout* layout = new QVBoxLayout;
	QGridLayout *pLayout = new QGridLayout();
	_testvec = vec;
	int i = 0, j = 0;
	for (auto a : vec)
	{
		QLabel* label = new QLabel;
		//label->setFixedSize();
		label->setAlignment(Qt::AlignCenter);
		label->setText(a);
		pLayout->addWidget(label, i, j++);
		if (j > 4) {
			j = 0;
			i++;
		}
		_map.insert(toTestName(a), label);
	}
	QHBoxLayout* hlayout = new QHBoxLayout();
	QLabel* info_label = new QLabel(QString::fromLocal8Bit("测试结果"),this);
	info_label->setObjectName("info_label");
	hlayout->addWidget(info_label);
	hlayout->addStretch();
	startTestBtn = new QPushButton(QString::fromLocal8Bit("开始测试"));
	startTestBtn->setObjectName("m_testButton");
	hlayout->addWidget(startTestBtn);
	layout->addLayout(hlayout);
	layout->addLayout(pLayout);
	QVBoxLayout* layout_2 = new QVBoxLayout;
	show_info_label = new MLabel(this);
	show_info_label->setObjectName("show_info_label");
	layout_2->addWidget(show_info_label);
	layout->addLayout(layout_2);
	ui->groupBox_3->setLayout(layout);
	bool bl = connect(startTestBtn, SIGNAL(clicked()), this, SLOT(slots_testButton()));
	startTestBtn->setEnabled(false);
	ui->default_Button->setVisible(false);
	ui->compar_Button->setVisible(false);
	//initStyle();
}

void IPCTestDemo::PlayStart()
{
	//this->PlayStop();

	m_playThread = new QThread();
	m_player = new RTSP_Player(m_ffmpeg);
 	m_player->SetPlayerUrl(m_playUrl);
// 
	bool bl = connect(this, SIGNAL(SigPlayStart()), m_player, SLOT(PlayerStart()));
	bl = connect(this, SIGNAL(SigPlayStop()), m_player, SLOT(PlayerStop()));
	bl = connect(m_player, SIGNAL(SigOpenUrlResult(int)), this, SLOT(SlotOpenUrlResult(int)));
	bl = connect(m_player, SIGNAL(Sig_FramTimeOut()), this, SLOT(slot_fram_stop()));
	m_player->moveToThread(m_playThread);
	m_playThread->start();
	emit SigPlayStart();
}

void IPCTestDemo::deleteLater()
{
	m_playThread->quit();		// 也可以使用thread->exit(0);
	m_playThread->wait(1500);	// wait函数是个阻塞的接口，意思是线程必须真的退出了，才会执行wait之后的语句，否则将会一直阻塞在这里，如果在界面上使用，需要保证线程中代码的合理性。
	delete m_playThread;
	m_playThread = nullptr;
}

void IPCTestDemo::PlayStop()
{
	if (m_player)
	{
		//emit SigPlayStop();
		m_player->PlayerStop();
	}

	if (m_playThread)
	{
		m_playThread->quit();
		m_playThread->wait(1500);
		delete m_playThread;
		m_playThread = nullptr;
	}

	if (m_player)
	{
		delete m_player;
		m_player = nullptr;
	}
}

void IPCTestDemo::getDeviceInfo()
{
	_ParCmd.ParsCmd(CMD_DEVIINFO, A_TEST);
}

void IPCTestDemo::comparInfo(QMap<QString, QString> map)
{
	QString mac = map.value("mac");
	mac = mac.remove(":");
	map.insert("mac", mac);
	bool bl = true;
	if (map.value("cmei") == edit_CMEI->text()) {
		edit_CMEI->setStyleSheet("color:#20FCFF;");
	}
	else{
		bl = false;
		edit_CMEI->setStyleSheet("color:#FF0000;");
	}

	if(map.value("mac") == ui->lineEdit_MAC->text()) {
		ui->lineEdit_MAC->setStyleSheet("color:#20FCFF;");
	}
	else{
		bl = false;
		ui->lineEdit_MAC->setStyleSheet("color:#FF0000;");
	}

	if (map.value("sn") == ui->lineEdit_SN->text()) {
		ui->lineEdit_SN->setStyleSheet("color:#20FCFF;");
	}
	else{
		bl = false;
		ui->lineEdit_SN->setStyleSheet("color:#FF0000;");
	}

	if (map.value("ciphertextpassword") == ui->lineEdit_CPassword->text()) {
		ui->lineEdit_CPassword->setStyleSheet("color:#20FCFF;");
	}
	else{
		bl = false;
		ui->lineEdit_CPassword->setStyleSheet("color:#FF0000;");
	}

	if (map.value("devkey") == ui->lineEdit_DevKey->text()) {
		ui->lineEdit_DevKey->setStyleSheet("color:#20FCFF");
	}
	else{
		bl = false;
		ui->lineEdit_DevKey->setStyleSheet("color:#FF0000;");
	}

	if (map.value("userpass") == ui->lineEdit_UserPass->text()) {
		ui->lineEdit_UserPass->setStyleSheet("color:#20FCFF;");
	}
	else{
		bl = false;
		ui->lineEdit_UserPass->setStyleSheet("color:#FF0000;");
	}
	if(bl){
		showInfo(true, QString::fromLocal8Bit("比对成功"));
	}
	else{
		showInfo(true, QString::fromLocal8Bit("比对失败"));
	}
}

QString IPCTestDemo::errCode(int dex)
{
	QString str;
	switch (dex)
	{
	case CMD_DEVIINFO:						str = "IPC015";					break;
	case CMD_URL:							str = "IPC016";					break;
	case CMD_KEYREAD:						str = "IPC013";					break;
	case CMD_EXITTEST:						str = "IPC014";					break;
	case CMD_SETTIME:						str = "IPC037";					break;
	case CMD_KEYWRITE:						str = "IPC012";					break;
	case CMD_SETDEFAULT:					str = "IPC017";					break;
	case CMD_WHITELIGHT:					str = "IPC001";					break;
	case CMD_IRLIGHT:						str = "IPC002";					break;
	case CMD_IRCUT:							str = "IPC003";					break;
	case CMD_NETLED:						str = "IPC004";					break;
	case CMD_SPEAKERTEST:					str = "IPC005";					break;
	case CMD_MICTEST:						str = "IPC006";					break;
	case CMD_IRNVTEST:						str = "IPC007";					break;
	case CMD_FULLCOLORNV:					str = "IPC008";					break;
	case CMD_KEYTEST:						str = "IPC009";					break;
	case CMD_VOIPKEYTEST:					str = "IPC027";					break;
	case CMD_TFTEST:						str = "IPC010";					break;
	case CMD_LDRTEST:						str = "IPC011";					break;
	case CMD_PTZTEST:						str = "IPC026";					break;
	case CMD_WIFISCANRESULT:				str = "IPC025";					break;
	case CMD_VIDEO:							str = "IPC031";					break;
	case CMD_RESET:							str = "IPC038";					break;
	case CMD_FLASHVERIFY:					str = "IPC039";					break;
	case CMD_MARQUEE:						str = "IPC040";					break;
	case CMD_INFORMATION_COMPARISON:		str = "IPC036";					break;
	default:
		break;
	}
	return str;
}

QString IPCTestDemo::toTestName(QString str)
{
	if (str == QString::fromLocal8Bit("获取设备信息"))							{ return "devinfo"; }
	else if (str == QString::fromLocal8Bit("获取直播播放地址"))					{ return "url"; }
	else if (str == QString::fromLocal8Bit("读运营商信息"))						{ return "keyread"; }
	else if (str == QString::fromLocal8Bit("退出厂测模式"))						{ return "exittest"; }
	else if (str == QString::fromLocal8Bit("设置时间"))							{ return "timewrite"; }
	else if (str == QString::fromLocal8Bit("写运营商信息"))						{ return "keywrite"; }
	else if (str == QString::fromLocal8Bit("恢复出厂设置"))						{ return "setdefault"; }
	else if (str == QString::fromLocal8Bit("开关白光灯"))						{ return "whitelight"; }
	else if (str == QString::fromLocal8Bit("开关红外灯"))						{ return "irlight"; }
	else if (str == QString::fromLocal8Bit("开关IRCUT"))						{ return "ircut"; }
	else if (str == QString::fromLocal8Bit("开关网络LED灯"))					{ return "netled"; }
	else if (str == QString::fromLocal8Bit("测试扬声器"))						{ return "speakertest"; }
	else if (str == QString::fromLocal8Bit("测试麦克风"))						{ return "mictest"; }
	else if (str == QString::fromLocal8Bit("测试红外夜视"))						{ return "irnvtest"; }
	else if (str == QString::fromLocal8Bit("测试全彩夜视"))						{ return "fullcolornv"; }
	else if (str == QString::fromLocal8Bit("测试Rest按键"))						{ return "keytest"; }
	else if (str == QString::fromLocal8Bit("测试通话按键"))						{ return "voipkeytest"; }
	else if (str == QString::fromLocal8Bit("测试TF卡"))							{ return "tftest"; }
	else if (str == QString::fromLocal8Bit("测试光敏"))							{ return "ldrtest"; }
	else if (str == QString::fromLocal8Bit("PTZ马达测试"))						{ return "ptztest"; }
	else if (str == QString::fromLocal8Bit("WiFi测试"))							{ return "wifiscanresults"; }
	else if (str == QString::fromLocal8Bit("影像清晰度及脏点确认"))				{ return "video"; }
	else if (str == QString::fromLocal8Bit("批量升级模式退出"))					{ return "reset"; }
	else if (str == QString::fromLocal8Bit("flash数据校验"))					{ return "flash_verify"; }
	else if (str == QString::fromLocal8Bit("测试跑马灯"))						{ return "marquee"; }
	else
	{
		return "";
	}
}

QString IPCTestDemo::toTestChnName(int dex)
{
	QString str;
	switch (dex)
	{
	case CMD_DEVIINFO:						str = QString::fromLocal8Bit("获取设备信息");					break;
	case CMD_URL:							str = QString::fromLocal8Bit("获取直播播放地址");				break;
	case CMD_KEYREAD:						str = QString::fromLocal8Bit("读运营商信息");					break;
	case CMD_EXITTEST:						str = QString::fromLocal8Bit("退出厂测模式");					break;
	case CMD_SETTIME:						str = QString::fromLocal8Bit("设置时间");						break;
	case CMD_KEYWRITE:						str = QString::fromLocal8Bit("写运营商信息");					break;
	case CMD_SETDEFAULT:					str = QString::fromLocal8Bit("恢复出厂设置");					break;
	case CMD_WHITELIGHT:					str = QString::fromLocal8Bit("开关白光灯");						break;
	case CMD_IRLIGHT:						str = QString::fromLocal8Bit("开关红外灯");						break;
	case CMD_IRCUT:							str = QString::fromLocal8Bit("开关IRCUT");						break;
	case CMD_NETLED:						str = QString::fromLocal8Bit("开关网络LED");					break;
	case CMD_SPEAKERTEST:					str = QString::fromLocal8Bit("测试扬声器");						break;
	case CMD_MICTEST:						str = QString::fromLocal8Bit("测试麦克风");						break;
	case CMD_IRNVTEST:						str = QString::fromLocal8Bit("测试红外夜视");					break;
	case CMD_FULLCOLORNV:					str = QString::fromLocal8Bit("测试全彩夜视");					break;
	case CMD_KEYTEST:						str = QString::fromLocal8Bit("测试Rest按键");					break;
	case CMD_VOIPKEYTEST:					str = QString::fromLocal8Bit("测试通话按键");					break;
	case CMD_TFTEST:						str = QString::fromLocal8Bit("测试TF卡");						break;
	case CMD_LDRTEST:						str = QString::fromLocal8Bit("测试光敏");						break;
	case CMD_PTZTEST:						str = QString::fromLocal8Bit("PTZ马达测试");					break;
	case CMD_WIFISCANRESULT:				str = QString::fromLocal8Bit("获取WiFi站点测试");				break;
	case CMD_VIDEO:							str = QString::fromLocal8Bit("影像清晰度及脏点确认");			break;
	case CMD_RESET:							str = QString::fromLocal8Bit("批量升级模式退出");				break;
	case CMD_FLASHVERIFY:					str = QString::fromLocal8Bit("flash数据校验");					break;
	case CMD_MARQUEE:						str = QString::fromLocal8Bit("测试跑马灯");						break;
	case CMD_INFORMATION_COMPARISON:		str = QString::fromLocal8Bit("信息比对");						break;
	default:
		break;
	}
	return str;
}

void IPCTestDemo::clearMesInfo()
{
	ui->lineEdit_MAC->setText("");
	ui->lineEdit_SN->setText("");
	ui->lineEdit_CPassword->setText("");
	ui->lineEdit_DevKey->setText("");
	ui->lineEdit_UserPass->setText("");
	ui->lineEdit_MOCode->setText("");
	edit_CMEI->setText("");
	edit_PCBASN->setText("");
	ui->lineEdit_MAC->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_SN->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_CPassword->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_DevKey->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_UserPass->setStyleSheet("color:#F0F0F0;");
	edit_CMEI->setStyleSheet("color:#F0F0F0;");
}

void IPCTestDemo::clearMesInfoNoCMEI()
{
	ui->lineEdit_MAC->setText("");
	ui->lineEdit_SN->setText("");
	ui->lineEdit_CPassword->setText("");
	ui->lineEdit_DevKey->setText("");
	ui->lineEdit_UserPass->setText("");
	ui->lineEdit_MOCode->setText("");
	ui->lineEdit_MAC->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_SN->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_CPassword->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_DevKey->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_UserPass->setStyleSheet("color:#F0F0F0;");
	edit_CMEI->setStyleSheet("color:#F0F0F0;");
}

void IPCTestDemo::clearAllInfo()
{
	ui->lineEdit_MAC->setText("");
	ui->lineEdit_SN->setText("");
	ui->lineEdit_CPassword->setText("");
	ui->lineEdit_DevKey->setText("");
	ui->lineEdit_UserPass->setText("");
	ui->lineEdit_DeviceName->setText("");
	ui->lineEdit_AppVersion->setText("");
	ui->lineEdit_DeviceModel->setText("");
	ui->lineEdit_MOCode->setText("");
	edit_CMEI->setText("");
	edit_PCBASN->setText("");
	ui->lineEdit_MAC->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_SN->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_CPassword->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_DevKey->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_UserPass->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_DeviceName->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_AppVersion->setStyleSheet("color:#F0F0F0;");
	ui->lineEdit_DeviceModel->setStyleSheet("color:#F0F0F0;");
	edit_CMEI->setStyleSheet("color:#F0F0F0;");
}

void IPCTestDemo::showInfo(bool bl, QString str)
{
	if (bl)	{
		CPCLOG_INFO << str.toStdString();
		show_info_label->setStyleSheet("color:#22D3EE;");
	}
	else {
		CPCLOG_ERROR << str.toStdString();
		show_info_label->setStyleSheet("color:#FF0000;");
	}
	show_info_label->setText(str);
}

void IPCTestDemo::NextTest()
{
	if (_testlist.count() != 0)
	{
		if (IsConnectIPC)
			_ParCmd.ParsCmd(gVar->comparTestname(_testlist.first()), A_TEST);
	}
	else if (_testlist.count() == 0)
	{
		ui->write_Button->setEnabled(true);
	}
}

void IPCTestDemo::upDataToPage_20(bool bl, QString str, QVariant var)
{
	if(bl){
		QMap<QString, QString> _maps = var.value<QMap<QString, QString>>();
		ui->lineEdit_MOCode->setText(_maps.value("MOCode"));
	}
	showInfo(bl, str);
}

void IPCTestDemo::upDataToPage_30(bool bl, QString str)
{
	if (bl)
	{
		clearAllInfo();
		if (blRtsp == true) {
			this->PlayStop();
		}
		_ParCmd.tcpDisConnect();
		ui->write_Button->setEnabled(false);
		ui->compar_Button->setEnabled(false);
	}
	showInfo(bl, str);
}

void IPCTestDemo::upDataToPage_40(bool bl, QString str, QVariant var)
{
	if (bl) {
		QMap<QString, QString> _maps = var.value<QMap<QString, QString>>();
		QVariant DataVar;
		QMap<QString, QString> _map;
		_map.insert("PCBASN", _maps.value("PCBASN"));
		_map.insert("CMEI", _maps.value("CMEI"));
		_map.insert("SN", _maps.value("SN"));
		_map.insert("Mac", _maps.value("Mac"));
		_map.insert("DevKey", _maps.value("DevKey"));
		_map.insert("UserPass", _maps.value("UserPass"));
		_map.insert("CiphertextPassword", _maps.value("CiphertextPassword"));
		_map.insert("DeviceSerialNumber", _maps.value("DeviceSerialNumber"));

		DataVar.setValue(_map);
		emit writeInfo(DataVar);
//		ui->write_Button->setEnabled(true);
	}
	else {
		showInfo(bl, str);
	}
}

void IPCTestDemo::upDataFromIPC(QVariant var, QString str)
{
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	QMap<QString, QString> _maps = var.value<QMap<QString, QString>>();
	if (str == "devinfo") {
		ui->lineEdit_DeviceName->setText(_maps.value("device_name"));
		ui->lineEdit_DeviceModel->setText(_maps.value("device_model"));
		QString ver_str = _maps.value("app_version");
		if (ver_str.compare(QString::fromStdString(ptr->m_stCommonInfo.firmwareVersion)) == 0)
		{
			ui->lineEdit_AppVersion->setStyleSheet("color:#20FCFF;");
			startTestBtn->setEnabled(true);
		}
		else
		{
			showInfo(false, QString::fromLocal8Bit("固件版本不正确"));
			ui->lineEdit_AppVersion->setStyleSheet("color:#FF0000;");
			startTestBtn->setEnabled(false);
		}
		ui->lineEdit_AppVersion->setText(ver_str);
#ifdef PCBA
		edit_PCBASN->setText(_maps.value("cmei"));
#else
		edit_CMEI->setText(_maps.value("cmei"));
#endif
		//ui->lineEdit_CMEI->setText(_maps.value("cmei"));
		//ui->lineEdit_MOCode->setText(_maps.value("device_model"));
		ui->lineEdit_MAC->setText(_maps.value("mac"));
		ui->lineEdit_SN->setText(_maps.value("sn"));
		ui->lineEdit_CPassword->setText(_maps.value("ciphertextpassword"));
		ui->lineEdit_DevKey->setText(_maps.value("devkey"));
		ui->lineEdit_UserPass->setText(_maps.value("userpass"));
	}
	else if(str == "url"){
		foreach(const QString map_str, _maps.keys())
		{
			m_playUrl = _maps.value(map_str);
		}
		switch_rtsp();
	}
	else if(str == "keyread"){
		comparInfo(_maps);
	}
}

void IPCTestDemo::SlotGetOneFrame(QImage img)
{
	m_image = img;
	emit sigOneImage(_index ,img);
	this->update();
}

void IPCTestDemo::SlotOpenUrlResult(int result)
{
	if (result == RTSP_Player::FFmpegInitFail)
	{
		this->PlayStop();

		QString rtspUrl = m_playUrl;
		QString errMsg = QString::fromLocal8Bit("打开") + rtspUrl + QString::fromLocal8Bit("失败");
		MyHelper::ShowMessageBoxError(errMsg);
		blRtsp = false;
	}
	else if (result == RTSP_Player::FFmpegInitSucc)
	{
		blRtsp = true;
	}
	else if (result == RTSP_Player::FFmpegStoped)
	{
		this->PlayStop();
		ui->pushButton_2->setText(QString::fromLocal8Bit("连接"));
		_ParCmd.tcpDisConnect();
		blRtsp = false;
	}
	else
	{
#if 0
		this->PlayStop();

		if (result == -825242872) {
			UsernameAndPasswordFrm *adminFrm = new UsernameAndPasswordFrm();
			adminFrm->show();
			if (g_username.length() == 0 || g_password.length() == 0) {
				QString rtspUrl = ui->lineEdit->text().trimmed();
				QString errMsg = "打开" + rtspUrl + "失败";
				MyHelper::ShowMessageBoxError(errMsg);
				ui.pushButton->setText("start");
			}
			else
			{
				ui.pushButton->setEnabled(false);
				QString tmp = m_playUrl.mid((int)strlen("rtsp://"), -1);
				qDebug("tmp=%s", qPrintable(tmp));
				m_playUrl = "rtsp://" + g_username + ":" + g_password + "@" + tmp;
				qDebug("m_playUrl=%s", qPrintable(m_playUrl));
				//this->PlayProcess();
				this->PlayStart();
			}
		}
#endif
	}
}

void IPCTestDemo::SlotComparInfo()
{
	_ParCmd.ParsCmd(CMD_KEYREAD, A_TEST);
}

void IPCTestDemo::SlotWriteInfo()
{
#ifndef MES
	QVariant DataVar;
	QMap<QString, QString> _map;
	_map.insert("CMEI", edit_CMEI->text());
	_map.insert("Mac", ui->lineEdit_MAC->text());
	_map.insert("SN", ui->lineEdit_SN->text());
	_map.insert("CiphertextPassword", ui->lineEdit_CPassword->text());
	_map.insert("DevKey", ui->lineEdit_DevKey->text());
	_map.insert("UserPass", ui->lineEdit_UserPass->text());

	DataVar.setValue(_map);
	emit writeInfo(DataVar);
#endif
	_ParCmd.ParsCmd(CMD_KEYWRITE, A_TEST);
}

void IPCTestDemo::SlotDefault()
{
	_ParCmd.ParsCmd(CMD_SETDEFAULT, A_TEST);
}

void IPCTestDemo::switch_rtsp()
{
	if (blRtsp == false) {
		if (m_playUrl.length() <= (int)strlen("rtsp://")) {
			MyHelper::ShowMessageBoxError(QString::fromLocal8Bit("请输入rtsp播放地址!"));
			return;
		}

		_ParCmd.ParsCmd(CMD_SETTIME, A_TEST);
		//this->PlayProcess();
		this->PlayStart();
	}
	else if (blRtsp == true) {
		this->PlayStop();
	}
}

void IPCTestDemo::on_pushButton_2_clicked()
{
	if (ui->pushButton_2->text().compare(QString::fromLocal8Bit("连接")) == 0) {
		_ParCmd.tcpConnect(_index);
		clearAllInfo();
		ui->pushButton_2->setText(QString::fromLocal8Bit("正在连接..."));
		ui->pushButton_2->setEnabled(false);
		ui->write_Button->setEnabled(false);
		ui->compar_Button->setEnabled(false);
		showInfo(true,"");
	}
	else if (ui->pushButton_2->text().compare(QString::fromLocal8Bit("断开连接")) == 0) {
		_ParCmd.tcpDisConnect();
		clearAllInfo();
		startTestBtn->setEnabled(false);
		ui->pushButton_2->setEnabled(false);
		ui->write_Button->setEnabled(false);
		ui->compar_Button->setEnabled(false);
		blRtsp = false;
		this->PlayStop();
	}
}

void IPCTestDemo::slots_testButton()
{
	_testlist.clear();
	_mapResult.clear();
	showInfo(true, "");
	QMap<QString, QLabel*>::iterator iter = _map.begin();
	while (iter != _map.end())
	{
		iter.value()->setStyleSheet("background-color:rgba(102,102,102,0.25);"); // 迭代器
		iter++;
	}
	for (auto a: _testvec)
	{
		_testlist.push_back(toTestName(a));
	}
	if (_testlist.count() != 0)
	{
		if (IsConnectIPC)
			_ParCmd.ParsCmd(gVar->comparTestname(_testlist.first()), A_TEST);
	}
}

void IPCTestDemo::upToMES30()
{
	int bl = 1;
	QString str;
	QString errstr;
	QMap<int, QString>::Iterator iter;
	for (iter = _mapResult.begin(); iter != _mapResult.end(); ++iter)
	{
		if(iter.value() == "2" || iter.value() == "3"){
			bl = 0;
			errstr += errCode(iter.key()) + ";";
		}
		str += toTestChnName(iter.key()) + ":" + iter.value() + ";";
	}
#ifdef MES
	_Mesjob->common_send30info(bl, str, errstr);
#endif // MES	
}

void IPCTestDemo::writeInfoReturn(int index, int value)
{
	_mapResult.insert(index, QString::number(value));
	if (index == CMD_KEYWRITE) {
		if (value == 1) {
			showInfo(true, QString::fromLocal8Bit("写入信息成功!"));
			clearAllInfo();
			getDeviceInfo();
//			_ParCmd.ParsCmd(CMD_RESET, A_TEST);
		}
		else {
			showInfo(false, QString::fromLocal8Bit("写入信息失败!"));
			CPCLOG_ERROR << "写入信息失败!";
		}
		upToMES30();
	}
}

void IPCTestDemo::slots_isConnectIPC(bool bl,QString str)
{
	if (bl) {
		_ParCmd.ParsCmd(CMD_URL, A_TEST);
		ui->pushButton_2->setText(QString::fromLocal8Bit("断开连接"));
		ui->lineEdit_connect->setText(str);
		Sleep(100);
		getDeviceInfo();
		_Business->setPCMD(&_ParCmd);
#ifndef MES
		ui->write_Button->setEnabled(true);
#endif // !MES

	}
	else
	{
		ui->pushButton_2->setText(QString::fromLocal8Bit("连接"));
		ui->lineEdit_connect->setText(str);
		blRtsp = false;
		this->PlayStop();
	}
	IsConnectIPC = bl;
	ui->pushButton_2->setEnabled(true);
}

void IPCTestDemo::slots_ManTest(QString testname, int revalue)
{
	QString backColor;
	switch (revalue)
	{
	case 0:backColor = "background-color:#64A600;color:#000000";	break;
	case 1:backColor = "background-color:#22D3EE;color:#000000";	break;
	case 2:backColor = "background-color:#850000;color:#FFFFFF";	break;
	case 3:backColor = "background-color:blue;color:#FFFFFF";		break;
	case 4:backColor = "background-color:yellow;color:#000000";		break;
	default:
		break;
	}
	if(testname=="ptzatlogo"&&revalue == Result_False){
		showInfo(false, QString::fromLocal8Bit("马达恢复初始位置失败."));
		testname = "ptztest";
		_mapResult.insert(gVar->comparTestname(testname), QString::number(revalue));
		_map.value(testname)->setStyleSheet(backColor);
		upToMES30();
		return;
	}
	_map.value(testname)->setStyleSheet(backColor);
	_mapResult.insert(gVar->comparTestname(testname), QString::number(revalue));
	if (revalue == 0x01) {
		_testlist.removeOne(testname);
		NextTest();
	}
	else
	{
		upToMES30();
	}
}

void IPCTestDemo::slots_AutoTest(QString testname, int revalue)
{
	QString backColor;
	switch (revalue)
	{
	case 0:backColor = "background-color:#64A600;color:#000000";	break;
	case 1:backColor = "background-color:#22D3EE;color:#000000";	break;
	case 2:backColor = "background-color:#850000;color:#FFFFFF";	break;
	case 3:backColor = "background-color:blue;color:#FFFFFF;";		break;
	case 4:backColor = "background-color:yellow;color:#000000;";	break;
	default:
		break;
	}
	_map.value(testname)->setStyleSheet(backColor);
	if (revalue == 0x01|| revalue == 0x02|| revalue == 0x03) 
	{
		_Business->reasultAutoTest();
		_mapResult.insert(gVar->comparTestname(testname), QString::number(revalue));
	}
	if (revalue == 0x01) {
		_testlist.removeOne(testname);
		NextTest();
	}
	else if(revalue == 0x02 || revalue == 0x03)
	{
		upToMES30();
	}
}

void IPCTestDemo::slots_upReturn(QString testname, int revalue)
{
	_mapResult.insert(gVar->comparTestname(testname), QString::number(revalue));
	if (testname.contains("keywrite")) {
		if (revalue == 1) {
			writeInfoReturn(CMD_KEYWRITE, revalue);
		}
		else {
			QMessageBox msgBox;
			QString ss = toTestChnName(gVar->comparTestname(testname)) + QString::fromLocal8Bit("失败");
			msgBox.setText(ss);
			msgBox.exec();
			writeInfoReturn(CMD_KEYWRITE, revalue);
		}
	}
	else if (testname.contains("reset")) {
		if (revalue == 1) {
			showInfo(true, QString::fromLocal8Bit("批量升级模式退出成功."));
		}
		else {
			showInfo(false, QString::fromLocal8Bit("批量升级模式退出失败."));
		}
		upToMES30();
	}
	else if (revalue == Result_False) {
		QString testName = toTestChnName(gVar->comparTestname(testname));
		QString showMessage = testName + QString::fromLocal8Bit("失败。");
		showInfo(false, showMessage);
	}
}

void IPCTestDemo::slot_fram_stop()
{
	if (ui->pushButton_2->text().compare(QString::fromLocal8Bit("断开连接")) == 0) {
		clearAllInfo();
		ui->pushButton_2->setEnabled(false);
		ui->write_Button->setEnabled(false);
		ui->compar_Button->setEnabled(false);
		_ParCmd.tcpDisConnect();
		this->PlayStop();
	}
}

void IPCTestDemo::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Return) //扫码枪内字符串，以回车结尾
	{
		//clearMesInfoNoCMEI();
		QString str;
#ifdef PCBA
		str = edit_PCBASN->text();
#else
		str = edit_CMEI->text();
#endif	//PCBA

#ifdef MES
		_Mesjob->common_get20info(str);
#endif // MES
	}
}

void IPCTestDemo::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);

	if (m_image.size().width() <= 0)
		return;
	QImage img = m_image.scaled(ui->rtsp_label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

 	//QPoint p5 = ui->rtsp_label->mapToGlobal(QPoint(0, 0));   //按钮相对于桌面原点的绝对位置
	QPoint p5 = ui->rtsp_label->mapToParent(QPoint(0, 0));   //按钮相对于父类窗口的绝对位置
	//ui.rtsp_label->setPixmap(QPixmap::fromImage(img));
	painter.drawImage(p5, img);
}

void IPCTestDemo::closeEvent(QCloseEvent *e)
{
	this->PlayStop();
}

void IPCTestDemo::mouseDoubleClickEvent(QMouseEvent* e)
{
	//是否时左键
	if (e->button() == Qt::LeftButton)
	{
		//获取鼠标坐标
		int mouse_x = QCursor::pos().x();
		int mouse_y = QCursor::pos().y();
		//获取鼠标点击到的控件
		QWidget* subWidget = QApplication::widgetAt(mouse_x, mouse_y);
		QString helpstr = subWidget->toolTip();
		if (!helpstr.isEmpty())
		{
			QLabel* label = dynamic_cast<QLabel*>(subWidget);//只有QLabel设置了toolTip
			int index = helpstr.toInt();
			emit sigChanggeIndex(index);
		}
	}
}