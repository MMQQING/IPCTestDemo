#include "autoprompt.h"
#include "GlobleVar.h"

autoprompt::autoprompt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

autoprompt::~autoprompt()
{}

void autoprompt::setPCMD(ParsingCmd* cmd)
{
	_ParCMD = cmd;
}

void autoprompt::setTitle(QString str)
{
	ui.label->setText(str);
}

void autoprompt::slots_close_window()
{
	if (!this->isHidden()) {
		this->hide();
	}
	//todo.. ³¬Ê±GUI
	_ParCMD->ParsCmd(gVar->comparTestname(_TestName), A_RESULT, Result_Timeout);
}

void autoprompt::slots_timer_out(QString testname, int time)
{
	_TestName = testname;
	QTimer::singleShot(time, this, SLOT(slots_close_window()));
}
