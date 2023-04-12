#include "BusinessLogic.h"
#include "GlobleVar.h"
#include <qDebug>
BusinessLogic::BusinessLogic(QWidget *parent)
{
	_ParentWideget = parent;
	init();
}

BusinessLogic::~BusinessLogic()
{
}

void BusinessLogic::init()
{
	//_TimerRequest = new QTimer(this);
	bindSinalSlot();
}

void BusinessLogic::setPCMD(ParsingCmd* cmd)
{
	_ParCMD = cmd;
	_MyPromtWidget.setPCMD(cmd);
	_MyAutoPromtWidget.setPCMD(cmd);
	_MyMovingWidget.setPCMD(cmd);
	_MyWiFiWidget.setPCMD(cmd);
}

void BusinessLogic::bindSinalSlot()
{
	bool bl = connect(this, SIGNAL(timeout(QString, int)), &_MyPromtWidget, SLOT(slots_timer_out(QString, int)));
	//bl = connect(_TimerRequest, &QTimer::timeout, this, &BusinessLogic::slots_AutoRequest);
	bl = connect(this, SIGNAL(autotimeout(QString, int)), &_MyAutoPromtWidget, SLOT(slots_timer_out(QString, int)));
}

void BusinessLogic::slots_BusiManTest(QString testname, int time)
{
	_TestName = testname;
	int _timeout = time * 1000;
	QPoint p = _ParentWideget->mapToGlobal(QPoint(0, 0)); 
	int w = _ParentWideget->width() * 0.9;
	_MyPromtWidget.setTitle(testname, w, 160);
	_MyPromtWidget.move(p.x() + (w * 0.05), p.y() + 100);
	_MyPromtWidget.show();
	_MyPromtWidget.raise();
	emit timeout(_TestName,_timeout);
}

void BusinessLogic::slots_showAutoTest(QString testname, int time)
{
	_TestName = testname;
	_Time = time;
	int _timeout = time * 1000;
	QPoint p = _ParentWideget->mapToGlobal(QPoint(0, 0));
	int w = _ParentWideget->width() * 0.9;
	_MyAutoPromtWidget.setTitle(testname, w, 160);
	_MyAutoPromtWidget.move(p.x() + (w * 0.05), p.y() + 100);
	_MyAutoPromtWidget.show();
	_MyAutoPromtWidget.raise();
	emit autotimeout(_TestName,_timeout);

	_TimerRequest = new QTimer(this);
	bool bl = connect(_TimerRequest, &QTimer::timeout, this, &BusinessLogic::slots_AutoRequest);
	_TimerRequest->start(1000);
	qDebug() << "timeout:" << _timeout;
// 	QTimer::singleShot(_timeout, this, [=]() {
// 		_TimerRequest->stop();
// 		_MyAutoPromtWidget.close_window();
// 	});
}

void BusinessLogic::reasultAutoTest()
{
	_TimerRequest->stop();
	_MyAutoPromtWidget.close_window();
}

void BusinessLogic::slot_showMovingWidget()
{
// 	int x = _ParentWideget->geometry().x();
// 	int y = _ParentWideget->geometry().y() + 27 + _ParentWideget->geometry().height() - 200;
// 	_MyMovingWidget.move(x, y);
	QPoint p = _ParentWideget->mapToGlobal(QPoint(0, 0));
	_MyMovingWidget.move(p.x(), p.y());
	_MyMovingWidget.show();
	_MyMovingWidget.raise();
}

void BusinessLogic::slot_showWifiWidget(QVariant var)
{
	QMap<QString, QString> _maps = var.value<QMap<QString, QString>>();
	QPoint p = _ParentWideget->mapToGlobal(QPoint(0, 0));
	_MyWiFiWidget.initPage(_maps);
	_MyWiFiWidget.move(p.x(), p.y());
	_MyWiFiWidget.show();
	_MyWiFiWidget.raise();
}

void BusinessLogic::slots_AutoRequest()
{
	if(_Time--)
	{
		qDebug() << "_Time:" << _Time;
		_ParCMD->ParsCmd(gVar->comparTestname(_TestName), A_RESULT);
	}
	else
	{
		reasultAutoTest();
	}
}
