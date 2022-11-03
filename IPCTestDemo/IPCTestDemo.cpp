#include "IPCTestDemo.h"
#include "GlobleVar.h"
#include <QKeyEvent> //用于键盘事件
#include "RTSP_Player.h"
#include "MyHelper.h"
#include <QPainter>
#include <QMetaType>
#include <algorithm>
#include <QFormLayout>

IPCTestDemo::IPCTestDemo(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::IPCTestDemo)
{
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
	bool bl = connect(&_ParCmd, SIGNAL(manTimeout(QString,int)), _Business, SLOT(slots_ManTest(QString,int)));
	bl = connect(&_ParCmd, SIGNAL(autoTimeout(QString, int)), _Business, SLOT(slots_AutoTest(QString, int)));
	bl = connect(_Mesjob.get(), SIGNAL(mes40Res(bool, QString)), this, SLOT(upDataToPage_40(bool, QString)));
	bl = connect(_Mesjob.get(), SIGNAL(mes20Res(bool, QString)), this, SLOT(upDataToPage_20(bool, QString)));
	bl = connect(&_ParCmd, SIGNAL(message(QVariant, QString)), this, SLOT(upDataFromIPC(QVariant, QString)));
	bl = connect(&_ParCmd, SIGNAL(tcpConn(bool)), this, SLOT(slots_isConnectIPC(bool)));
	bl = connect(&_ParCmd, SIGNAL(manResult(QString, int)), this, SLOT(slots_ManTest(QString, int)));
	bl = connect(&_ParCmd, SIGNAL(autoResult(QString, int)), this, SLOT(slots_AutoTest(QString, int)));
	bl = connect(&_ParCmd, SIGNAL(myResult(QString, int)), this, SLOT(slots_upReturn(QString, int)));
	bl = connect(this, SIGNAL(sigNextTest()), this, SLOT(slots_NextTest()));
	bl = connect(ui->compar_Button, SIGNAL(clicked()), this, SLOT(SlotComparInfo())); 
	bl = connect(ui->write_Button, SIGNAL(clicked()), this, SLOT(SlotWriteInfo())); 
	bl = connect(ui->default_Button, SIGNAL(clicked()), this, SLOT(SlotDefault()));
}

