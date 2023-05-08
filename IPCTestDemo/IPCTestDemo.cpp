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
// 	if (m_playThread)
// 	{
// 		m_playThread->quit();
// 		m_playThread->wait(1500);
// 		delete m_playThread;
// 		m_playThread = nullptr;
// 	}
// 
// 	if (m_player)
// 	{
// 		delete m_player;
// 		m_player = nullptr;
// 	}
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
	//bl = connect(&_ParCmd, SIGNAL(autoResult(QString, int)), _Business, SLOT(slots_reasultAutoTest(QString, int)));
	bl = connect(&_ParCmd, SIGNAL(myResult(QString, int)), this, SLOT(slots_upReturn(QString, int)));
	bl = connect(this, SIGNAL(sigNextTest()), this, SLOT(slots_NextTest()));
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
	ui->lineEdit_MAC->setEnabled(false);
	ui->lineEdit_MOCode->setEnabled(false);
	ui->lineEdit_SN->setEnabled(false);
	ui->lineEdit_CPassword->setEnabled(false);
	ui->lineEdit_DevKey->setEnabled(false);
	ui->lineEdit_UserPass->setEnabled(false);
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
	ui->write_Button->setEnabled(false);
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
// 
// 	m_playThread = new QThread();
// 	m_player = new RTSP_Player(m_ffmpeg);
// //	m_player->SetPlayerUrl(m_playUrl);
// 
// 	bl = connect(this, SIGNAL(SigPlayStart()), m_player, SLOT(PlayerStart()));
// 	bl = connect(this, SIGNAL(SigPlayStop()), m_player, SLOT(PlayerStop()));
// 	bl = connect(m_player, SIGNAL(SigOpenUrlResult(int)), this, SLOT(SlotOpenUrlResult(int)));
// 	m_player->moveToThread(m_playThread);
// 	// 	connect(m_playThread, &QThread::finished, this, &IPCTestDemo::deleteLater);	// 退出后释放TestObject对象资源
// 	// 	connect(m_playThread, &QThread::finished, m_playThread, &QThread::deleteLater);	// 退出后释放QThread对象资源
// 	m_playThread->start();
// 	bl = connect(&_ParCmd, SIGNAL(updateConn(bool)), m_player, SLOT(upConnect(bool)));
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
	m_player->moveToThread(m_playThread);
// 	connect(m_playThread, &QThread::finished, this, &IPCTestDemo::deleteLater);	// 退出后释放TestObject对象资源
// 	connect(m_playThread, &QThread::finished, m_playThread, &QThread::deleteLater);	// 退出后释放QThread对象资源
	m_playThread->start();
	bl = connect(&_ParCmd, SIGNAL(updateConn(bool)), m_player, SLOT(upConnect(bool)));
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

// void IPCTestDemo::PlayProcess()
// {
// 	this->PlayStart();
// 	
// }

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
		slots_upButton();
		show_info_label->setText(QString::fromLocal8Bit("比对成功"));
	}
	else{
		show_info_label->setText(QString::fromLocal8Bit("比对失败"));
	}
}

QString IPCTestDemo::errCode(QString str)
{
	if (str == "whitelight") { return "IPC001"; }
	else if (str == "irlight") { return "IPC002"; }
	else if (str == "ircut") { return "IPC003"; }
	else if (str == "netled") { return "IPC004"; }
	else if (str == "speakertest") { return "IPC005"; }
	else if (str == "mictest") { return "IPC006"; }
	else if (str == "irnvtest") { return "IPC007"; }
	else if (str == "fullcolornv") { return "IPC008"; }
	else if (str == "keytest") { return "IPC009"; }
	else if (str == "tftest") { return "IPC010"; }
	else if (str == "ldrtest") { return "IPC011"; }
	else if (str == "keywrite") { return "IPC012"; }
	else if (str == "keyread") { return "IPC013"; }
	else if (str == "exittest") { return "IPC014"; }
	else if (str == "devinfo") { return "IPC015"; }
	else if (str == "url") { return "IPC016"; }
	else if (str == "setdefault") { return "IPC017"; }
	else
	{
		return "";
	}
}

