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
	//手动测试项目
	void slots_BusiManTest(QString testname, int time);
	
	//自动测试项目
	void slots_showAutoTest(QString testname, int time);
	void slots_AutoRequest();
	//PTZ马达移动界面
	void slot_showMovingWidget();
	//WiFi界面
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