#pragma once

#include <QWidget>
#include "ui_IPCTestDemo.h"
#include "ParsingCmd.h"
#include "BusinessLogic.h"

class IPCTestDemo : public QWidget
{
	Q_OBJECT

public:
	IPCTestDemo(QWidget *parent = nullptr);
	~IPCTestDemo();
private:
	void bindSinalSlot();
	void initPage();
private:
	Ui::IPCTestDemoClass			ui;
	ParsingCmd						_ParCmd;
	BusinessLogic					_Business;
};
