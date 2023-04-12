#pragma once
#include <QObject>
#include <QTimer>
#include <QWidget>
#include "prompt.h"
#include "autoprompt.h"
#include "MovingWidget.h"
#include "WifiTestWidget.h"
class BusinessLogic : public QObject
{
	Q_OBJECT
public:
	explicit BusinessLogic(QWidget *parent = nullptr);
	~BusinessLogic();
	void init();
	void setPCMD(ParsingCmd* cmd);
	void reasultAutoTest();
private:
	void bindSinalSlot();
	
private slots:
	//�ֶ�������Ŀ
	void slots_BusiManTest(QString testname, int time);
	
	//�Զ�������Ŀ
	void slots_showAutoTest(QString testname, int time);
	void slots_AutoRequest();
	//PTZ����ƶ�����
	void slot_showMovingWidget();
	//WiFi����
	void slot_showWifiWidget(QVariant);
	

signals:
	void timeout(QString testname, int time);
	void autotimeout(QString testname, int time);
public:
	MovingWidget				_MyMovingWidget;
	WifiTestWidget				_MyWiFiWidget;
private:
	QWidget*					_ParentWideget;
	ParsingCmd*					_ParCMD;
	prompt						_MyPromtWidget;
	autoprompt					_MyAutoPromtWidget;
	ThreadPool::Callback		_ThreadCallBack;
	QTimer*						_TimerRequest;
	QString						_TestName;
	int							_Time;
};