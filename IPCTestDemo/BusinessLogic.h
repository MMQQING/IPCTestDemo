#pragma once
#include <QObject>
#include <QTimer>
#include "prompt.h"
class BusinessLogic : public QObject
{
	Q_OBJECT
public:
	BusinessLogic();
	~BusinessLogic();
	void init();
private:
	void bindSinalSlot();
	
private slots:
	//�ֶ�������Ŀ
	void slots_ManTest(QString testname, int time);
	//�Զ�������Ŀ
	void slots_AutoTest(QString testname, int time);
	void slots_AutoRequest();
signals:
	void timeout(QString testname, int time);
private:
	ParsingCmd					_ParCMD;
	prompt						_MyPromtWidget;
	ThreadPool::Callback		_ThreadCallBack;
	QTimer*						_TimerRequest;
	QString						_TestName;
};