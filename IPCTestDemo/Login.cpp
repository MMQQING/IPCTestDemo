#include "Login.h"
#include <QSettings>
#include "GlobleVar.h"
#include <QMessageBox>
#include <QtNetwork>
#include "Mesjob.h"

Login::Login(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);	
	Init();
}

Login::~Login()
{
}

void Login::Init()
{
	//ui.objadvcfg->setEnabled(false);
	setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
	setFixedSize(this->width(), this->height());	
	CpcLogs::init_log((severity_levels)7, (severity_levels)7);
	initStyle();
	m_bShowIP = false;
	m_bMesConn = false;
	m_autoConn = false;
	ui.groupBox->setVisible(false);
	this->layout()->setSizeConstraint(QLayout::SetMinimumSize);
	this->setFixedHeight(0);
	ui.objLoginStatus->setStyleSheet("background-color:#64A600;");
	ui.objadvcfg->setText(QStringLiteral("高级>>"));
	ui.objLogo->setPixmap(QPixmap(":/IPCTestDemo/Resources/logo1.png"));
	
	InitPara();
	getLocalIP();
	
	std::shared_ptr<GlobleVar> gInstance = GlobleVar::GetInstance();
 	m_ptrCmd = std::shared_ptr<ParsingCmd>(new ParsingCmd());
 	gInstance->setCmdObject(m_ptrCmd);
 	gInstance->m_stCommonInfo.strconnect = "与Mes未连接";
	bindSignalSlot();
	if (gInstance->m_stCommonInfo.iAutoLogin)
		slot_login();
}

void Login::initStyle()
{
	QFile styleFile(":/Portable/Resources/style.qss");
	if (!styleFile.open(QIODevice::ReadOnly))
	{
		//CPCLOG_WARNING << "open style file error, style file path:" + styleFile.fileName().toStdString();
		return;
	}
	this->setStyleSheet(styleFile.readAll());
	styleFile.close();
}

void Login::InitPara()
{
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	//更新界面
	ui.objaccount->setText(QString::fromStdString(ptr->m_stCommonInfo.strUsername));
	ui.objpsw->setText(QString::fromStdString(ptr->m_stCommonInfo.strPsw));
	ui.objrescode->setText(QString::fromStdString(ptr->m_stCommonInfo.iResCode));
	ui.objautologin->setChecked(ptr->m_stCommonInfo.iAutoLogin);
	ui.objServerip->setText(QString::fromStdString(ptr->m_stCommonInfo.strRemoteIP));
	ui.objlocalip->setCurrentText(QString::fromStdString(ptr->m_stCommonInfo.strLocalIP));
}

void Login::bindSignalSlot()
{
	bool bl = connect(ui.objadvcfg, SIGNAL(clicked()), this, SLOT(slot_advcfg()));
	bl = connect(ui.objlogin, SIGNAL(clicked()), this, SLOT(slot_login()));
	bl = connect(ui.objquit, SIGNAL(clicked()), this, SLOT(slot_quit()));
	bl = connect(ui.objlocalip, SIGNAL(clicked()), this, SLOT(slot_updateLIP()));
	bl = connect(ui.objautologin, SIGNAL(toggled(bool)), this, SLOT(slot_autoLogin(bool)));
	bl = connect(ui.objtest, SIGNAL(clicked()), this, SLOT(slot_test()));
	bl = connect(m_ptrCmd.get()->m_ptrMes.get(), SIGNAL(mesConn(bool, QString)), this, SLOT(slot_mesConn(bool, QString)));
	bl = connect(m_ptrCmd.get(), SIGNAL(mesloginStatus(bool)), this, SLOT(slot_loginStatus(bool)));
}

void Login::getLocalIP()
{
	QList<QHostAddress> addrList = QNetworkInterface::allAddresses();
	foreach(QHostAddress addr, addrList)
	{
		if (addr.protocol() == QAbstractSocket::IPv4Protocol)
			ui.objlocalip->addItem(addr.toString());
	}
	QString strname = QHostInfo::localHostName();
	QHostInfo info = QHostInfo::fromName(strname);
	foreach (QHostAddress addr, info.addresses())
	{
		if (addr.protocol() == QAbstractSocket::IPv4Protocol)
			ui.objlocalip->addItem(addr.toString());
	}
}

void Login::login()
{
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	if (nullptr == ptr->m_cmdObject)
		return;
	ptr->m_stCommonInfo.strUsername = ui.objaccount->text().toStdString();
	ptr->m_stCommonInfo.strPsw = ui.objpsw->text().toStdString();
	ptr->m_stCommonInfo.iAccess = 0;

//	ptr->m_cmdObject->ParsCmd(0, 0, CMD_CTRL_LOGIN);
}

void Login::slot_advcfg()
{	
	m_bShowIP = !m_bShowIP;
	if (m_bShowIP)
	{
		ui.objadvcfg->setText(QStringLiteral("高级<<"));
		ui.groupBox->setVisible(true);		
	}
	else
	{
		ui.objadvcfg->setText(QStringLiteral("高级>>"));
		ui.groupBox->setVisible(false);
		adjustSize();
	}	
}

void Login::slot_login()
{	
	if (!m_bMesConn)
		//登录前自动进行网络连接，连接成功发送登录命令，否则不发送登录命令，提示用户修改网络地址进行测试
		slot_test();
	else
		login();
}

void Login::slot_quit()
{
	GlobleVar::GetInstance()->m_iLogin = 7;
	this->close();
}

void Login::slot_test()
{
	//连接服务端TCP
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	ptr->m_stCommonInfo.strUsername = ui.objaccount->text().toStdString();
	ptr->m_stCommonInfo.strPsw = ui.objpsw->text().toStdString();
	ptr->m_stCommonInfo.iResCode = ui.objrescode->text().toStdString();
	ptr->m_stCommonInfo.strLocalIP = ui.objlocalip->currentText().toStdString();
	ptr->m_stCommonInfo.strRemoteIP = ui.objServerip->text().toStdString();
	std::shared_ptr<ParsingCmd> ptrCmd = ptr->m_cmdObject;
	if (nullptr == ptrCmd)
		return;
	ptrCmd->mesConnect();

	//测试使用
// 	this->hide();
// 	if (nullptr == m_xb)
// 		m_xb = std::make_shared<AddTestItemDialog>();
// 	m_xb->show();
// 	m_xb->raise();
	return;
}

void Login::slot_autoLogin(bool b)
{
	GlobleVar::GetInstance()->m_stCommonInfo.iAutoLogin = (b == true ? 1 : 0);
}

void Login::slot_updateLIP()
{
	ui.objlocalip->clear();
	getLocalIP();
}

void Login::slot_mesConn(bool b, QString str)
{
	if (b)
	{
		ui.objLoginStatus->setText(str);
		ui.objLoginStatus->setStyleSheet("background-color:#64A600;");
		m_bMesConn = true;
		GlobleVar::GetInstance()->savepara(GlobleVar::GetInstance()->m_strXmlCfgPath.toStdString());

		login();
	}
	else
	{
		ui.objLoginStatus->setText(str);
		ui.objLoginStatus->setStyleSheet("background-color:#A23400;");
		m_bMesConn = false;
	}
}

void Login::slot_loginStatus(bool iStatus)
{
	if (iStatus)
	{
		this->hide();
		if(nullptr == m_xb)
			m_xb = std::make_shared<AddTestItemDialog>();
		m_xb->show();
		m_xb->raise();
	}
	else
	{
		ui.objLoginStatus->setText(QString::fromStdString(strLogin[iStatus]));
		ui.objLoginStatus->setStyleSheet("background-color:#A23400;");
	}
}