QString IPCTestDemo::toTestName(QString str)
{
	if (str == QString::fromLocal8Bit("写运营商信息")) { return "keywrite"; }
	else if(str == QString::fromLocal8Bit("恢复出厂设置")){ return "setdefault"; }
	else if (str == QString::fromLocal8Bit("开关白光灯")) { return "whitelight"; }
	else if (str == QString::fromLocal8Bit("开关红外灯")) { return "irlight"; }
	else if (str == QString::fromLocal8Bit("开关IRCUT")) { return "ircut"; }
	else if (str == QString::fromLocal8Bit("开关网络LED灯")) { return "netled"; }
	else if (str == QString::fromLocal8Bit("测试扬声器")) { return "speakertest"; }
	else if (str == QString::fromLocal8Bit("测试麦克风")) { return "mictest"; }
	else if (str == QString::fromLocal8Bit("测试红外夜视")) { return "irnvtest"; }
	else if (str == QString::fromLocal8Bit("测试全彩夜视")) { return "fullcolornv"; }
	else if (str == QString::fromLocal8Bit("测试Rest按键")) { return "keytest"; }
	else if (str == QString::fromLocal8Bit("测试通话按键")) { return "voipkeytest"; }
	else if (str == QString::fromLocal8Bit("测试TF卡")) { return "tftest"; }
	else if (str == QString::fromLocal8Bit("测试光敏")) { return "ldrtest"; }
	else if (str == QString::fromLocal8Bit("PTZ马达测试")) { return "ptztest"; }
	else if (str == QString::fromLocal8Bit("WiFi测试")) { return "wifiscanresults"; }
	else if (str == QString::fromLocal8Bit("影像清晰度及脏点确认")) { return "video"; }
	else
	{
		return "";
	}
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

void IPCTestDemo::upDataToPage_20(bool bl, QString str, QVariant var)
{
	if(bl){
		QMap<QString, QString> _maps = var.value<QMap<QString, QString>>();
		ui->lineEdit_MOCode->setText(_maps.value("MOCode"));
	}
	show_info_label->setText(str);
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
	show_info_label->setText(str);
}

void IPCTestDemo::upDataToPage_40(bool bl, QString str, QVariant var)
{
	if (bl) {
		QMap<QString, QString> _maps = var.value<QMap<QString, QString>>();
		ui->lineEdit_MAC->setText(_maps.value("Mac"));
		ui->lineEdit_SN->setText(_maps.value("SN"));
		ui->lineEdit_CPassword->setText(_maps.value("CiphertextPassword"));
		ui->lineEdit_DeviceName->setText(_maps.value("DeviceSerialNumber"));
		ui->lineEdit_DevKey->setText(_maps.value("DevKey"));
		ui->lineEdit_UserPass->setText(_maps.value("UserPass"));
		edit_CMEI->setText(_maps.value("CMEI"));
		edit_PCBASN->setText(_maps.value("PCBASN"));
		ui->write_Button->setEnabled(true);
		ui->compar_Button->setEnabled(true);
	}
	else {


	}
	show_info_label->setText(str);
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
			ui->lineEdit_AppVersion->setStyleSheet("color:#FF0000;");
			show_info_label->setStyleSheet("color:#FF0000;");
			startTestBtn->setEnabled(false);
			show_info_label->setText(QString::fromLocal8Bit("固件版本不正确"));
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
	QVariant DataVar;
	QMap<QString, QString> _map;
	_map.insert("SN", ui->lineEdit_SN->text());
	_map.insert("Mac", ui->lineEdit_MAC->text());
	_map.insert("CMEI", edit_CMEI->text());
	_map.insert("DeviceSerialNumber", ui->lineEdit_DeviceName->text());
	_map.insert("DevKey", ui->lineEdit_DevKey->text());
	_map.insert("UserPass", ui->lineEdit_UserPass->text());
	_map.insert("CiphertextPassword", ui->lineEdit_CPassword->text());

	DataVar.setValue(_map);
	emit writeInfo(DataVar);
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
		ui->pushButton_2->setText(QString::fromLocal8Bit("正在连接..."));
		ui->pushButton_2->setEnabled(false);
		ui->write_Button->setEnabled(false);
		ui->compar_Button->setEnabled(false);
		show_info_label->setStyleSheet("color:#22D3EE;");
		show_info_label->setText("");
	}
	else if (ui->pushButton_2->text().compare(QString::fromLocal8Bit("断开连接")) == 0) {
		_ParCmd.tcpDisConnect();
		ui->pushButton_2->setEnabled(false);
		ui->write_Button->setEnabled(false);
		ui->compar_Button->setEnabled(false);
		blRtsp = false;
		this->PlayStop();
		//switch_rtsp();
	}
}

