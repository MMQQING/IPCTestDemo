#pragma once
#include <QObject>
#include <QTimer>
#include "prompt.h"
#include "autoprompt.h"
class BusinessLogic : public QObject
{
	Q_OBJECT
public:
	BusinessLogic();
	~BusinessLogic();
	void init();
	void setPCMD(ParsingCmd* cmd);
private:
	void bindSinalSlot();
	
private slots:
	//手动测试项目
	void slots_ManTest(QString testname, int time);
	
	//自动测试项目
	void slots_AutoTest(QString testname, int time);
	void slots_AutoRequest();
signals:
	void timeout(QString testname, int time);
	void autotimeout(QString testname, int time);
private:
	ParsingCmd*					_ParCMD;
	prompt						_MyPromtWidget;
	autoprompt					_MyAutoPromtWidget;
	ThreadPool::Callback		_ThreadCallBack;
	QTimer*						_TimerRequest;
	QString						_TestName;
};