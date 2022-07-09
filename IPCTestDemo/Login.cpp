#include "Login.h"
#include <QSettings>
#include "GlobleVar.h"
#include <QMessageBox>
#include <QtNetwork>

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
	ui.objadvcfg->setText(QStringLiteral("�߼�>>"));
	ui.objLogo->setPixmap(QPixmap(":/IPCTestDemo/Resources/logo1.png"));
	
	InitPara();
	getLocalIP();
	
	std::shared_ptr<GlobleVar> gInstance = GlobleVar::GetInstance();
 	m_ptrCmd = std::shared_ptr<ParsingCmd>(new ParsingCmd());
 	gInstance->setCmdObject(m_ptrCmd);
 	gInstance->m_stCommonInfo.strconnect = "������δ����";
	bindSignalSlot();
//	if (gInstance->m_stCommonInfo.iAutoLogin)
//		slot_login();
}

void Login::initStyle()
{
	QFile styleFile(":/Portable/Resources/style.qss");
	if (!styleFile.open(QIODevice::ReadOnly))
	{
		CPCLOG_WARNING << "open style file error, style file path:" + styleFile.fileName().toStdString();
		return;
	}
	this->setStyleSheet(styleFile.readAll());
	styleFile.close();
}

void Login::InitPara()
{
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	//���½���
	ui.objaccount->setText(QString::fromStdString(ptr->m_stCommonInfo.strUsername));
	ui.objpsw->setText(QString::fromStdString(ptr->m_stCommonInfo.strPsw));
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
	bl = connect(m_ptrCmd.get(), SIGNAL(tcpConn(bool)), this, SLOT(slot_tcpConn(bool)));
	bl = connect(m_ptrCmd.get(), SIGNAL(loginStatus(int)), this, SLOT(slot_loginStatus(int)));
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
		ui.objadvcfg->setText(QStringLiteral("�߼�<<"));
		ui.groupBox->setVisible(true);		
	}
	else
	{
		ui.objadvcfg->setText(QStringLiteral("�߼�>>"));
		ui.groupBox->setVisible(false);
		adjustSize();
	}	
}

void Login::slot_login()
{	
	if (!m_bMesConn)
		//��¼ǰ�Զ������������ӣ����ӳɹ����͵�¼������򲻷��͵�¼�����ʾ�û��޸������ַ���в���
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
	//���ӷ����TCP
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	ptr->m_stCommonInfo.strLocalIP = ui.objlocalip->currentText().toStdString();
	ptr->m_stCommonInfo.strRemoteIP = ui.objServerip->text().toStdString();
	std::shared_ptr<ParsingCmd> ptrCmd = ptr->m_cmdObject;
	if (nullptr == ptrCmd)
		return;
	ptrCmd->mesConnect();

	//����ʹ��
	this->hide();
	if (nullptr == m_xb)
		m_xb = std::make_shared<AddTestItemDialog>();
	m_xb->show();
	m_xb->raise();
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

void Login::slot_tcpConn(bool b)
{
	if (b)
	{
		ui.objLoginStatus->setText(QStringLiteral("���ӳɹ�����<��¼>..."));
		ui.objLoginStatus->setStyleSheet("background-color:#64A600;");
		m_bMesConn = true;
		GlobleVar::GetInstance()->savepara(GlobleVar::GetInstance()->m_strXmlCfgPath.toStdString());

		login();
	}
	else
	{
		ui.objLoginStatus->setText(QStringLiteral("����ʧ�ܣ����޸�Url������..."));
		ui.objLoginStatus->setStyleSheet("background-color:#A23400;");
		m_bMesConn = false;
	}
}

void Login::slot_loginStatus(int iStatus)
{
	if (E_Login_OK == (eLoginState)iStatus)
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