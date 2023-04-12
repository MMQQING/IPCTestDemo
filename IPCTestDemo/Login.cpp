#include "Login.h"
#include <QSettings>
#include "GlobleVar.h"
#include <QMessageBox>
#include <QtNetwork>
#include <QDesktopWidget>
//#include "Mesjob.h"

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
	m_mousePressed = false;
	//ui.objadvcfg->setEnabled(false);
	ui.objlocalip->setView(new QListView());
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	//setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
	//setFixedSize(this->width(), this->height());
	this->setFixedSize(486, 646);
	QDesktopWidget *desktop = QApplication::desktop();
	move((desktop->width() - this->width()) / 2, (desktop->height() - this->height()) / 2);
	//ui.objLogo->setFixedWidth(426);
	CpcLogs::init_log(slog_debug, slog_debug);
	QString ss = QString::fromLocal8Bit("\n�������\n");
	CPCLOG_INFO << ss.toStdString();
	initStyle();
	m_bShowIP = false;
	m_bMesConn = false;
	m_autoConn = false;
	ui.groupBox->setVisible(false);
	this->layout()->setSizeConstraint(QLayout::SetMinimumSize);
	this->setFixedHeight(0);
	//ui.objLoginStatus->setStyleSheet("background-color:#64A600;");
	ui.objadvcfg->setText(QStringLiteral("�߼�>>"));
	//ui.objLogo->setPixmap(QPixmap(":/IPCTestDemo/Resources/logo_slices/logo.png"));
	
	InitPara();
	getLocalIP();
	
	std::shared_ptr<GlobleVar> gInstance = GlobleVar::GetInstance();
 	m_ptrCmd = std::shared_ptr<ParsingCmd>(new ParsingCmd());
	m_ptrMes = std::shared_ptr<Mesjob>(new Mesjob());
 	gInstance->setCmdObject(m_ptrCmd);
 	gInstance->m_stCommonInfo.strconnect = "��Mesδ����";
	bindSignalSlot();
	if (gInstance->m_stCommonInfo.iAutoLogin)
		slot_login();
}

void Login::initStyle()
{
	QFile styleFile(":/IPCTestDemo/Resources/login.qss");
	if (!styleFile.open(QIODevice::ReadOnly))
	{
		//CPCLOG_WARNING << "open style file error, style file path:" + styleFile.fileName().toStdString();
		return;
	}
	this->setStyleSheet(styleFile.readAll());
	//qApp->setStyleSheet(styleFile.readAll());
	styleFile.close();
}

void Login::InitPara()
{
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	//���½���
	ui.objaccount->setText(QString::fromStdString(ptr->m_stCommonInfo.strUsername));
	ui.objpsw->setText(QString::fromStdString(ptr->m_stCommonInfo.strPsw));
	ui.objrescode->setText(QString::fromStdString(ptr->m_stCommonInfo.iResCode));
	ui.objversion->setText(QString::fromStdString(ptr->m_stCommonInfo.firmwareVersion));
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
	bl = connect(ui.objtest, SIGNAL(clicked()), this, SLOT(slot_login()));
	bl = connect(m_ptrMes.get(), SIGNAL(mesConn(bool, QString)), this, SLOT(slot_mesConn(bool, QString)));
	//bl = connect(this, SIGNAL(mesloginStatus(bool)), this, SLOT(slot_loginStatus(bool)));
}

void Login::getLocalIP()
{
	QList<QHostAddress> addrList = QNetworkInterface::allAddresses();
	foreach(QHostAddress addr, addrList)
	{
		if (addr.protocol() == QAbstractSocket::IPv4Protocol)
			ui.objlocalip->addItem(addr.toString());
	}
// 	QString strname = QHostInfo::localHostName();
// 	QHostInfo info = QHostInfo::fromName(strname);
// 	foreach (QHostAddress addr, info.addresses())
// 	{
// 		if (addr.protocol() == QAbstractSocket::IPv4Protocol)
// 			ui.objlocalip->addItem(addr.toString());
// 	}
}

void Login::login()
{
	this->hide();
	if (nullptr == m_xb)
		m_xb = std::make_shared<AddTestItemDialog>();
	m_xb->show();
	m_xb->raise();
}

void Login::slot_advcfg()
{	
	m_bShowIP = !m_bShowIP;
	if (m_bShowIP)
	{
		ui.objadvcfg->setText(QStringLiteral("�߼�<<"));
		ui.groupBox->setVisible(true);
		int w = this->width();
		int h = this->height();
 		adjustSize();
// 		w = this->width();
// 		h = this->height();
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
	//GlobleVar::GetInstance()->m_iLogin = 7;
	this->close();
}

void Login::slot_test()
{
	//���ӷ����TCP
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	ptr->m_stCommonInfo.strUsername = ui.objaccount->text().toStdString();
	ptr->m_stCommonInfo.strPsw = ui.objpsw->text().toStdString();
	ptr->m_stCommonInfo.iResCode = ui.objrescode->text().toStdString();
	ptr->m_stCommonInfo.firmwareVersion = ui.objversion->text().toStdString();
	ptr->m_stCommonInfo.strLocalIP = ui.objlocalip->currentText().toStdString();
	ptr->m_stCommonInfo.strRemoteIP = ui.objServerip->text().toStdString();
	std::shared_ptr<ParsingCmd> ptrCmd = ptr->m_cmdObject;
	if (nullptr == ptrCmd)
		return;
	m_ptrMes->common_login();

	//����ʹ��
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

void Login::mouseMoveEvent(QMouseEvent *e)
{
	if (m_mousePressed && (e->buttons() && Qt::LeftButton) /*&& !m_bmax*/)
	{
		// delta ���ƫ����, 
		QPoint delta = e->globalPos() - m_poStartPosition;
		move(m_poFramePosition + delta);
		e->accept();
	}
}

void Login::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		m_mousePressed = true;
		m_poStartPosition = e->globalPos();
		m_poFramePosition = frameGeometry().topLeft();
		e->accept();
	}
}

void Login::mouseReleaseEvent(QMouseEvent *e)
{
	m_mousePressed = false;
}

void Login::slot_mesConn(bool b, QString str)
{
	if (b)
	{
		ui.objLoginStatus->setText(str);
		//ui.objLoginStatus->setStyleSheet("background-color:#64A600;");
		m_bMesConn = true;
		GlobleVar::GetInstance()->savepara(GlobleVar::GetInstance()->m_strXmlCfgPath.toStdString());

		login();
	}
	else
	{
		ui.objLoginStatus->setText(str);
		//ui.objLoginStatus->setStyleSheet("background-color:#A23400;");
		m_bMesConn = false;
	}
}