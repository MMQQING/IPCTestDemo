#include "MovingWidget.h"
#include "GlobleVar.h"

MovingWidget::MovingWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
}

MovingWidget::~MovingWidget()
{}

void MovingWidget::init()
{
	bindSinalSlot();
	this->setWindowFlags(Qt::WindowStaysOnTopHint);
}

void MovingWidget::setPCMD(ParsingCmd* cmd)
{
	_ParCMD = cmd;
}

void MovingWidget::bindSinalSlot()
{
	bool bl = connect(ui.up_pushButton, SIGNAL(clicked()), this, SLOT(up_button()));
	bl = connect(ui.down_pushButton, SIGNAL(clicked()), this, SLOT(down_button()));
	bl = connect(ui.left_pushButton, SIGNAL(clicked()), this, SLOT(left_button()));
	bl = connect(ui.right_pushButton, SIGNAL(clicked()), this, SLOT(right_button()));
	bl = connect(ui.ok_pushButton, SIGNAL(clicked()), this, SLOT(ok_button()));
	bl = connect(ui.false_pushButton, SIGNAL(clicked()), this, SLOT(false_button()));
}

void MovingWidget::turnZero()
{
	_ParCMD->ParsCmd(ptz_zero, A_CONTROL);
}

void MovingWidget::up_button()
{
	_ParCMD->ParsCmd(ptz_up, A_CONTROL);
}

void MovingWidget::down_button()
{
	_ParCMD->ParsCmd(ptz_down, A_CONTROL);
}

void MovingWidget::left_button()
{
	_ParCMD->ParsCmd(ptz_left, A_CONTROL);
}

void MovingWidget::right_button()
{
	_ParCMD->ParsCmd(ptz_right, A_CONTROL);
}

void MovingWidget::false_button()
{
	if (!this->isHidden()) {
		this->hide();
	}
	_ParCMD->ParsCmd(ptz_zero, A_CONTROL);
	emit ptz_signal("ptztest", Result_False);
}

void MovingWidget::ok_button()
{
	if (!this->isHidden()) {
		this->hide();
	}
	_ParCMD->ParsCmd(ptz_zero, A_CONTROL);
	emit ptz_signal("ptztest", Result_Pass);
}
