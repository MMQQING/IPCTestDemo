#include "MainWidgets.h"
#include <QPainter>

MainWidgets::MainWidgets(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::MainWidgets)
{
	ui->setupUi(this);
	initPage();
	bindSinalSlot();
}

MainWidgets::~MainWidgets()
{}



void MainWidgets::initPage()
{
	//设置主界面标题栏自绘
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	setWindowState(Qt::WindowMaximized);
	bigWindow = new BigWindowClass(this);
	_index = -1;
	initStyle();
}

void MainWidgets::bindSinalSlot()
{
	bool bl = connect(this, SIGNAL(showPicture(QImage)), bigWindow, SLOT(SlotShowOneFrame(QImage)));
}

void MainWidgets::initStyle()
{
	QFile styleFile(":/IPCTestDemo/Resources/MainWidget.qss");
	if (!styleFile.open(QIODevice::ReadOnly))
	{
		return;
	}
	this->setStyleSheet(styleFile.readAll());
	styleFile.close();
}

void MainWidgets::SlotGetOneFrame(int index, QImage img)
{
	if(index == _index){
		emit showPicture(img);
	}
}

void MainWidgets::slot_changge_index(int num)
{
	_index = num;
	bigWindow->show();
}

void MainWidgets::slot_addItem(QVector<QString> vec)
{
	int m = ui->main_widget->width();
	QHBoxLayout *layout = new QHBoxLayout;
	layout->setContentsMargins(20, 14, 20, 14);
	for (int i = 0; i < 4; i++) {
		IPCTestDemo* ipcPage = new IPCTestDemo(i);
		bool bl = connect(this, SIGNAL(vector_test(QVector<QString>)), ipcPage, SLOT(initTestItems(QVector<QString>)));
		bl = connect(ipcPage, SIGNAL(sigChanggeIndex(int)), this, SLOT(slot_changge_index(int)));
		bl = connect(ipcPage, SIGNAL(sigOneImage(int, QImage)), this, SLOT(SlotGetOneFrame(int, QImage)));
		layout->addWidget(ipcPage);
	}
	ui->main_widget->setLayout(layout);
	emit vector_test(vec);
}