void IPCTestDemo::slots_testButton()
{
	_testlist.clear();
	_mapResult.clear();
	show_info_label->setStyleSheet("color:#22D3EE;");
	show_info_label->setText("");
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

void IPCTestDemo::slots_upButton()
{
	int bl = 1;
	QString str;
	QString errstr;
	QMap<QString, QString>::Iterator iter;
	for (iter = _mapResult.begin(); iter != _mapResult.end(); ++iter)
	{
		if(iter.value() == "2" || iter.value() == "3"){
			bl = 0;
			errstr += errCode(iter.key()) + ";";
		}
		str += iter.key() + ":" + iter.value() + ";";
	}
	_ParCmd.ParsCmd(CMD_RESET, A_TEST);
	_Mesjob->common_send30info(bl, str, errstr);
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
		show_info_label->setStyleSheet("color:#FF0000;");
		show_info_label->setText(QString::fromLocal8Bit("马达恢复初始位置失败."));
		testname = "ptztest";
	}
	_map.value(testname)->setStyleSheet(backColor);
	if (revalue == 0x01) {
		_mapResult.insert(testname, QString::number(revalue));
		_testlist.removeOne(testname);
		//_testlist.pop_front();
		emit sigNextTest();
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
	}
	if (revalue == 0x01) {
		_testlist.removeOne(testname);
		_mapResult.insert(testname, QString::number(revalue));
		//_testlist.pop_front();
		emit sigNextTest();
	}	
}

void IPCTestDemo::slots_NextTest()
{
	if (_testlist.count() != 0)
	{
		if (IsConnectIPC)
			_ParCmd.ParsCmd(gVar->comparTestname(_testlist.first()), A_TEST);
	}
	else if(_testlist.count() == 0)
	{
		QMap<QString, QString> my_mapResult;
		my_mapResult = _mapResult;
		///to do....
		///upto mes
	}
}

void IPCTestDemo::slots_upReturn(QString testname, int revalue)
{
	if (testname.contains("keywrite")) {
		if (revalue == 1) {
			show_info_label->setText(QString::fromLocal8Bit("写入信息成功"));
			clearMesInfo();
			slots_upButton();
		}
		else {
			show_info_label->setText(QString::fromLocal8Bit("写入信息失败"));
		}
	}
	_mapResult.insert(testname, QString::number(revalue));
}

// void IPCTestDemo::keyPressEvent(QKeyEvent *keyValue)
// {
// 	if (keyValue->key() == Qt::Key_Return) //扫码枪内字符串，以回车结尾
// 	{
// //		_Mesjob->common_get20info(ui->lineEdit_SN->text());
// 		_Mesjob->common_get20info(edit_SN->text());
// 	}
// }

void IPCTestDemo::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Return) //扫码枪内字符串，以回车结尾
	{
		clearMesInfoNoCMEI();
#ifdef PCBA
		_Mesjob->common_get20info(edit_PCBASN->text());
#else
		_Mesjob->common_get20info(edit_CMEI->text());
#endif
		//_Mesjob->common_get20info(edit_CMEI->text());
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