#include "MainWidgets.h"
#include <QPainter>
#include <QDateTime>
#include "FSVersion.h"
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
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdata()));
	timer->start(1000);
	initStyle();
}

void MainWidgets::bindSinalSlot()
{
	bool bl = connect(this, SIGNAL(showPicture(QImage)), bigWindow, SLOT(SlotShowOneFrame(QImage)));
}

void MainWidgets::initStyle()
{
	ui->time_label->setAlignment(Qt::AlignVCenter);
	showVersion(FS_FILE_VERSION);
	QFile styleFile(":/IPCTestDemo/Resources/MainWidget.qss");
	if (!styleFile.open(QIODevice::ReadOnly))
	{
		return;
	}
	this->setStyleSheet(styleFile.readAll());
	styleFile.close();
}

void MainWidgets::showVersion(int yy, int mm, int dd, int ver)
{
	ui->version_label->setText(QString::fromLocal8Bit("软件版本：")+
		QString::number(yy)+"."+
		QString::number(mm) + "."+
		QString::number(dd) + "." + 
		QString::number(ver) );
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

void MainWidgets::timerUpdata(void)
{
	//QFont font("Microsoft YaHei", 20, 50);
	//ui->time_label->setFont(font);
	QDateTime time = QDateTime::currentDateTime();
	QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
	this->ui->time_label->setText(str);
}

void MainWidgets::slot_addItem(QVector<QString> vec)
{
	int m = ui->main_widget->width();
	QHBoxLayout *layout = new QHBoxLayout;
	layout->setContentsMargins(20, 14, 20, 0);
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