void IPCTestDemo::initPage()
{
	//设置主界面标题栏自绘
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	setWindowState(Qt::WindowMaximized);
	ui->groupBox_3->setMaximumWidth(ui->groupBox_2->width());
	//ui.horizontalLayout->setStretchFactor(ui.widget_4,3);
	ui->horizontalLayout->setStretchFactor(ui->widget_5, 1);
	QVBoxLayout* layor = (QVBoxLayout*)ui->widget_4->layout();
	layor->setStretchFactor(ui->groupBox_2, 5);
	layor->setStretchFactor(ui->groupBox_3, 5);
	ui->horizontalLayout_3->setStretchFactor(ui->lineEdit_IP,7);
	ui->horizontalLayout_3->setStretchFactor(ui->pushButton_2,3);

	ui->lineEdit_IP->hide();
	ui->lineEdit_connect->setEnabled(false);
	ui->lineEdit_MAC->setEnabled(false);
	ui->lineEdit_SN->setEnabled(false);
	ui->lineEdit_UID->setEnabled(false);
	ui->lineEdit_DevKey->setEnabled(false);
	ui->lineEdit_UserPass->setEnabled(false);
	ui->lineEdit_DeviceName->setEnabled(false);
	ui->lineEdit_AppVersion->setEnabled(false);
	ui->lineEdit_DeviceModel->setEnabled(false);

	//ui.lineEdit_SN->setFocusPolicy(Qt::StrongFocus);
	//qApp->installEventFilter(this); //为所有控件添加事件过滤器

	m_ffmpeg = new MyFFmpeg;
	_Business = new BusinessLogic(ui->groupBox_2);
	bool bl = connect(m_ffmpeg, SIGNAL(MyFFmpegSigGetOneFrame(QImage)), this, SLOT(SlotGetOneFrame(QImage)));

	ui->label->setStyleSheet("QLabel{border:2px solid green}");
	ui->label->installEventFilter(this);
	ui->pushButton->setEnabled(true);
	edit_CMEI = new ButtonEdit(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
	QObject::connect(edit_CMEI, &ButtonEdit::buttonClicked, [&](bool) {edit_CMEI->setText(""); });
	auto layout = new QFormLayout;
	layout->addRow(edit_CMEI);
	layout->setMargin(0);
	ui->CMEI_widget->setLayout(layout);
	
	edit_AppVersion = new ButtonEdit(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
	QObject::connect(edit_AppVersion, &ButtonEdit::buttonClicked, [&](bool) {edit_AppVersion->setText(""); });
	auto layout_1 = new QFormLayout;
	layout_1->addRow(edit_AppVersion);
	layout_1->setMargin(0);
	ui->AppVersion_widget->setLayout(layout_1);
}

void IPCTestDemo::initParam()
{
	_Mesjob = std::shared_ptr<Mesjob>(new Mesjob());
	IsConnectIPC = false;
}

void IPCTestDemo::PlayStart()
{
	this->PlayStop();

	m_playThread = new QThread();
	m_player = new RTSP_Player(m_ffmpeg);
	m_player->SetPlayerUrl(m_playUrl);

	connect(this, SIGNAL(SigPlayStart()), m_player, SLOT(PlayerStart()));
	connect(m_player, SIGNAL(SigOpenUrlResult(int)), this, SLOT(SlotOpenUrlResult(int)));

	m_player->moveToThread(m_playThread);
	m_playThread->start();
}

void IPCTestDemo::PlayStop()
{
	if (m_player)
	{
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

void IPCTestDemo::PlayProcess()
{
	this->PlayStart();
	emit SigPlayStart();
}

void IPCTestDemo::getDeviceInfo()
{
	_ParCmd.ParsCmd(CMD_DEVIINFO, A_TEST);
}

void IPCTestDemo::comparInfo(QMap<QString, QString> map)
{
	bool bl = true;
	if (map.value("cmei") == edit_CMEI->text()) {
		edit_CMEI->setStyleSheet("background-color:#64A600;");
	}
	else{
		bl = false;
		edit_CMEI->setStyleSheet("background-color:#A23400;");
	}

	if(map.value("mac") == ui->lineEdit_MAC->text()) {
		ui->lineEdit_MAC->setStyleSheet("background-color:#64A600;");
	}
	else{
		bl = false;
		ui->lineEdit_MAC->setStyleSheet("background-color:#A23400;");
	}

	if (map.value("sn") == ui->lineEdit_SN->text()) {
		ui->lineEdit_SN->setStyleSheet("background-color:#64A600;");
	}
	else{
		bl = false;
		ui->lineEdit_SN->setStyleSheet("background-color:#A23400;");
	}

	if (map.value("deviceserialnumber") == ui->lineEdit_UID->text()) {
		ui->lineEdit_UID->setStyleSheet("background-color:#64A600;");
	}
	else{
		bl = false;
		ui->lineEdit_UID->setStyleSheet("background-color:#A23400;");
	}

	if (map.value("devkey") == ui->lineEdit_DevKey->text()) {
		ui->lineEdit_DevKey->setStyleSheet("background-color:#64A600;");
	}
	else{
		bl = false;
		ui->lineEdit_DevKey->setStyleSheet("background-color:#A23400;");
	}

	if (map.value("userpass") == ui->lineEdit_UserPass->text()) {
		ui->lineEdit_UserPass->setStyleSheet("background-color:#64A600;");
	}
	else{
		bl = false;
		ui->lineEdit_UserPass->setStyleSheet("background-color:#A23400;");
	}
	if(bl){
		slots_upButton();
	}
	else{
		QString str = "比对失败";
		ui->label_2->setText(str);
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
	else if (str == QString::fromLocal8Bit("测试TF卡")) { return "tftest"; }
	else if (str == QString::fromLocal8Bit("测试光敏")) { return "ldrtest"; }
	else
	{
		return "";
	}
}

void IPCTestDemo::clearMesInfo()
{
	ui->lineEdit_MAC->setText("");
	ui->lineEdit_SN->setText("");
	ui->lineEdit_UID->setText("");
	ui->lineEdit_DevKey->setText("");
	ui->lineEdit_UserPass->setText("");
	edit_CMEI->setText("");
	ui->lineEdit_MAC->setStyleSheet("background-color:#F0F0F0;");
	ui->lineEdit_SN->setStyleSheet("background-color:#F0F0F0;");
	ui->lineEdit_UID->setStyleSheet("background-color:#F0F0F0;");
	ui->lineEdit_DevKey->setStyleSheet("background-color:#F0F0F0;");
	ui->lineEdit_UserPass->setStyleSheet("background-color:#F0F0F0;");
}

void IPCTestDemo::clearAllInfo()
{
	ui->lineEdit_MAC->setText("");
	ui->lineEdit_SN->setText("");
	ui->lineEdit_UID->setText("");
	ui->lineEdit_DevKey->setText("");
	ui->lineEdit_UserPass->setText("");
	ui->lineEdit_DeviceName->setText("");
	ui->lineEdit_AppVersion->setText("");
	ui->lineEdit_DeviceModel->setText("");
	edit_CMEI->setText("");
	ui->lineEdit_MAC->setStyleSheet("background-color:#F0F0F0;");
	ui->lineEdit_SN->setStyleSheet("background-color:#F0F0F0;");
	ui->lineEdit_UID->setStyleSheet("background-color:#F0F0F0;");
	ui->lineEdit_DevKey->setStyleSheet("background-color:#F0F0F0;");
	ui->lineEdit_UserPass->setStyleSheet("background-color:#F0F0F0;");
	ui->lineEdit_DeviceName->setStyleSheet("background-color:#F0F0F0;");
	ui->lineEdit_AppVersion->setStyleSheet("background-color:#F0F0F0;");
	ui->lineEdit_DeviceModel->setStyleSheet("background-color:#F0F0F0;");
}

void IPCTestDemo::initTestItems(QVector<QString> vec)
{
	_testvec = vec;
	QVBoxLayout* layout = new QVBoxLayout;
	for(auto a: vec)
	{
		QLabel* label = new QLabel;
		label->setText(a);
		layout->addWidget(label);
		_map.insert(toTestName(a),label);
	}
	
	QHBoxLayout* hlayout = new QHBoxLayout();
	hlayout->addStretch();
	QPushButton* btn = new QPushButton(QString::fromLocal8Bit("开始测试"));
	btn->setObjectName("m_testButton");
	hlayout->addWidget(btn);
	QPushButton* btn1 = new QPushButton(QString::fromLocal8Bit("上传结果"));
	btn1->setObjectName("m_upButton");
	hlayout->addWidget(btn1);
	layout->addLayout(hlayout);
	ui->groupBox_3->setLayout(layout);
	bool bl = connect(btn, SIGNAL(clicked()), this, SLOT(slots_testButton()));
	bl = connect(btn1, SIGNAL(clicked()), this, SLOT(slots_upButton()));
	btn1->hide();
}

void IPCTestDemo::upDataToPage_20(bool bl, QString str)
{
	if (bl)
	{
		clearAllInfo();
		if (ui->pushButton->text().compare("stop") == 0) {
			ui->pushButton->setEnabled(false);
			this->PlayStop();
		}
		_ParCmd.tcpDisConnect();
	}
	ui->label_2->setText(str);
}

void IPCTestDemo::upDataToPage_40(bool bl, QString str)
{
	if (bl) {
		ui->lineEdit_MAC->setText(QString::fromStdString(gVar->m_st40Info.Mac));
		ui->lineEdit_SN->setText(QString::fromStdString(gVar->m_st40Info.SN));
		ui->lineEdit_UID->setText(QString::fromStdString(gVar->m_st40Info.DeviceSerialNumber));
		ui->lineEdit_DevKey->setText(QString::fromStdString(gVar->m_st40Info.DevKey));
		ui->lineEdit_UserPass->setText(QString::fromStdString(gVar->m_st40Info.UserPass));
		//ui.lineEdit_checkCode->setText(QString::fromStdString(gVar->m_st40Info.));
		//ui.lineEdit_firmVer->setText(QString::fromStdString(gVar->m_st40Info.));
	}
	else {


	}
	ui->label_2->setText(str);
}

void IPCTestDemo::upDataFromIPC(QVariant var, QString str)
{
	QMap<QString, QString> _maps = var.value<QMap<QString, QString>>();
	if (str == "devinfo") {
		ui->lineEdit_DeviceName->setText(_maps.value("device_name"));
		ui->lineEdit_DeviceModel->setText(_maps.value("device_model"));
		QString str = _maps.value("app_version");
//		if (str.compare(ui->AppVersionIn_lineEdit->text()) == 0)
		if (str.compare(edit_AppVersion->text()) == 0)
		{
			ui->lineEdit_AppVersion->setStyleSheet("background-color:#64A600;");
		}
		else
		{
			ui->lineEdit_AppVersion->setStyleSheet("background-color:#A23400;");
		}
		ui->lineEdit_AppVersion->setText(str);
	}
	else if(str == "url"){
		foreach(const QString str, _maps.keys())
		{
			ui->lineEdit->setText(_maps.value(str));
		}
	}
	else if(str == "keyread"){
		comparInfo(_maps);
	}
}

void IPCTestDemo::SlotGetOneFrame(QImage img)
{
	m_image = img;
	this->update();
}

void IPCTestDemo::SlotOpenUrlResult(int result)
{
	if (result == RTSP_Player::FFmpegInitFail)
	{
		this->PlayStop();

		QString rtspUrl = ui->lineEdit->text().trimmed();
		QString errMsg = "打开" + rtspUrl + "失败";
		MyHelper::ShowMessageBoxError(errMsg);
		ui->pushButton->setText("start");
	}
	else if (result == RTSP_Player::FFmpegInitSucc)
	{
		ui->pushButton->setText("stop");
	}
	else if (result == RTSP_Player::FFmpegStoped)
	{
		ui->pushButton->setText("start");
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
				this->PlayProcess();
			}
		}
#endif
	}

	ui->pushButton->setEnabled(true);
}

void IPCTestDemo::SlotComparInfo()
{
	_ParCmd.ParsCmd(CMD_KEYREAD, A_TEST);
}

void IPCTestDemo::SlotWriteInfo()
{
	_ParCmd.ParsCmd(CMD_KEYWRITE, A_TEST);
}

void IPCTestDemo::SlotDefault()
{
	_ParCmd.ParsCmd(CMD_SETDEFAULT, A_TEST);
}

void IPCTestDemo::on_pushButton_clicked()
{
	if (ui->pushButton->text().compare("start") == 0) {
		m_playUrl = ui->lineEdit->text().trimmed();
		if (m_playUrl.length() <= (int)strlen("rtsp://")) {
			MyHelper::ShowMessageBoxError(QString::fromLocal8Bit("请输入rtsp播放地址!"));
			return;
		}

		ui->pushButton->setEnabled(false);
		_ParCmd.ParsCmd(CMD_SETTIME, A_TEST);
		this->PlayProcess();
	}
	else if (ui->pushButton->text().compare("stop") == 0) {
		ui->pushButton->setEnabled(false);
		this->PlayStop();
	}
}

void IPCTestDemo::on_pushButton_2_clicked()
{
	if (ui->pushButton_2->text().compare(QString::fromLocal8Bit("连接")) == 0) {
		_ParCmd.tcpConnect();
		ui->pushButton_2->setEnabled(false);
	}
	else if (ui->pushButton_2->text().compare(QString::fromLocal8Bit("断开连接")) == 0) {
		_ParCmd.tcpDisConnect();
		ui->pushButton_2->setEnabled(false);
	}
}

void IPCTestDemo::slots_testButton()
{
	_testlist.clear();
	_mapResult.clear();
	QMap<QString, QLabel*>::iterator iter = _map.begin();
	while (iter != _map.end())
	{
		iter.value()->setStyleSheet("background-color:#F0F0F0;"); // 迭代器
		iter++;
	}
	for (auto a: _testvec)
	{
		_testlist.push_back(toTestName(a));
	}
	//_testlist = _map.keys();
	if (_testlist.count() != 0)
	{
		if (IsConnectIPC)
			_ParCmd.ParsCmd(gVar->comparTestname(_testlist.first()), A_TEST);
	}
	//if(IsConnectIPC)
	//	_ParCmd.ParsCmd(CMD_WHITELIGHT, A_TEST);
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
	_Mesjob->common_send30info(bl, str, errstr);
}

void IPCTestDemo::slots_isConnectIPC(bool bl)
{
	if (bl) {
		_ParCmd.ParsCmd(CMD_URL, A_TEST);
		ui->lineEdit_connect->setText(QString::fromLocal8Bit("连接成功"));
		ui->pushButton_2->setText(QString::fromLocal8Bit("断开连接"));
		Sleep(100);
		getDeviceInfo();
		_Business->setPCMD(&_ParCmd);
	}
	else
	{
		ui->pushButton_2->setText(QString::fromLocal8Bit("连接"));
		ui->lineEdit_connect->setText(QString::fromLocal8Bit("连接失败"));
	}
	IsConnectIPC = bl;
	ui->pushButton_2->setEnabled(true);
}

void IPCTestDemo::slots_ManTest(QString testname, int revalue)
{
	QString backColor;
	switch (revalue)
	{
	case 0:backColor = "background-color:#64A600;"; break;
	case 1:backColor = "background-color:green;"; break;
	case 2:backColor = "background-color:red;"; break;
	case 3:backColor = "background-color:blue;"; break;
	case 4:backColor = "background-color:yellow;"; break;
	default:
		break;
	}
	_map.value(testname)->setStyleSheet(backColor);
	_mapResult.insert(testname, QString::number(revalue));
	_testlist.removeOne(testname);
	//_testlist.pop_front();
	emit sigNextTest();
}

void IPCTestDemo::slots_AutoTest(QString testname, int revalue)
{
	QString backColor;
	switch (revalue)
	{
	case 0:backColor = "background-color:#64A600;"; break;
	case 1:backColor = "background-color:green;"; break;
	case 2:backColor = "background-color:red;"; break;
	case 3:backColor = "background-color:blue;"; break;
	case 4:backColor = "background-color:yellow;"; break;
	default:
		break;
	}
	_map.value(testname)->setStyleSheet(backColor);
	if(revalue != 0x04){
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
			ui->label_2->setText("写入信息成功");
			clearMesInfo();
		}
		else {
			ui->label_2->setText("写入信息失败");
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
		_Mesjob->common_get20info(edit_CMEI->text());
		//qDebug() << "QrCode R:" << barStr;
	}
}

void IPCTestDemo::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	QRect rec = ui->label->geometry();

	if (m_image.size().width() <= 0)
		return;
	QImage img = m_image.scaled(ui->label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	int x = ui->label->geometry().x() + ui->widget_5->geometry().x() + 9;
	int y = ui->label->geometry().y() + ui->widget->height() + 27;
	//ui.label->setPixmap(QPixmap::fromImage(img));
	painter.drawImage(QPoint(x, y), img);
}

void IPCTestDemo::closeEvent(QCloseEvent *e)
{
	this->PlayStop();
}
