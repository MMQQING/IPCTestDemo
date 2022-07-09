#include "IPCTestDemo.h"
#include "GlobleVar.h"
IPCTestDemo::IPCTestDemo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initPage();
	bindSinalSlot();
	_ParCmd.ParsCmd(CMD_DEVIINFO, A_TEST);
}

IPCTestDemo::~IPCTestDemo()
{

}

void IPCTestDemo::bindSinalSlot()
{
	bool bl = connect(&_ParCmd, SIGNAL(manTimeout(QString,int)), &_Business, SLOT(slots_ManTest(QString,int)));
	bl = connect(&_ParCmd, SIGNAL(autoTimeout(QString, int)), &_Business, SLOT(slots_AutoTest(QString, int)));
}

void IPCTestDemo::initPage()
{
	//设置主界面标题栏自绘
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	setWindowState(Qt::WindowMaximized);
}
