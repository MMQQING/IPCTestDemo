#include "BigWindowClass.h"
#include <QFormLayout>
#include <QSplitter>
#include <QPainter>
#include <QScreen>

BigWindowClass::BigWindowClass(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
}

BigWindowClass::~BigWindowClass()
{}

void BigWindowClass::init()
{
	//设置主界面标题栏自绘
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	//setWindowState(Qt::WindowMaximized);
	//setWindowFlags(Qt::WindowStaysOnTopHint);
	QScreen *screen = QGuiApplication::primaryScreen();
	QRect mm = screen->availableGeometry();
	int screen_width = mm.width();
	int screen_height = mm.height();
	this->setFixedSize(screen_width*0.8, screen_height*0.8);
	/*布局样例*/
	QGridLayout *pLayout = new QGridLayout();//栅格布局
	pLayout->setContentsMargins(0, 70, 37, 0);
	QHBoxLayout *firstRow = new QHBoxLayout;//第一行放个水平布局器
	firstRow->addStretch();
	QPushButton *myButton = new QPushButton();
	myButton->setObjectName("closeBigButton");
	firstRow->addWidget(myButton);
	pLayout->addLayout(firstRow, 0, 0);
	QSplitter *splitterSecondRow = new QSplitter(Qt::Vertical);//第二行放个竖直分割器
	pLayout->addWidget(splitterSecondRow, 1, 0);
	ui.label->setLayout(pLayout);
	bool bl = connect(myButton, SIGNAL(clicked()), this, SLOT(slot_closeWindow()));
}

void BigWindowClass::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);

	if (m_image.size().width() <= 0)
		return;
	QImage img = m_image.scaled(ui.label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	//QPoint p5 = ui.label->mapToGlobal(QPoint(0, 0));   //按钮相对于桌面原点的绝对位置
	QPoint p5 = ui.label->mapToParent(QPoint(0, 0));   //按钮相对于父类窗口的绝对位置
	//ui.label->setPixmap(QPixmap::fromImage(img));
	painter.drawImage(p5, img);
}

void BigWindowClass::slot_closeWindow()
{
	this->close();
}

void BigWindowClass::SlotShowOneFrame(QImage img)
{
	m_image = img;
	this->update();
}

void BigWindowClass::slot_changge_index(int num)
{
	this->show();
}
