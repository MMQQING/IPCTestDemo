#include "autoprompt.h"
#include "GlobleVar.h"

autoprompt::autoprompt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
	ui.label->setAlignment(Qt::AlignCenter);
	ui.label->setStyleSheet("QLabel{font-size: 18px;}");
}

autoprompt::~autoprompt()
{}

void autoprompt::setPCMD(ParsingCmd* cmd)
{
	_ParCMD = cmd;
}

void autoprompt::setTitle(QString str, int w, int h)
{
	this->setFixedSize(w, h);
	ui.label->setText(gVar->TestProjectName(str));
}

void autoprompt::close_window()
{
	_Timer->stop();
	if (!this->isHidden()) {
		this->hide();
	}
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
	//QTimer::singleShot(time, this, SLOT(slots_close_window()));
	_Timer = new QTimer(this);
	_Timer->setInterval(time);
	_Timer->setSingleShot(true);
	connect(_Timer, SIGNAL(timeout()), SLOT(slots_close_window()));
	_Timer->start();
}
