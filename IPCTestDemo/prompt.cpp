#include "prompt.h"
#include "GlobleVar.h"

prompt::prompt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
}

prompt::~prompt()
{

}

void prompt::init()
{
	bindSinalSlot();
}

void prompt::setPCMD(ParsingCmd* cmd)
{
	_ParCMD = cmd;
}

void prompt::setTitle(QString str)
{
	ui.label->setText(str);
}

void prompt::bindSinalSlot()
{
	bool bl = connect(ui.PassButton, SIGNAL(clicked()), this, SLOT(slots_pass_button()));
	bl = connect(ui.FalseButton, SIGNAL(clicked()), this, SLOT(slots_false_button()));
}

void prompt::slots_pass_button()
{
	if (!this->isHidden()) {
		this->hide();
	}
	//todo...Í¨¹ýGUI
	_ParCMD->ParsCmd(gVar->comparTestname(_TestName), A_RESULT, Result_Pass);
}

void prompt::slots_false_button()
{
	if (!this->isHidden()) {
		this->hide();
	}
	//todo...Ê§°ÜGUI
	_ParCMD->ParsCmd(gVar->comparTestname(_TestName), A_RESULT, Result_False);
}

void prompt::slots_close_window()
{
	if(!this->isHidden()){
		this->hide();
	}
	//todo.. ³¬Ê±GUI
	_ParCMD->ParsCmd(gVar->comparTestname(_TestName), A_RESULT, Result_Timeout);
}

void prompt::slots_timer_out(QString testname, int time)
{
	_TestName = testname;
	//QTimer::singleShot(time, this, SLOT(slots_close_window()));
}
