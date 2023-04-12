#pragma once
#include <QWidget>
#include "ui_Login.h"
#include "AddTestItemDialog.h"

class AddTestItemDialog;
class Login : public QWidget
{
	Q_OBJECT

public:
	Login(QWidget *parent = Q_NULLPTR);
	~Login();

private:
	Ui::Login ui;
	bool m_bShowIP;
	std::shared_ptr<ParsingCmd>					m_ptrCmd;
	bool m_bMesConn;  //Mes连接状态
	std::shared_ptr<AddTestItemDialog>			m_xb;
	bool m_autoConn;  //自动连接
	std::shared_ptr<Mesjob>						m_ptrMes;
	bool										m_mousePressed;
	QPoint m_poStartPosition;	// 拖动开始前的鼠标位置
	QPoint m_poFramePosition;	// 窗体的原始位置

private:
	void Init();
	void initStyle();
	void InitPara();
	void bindSignalSlot();
	void getLocalIP();
	void login();	
signals:
	void mesloginStatus(bool istatus);//mes登录状态
private slots:
	void slot_advcfg();
	void slot_login();
	void slot_quit();
	void slot_test();
	void slot_autoLogin(bool b);
	void slot_updateLIP();
protected:
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

public slots:
	void slot_mesConn(bool b,QString str);
};