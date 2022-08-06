#include "BusinessLogic.h"
#include "GlobleVar.h"
BusinessLogic::BusinessLogic()
{
	init();
}

BusinessLogic::~BusinessLogic()
{
}

void BusinessLogic::init()
{
	_TimerRequest = new QTimer(this);
	bindSinalSlot();
}

void BusinessLogic::setPCMD(ParsingCmd* cmd)
{
	_ParCMD = cmd;
	_MyPromtWidget.setPCMD(cmd);
	_MyAutoPromtWidget.setPCMD(cmd);
}

void BusinessLogic::bindSinalSlot()
{
	bool bl = connect(this, SIGNAL(timeout(QString, int)), &_MyPromtWidget, SLOT(slots_timer_out(QString, int)));
	bl = connect(_TimerRequest, &QTimer::timeout, this, &BusinessLogic::slots_AutoRequest);
	bl = connect(this, SIGNAL(autotimeout(QString, int)), &_MyAutoPromtWidget, SLOT(slots_timer_out(QString, int)));
}

void BusinessLogic::slots_ManTest(QString testname, int time)
{
	_TestName = testname;
	int _timeout = time * 1000;
	_MyPromtWidget.setTitle(testname);
	_MyPromtWidget.show();
	_MyPromtWidget.raise();
	emit timeout(_TestName,_timeout);
}

void BusinessLogic::slots_AutoTest(QString testname, int time)
{
	_TestName = testname;
	int _timeout = time * 1000;
	_MyAutoPromtWidget.setTitle(testname);
	_MyAutoPromtWidget.show();
	_MyAutoPromtWidget.raise();
	emit autotimeout(_TestName,_timeout);
	_TimerRequest->start(1000);
	QTimer::singleShot(_timeout, [=]() {
		_TimerRequest->stop();
	});
}

void BusinessLogic::slots_AutoRequest()
{
	_ParCMD->ParsCmd(gVar->comparTestname(_TestName), A_RESULT);
}
