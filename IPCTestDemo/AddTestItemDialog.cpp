#include "AddTestItemDialog.h"
#include "ui_AddTestItemDialog.h"
AddTestItemDialog::AddTestItemDialog(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::AddTestItemDialog)
{
	ui->setupUi(this);
	initFont();
	init();
}

AddTestItemDialog::~AddTestItemDialog()
{}
void AddTestItemDialog::initFont()
{
	// 创建字体
	int m_fontSize = 12;
	QString m_fontFamily = "FontAwesome";
	font.setFamily(m_fontFamily);
	font.setPointSize(m_fontSize);
}

void AddTestItemDialog::init()
{
	//设置主界面标题栏自绘
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	listA = new ProjectListWidget;
	listB = new ProjectListWidget;
// 	buttonAToB = new QPushButton("->");
// 	buttonBToA = new QPushButton("<-");
	buttonUp = new QPushButton();
	buttonDown = new QPushButton();
	buttonOk = new QPushButton();

	listA->addItem(QString::fromLocal8Bit("写运营商信息"));
	listA->addItem(QString::fromLocal8Bit("恢复出厂设置"));
	listA->addItem(QString::fromLocal8Bit("开关白光灯"));
	listA->addItem(QString::fromLocal8Bit("开关红外灯"));
	listA->addItem(QString::fromLocal8Bit("开光IRCUT"));
	listA->addItem(QString::fromLocal8Bit("开光网络LED灯"));
	listA->addItem(QString::fromLocal8Bit("测试扬声器"));

	listB->addItem(QString::fromLocal8Bit("测试麦克风"));
	listB->addItem(QString::fromLocal8Bit("测试红外夜视"));
	listB->addItem(QString::fromLocal8Bit("测试全彩夜视"));
	listB->addItem(QString::fromLocal8Bit("测试Rest按键"));
	listB->addItem(QString::fromLocal8Bit("测试TF卡"));
	listB->addItem(QString::fromLocal8Bit("测试光敏"));


// 	buttonAToB->setText("->");
// 	buttonBToA->setText("<-");
	buttonUp->setFont(font);
	buttonDown->setFont(font);
	buttonUp->setText(QChar(0xf062));
	buttonDown->setText(QChar(0xf063));
	buttonOk->setText(QString::fromLocal8Bit("设置"));

	QVBoxLayout *buttonLayout = new QVBoxLayout;
// 	buttonLayout->addWidget(buttonAToB);
// 	buttonLayout->addWidget(buttonBToA);
	buttonLayout->addWidget(buttonUp);
	buttonLayout->addWidget(buttonDown);
	buttonLayout->addWidget(buttonOk);

	QHBoxLayout *hboxLayout = new QHBoxLayout;
	hboxLayout->addWidget(listA);
	hboxLayout->addLayout(buttonLayout);
	hboxLayout->addWidget(listB);
	setLayout(hboxLayout);

	setFixedHeight(sizeHint().height());

// 	bool bl = connect(buttonAToB, SIGNAL(clicked()), this, SLOT(AToB_clicked()));
// 	connect(buttonBToA, SIGNAL(clicked()), this, SLOT(BToA_clicked()));
	bool bl = connect(buttonUp, SIGNAL(clicked()), this, SLOT(Up_clicked()));
	connect(buttonDown, SIGNAL(clicked()), this, SLOT(Down_clicked()));
	connect(buttonOk, SIGNAL(clicked()), this, SLOT(Set_clicked()));
}

// void AddTestItemDialog::AToB_clicked()
// {
// 	moveCurrentItem(listA, listB);
// }
// 
// void AddTestItemDialog::BToA_clicked()
// {
// 	moveCurrentItem(listB, listA);
// }

void AddTestItemDialog::moveCurrentItem(QListWidget *source,
	QListWidget *target)
{
	if (source->currentItem()) {
		QListWidgetItem *newItem = source->currentItem()->clone();
		target->addItem(newItem);
		target->setCurrentItem(newItem);
		delete source->currentItem();
	}
}

void AddTestItemDialog::Up_clicked()
{
	QListWidgetItem *selitem = listA->currentItem(); //获取用户当前选中项
	if (selitem != NULL) //判断非空，否则点击会出错
	{
		int currRow = listA->currentRow();//获取当前行号
		QString selstr = selitem->text(); //获取当前行内容
		if (currRow > 0) //限制（上移至第0行终止）
		{
			delete listA->takeItem(currRow);//删除当前行
			listA->insertItem(currRow - 1, selstr);//在上一行插入新行，内容为之前选中内容
		}
		listA->setCurrentRow(currRow - 1);//将上移后的新行选中，实现连续上移
	}
}

void AddTestItemDialog::Down_clicked()
{
	QListWidgetItem *selitem = listA->currentItem();
	if (selitem != NULL)
	{
		int currRow = listA->currentRow();
		QString selstr = selitem->text();
		int rowAll = listA->count();//获取当前list总行数
		if ((rowAll - currRow) > 0)//限制（下移至最后一行终止）
		{
			delete  listA->takeItem(currRow);
			listA->insertItem(currRow + 1, selstr);
		}
		listA->setCurrentRow(currRow + 1);
	}
}

void AddTestItemDialog::Set_clicked()
{
	QVector<QString> vector;
	for (int i = 0; i < listA->count(); i++)
	{
		QString itemText = listA->item(i)->text();
		vector.push_back(itemText);
	}
	this->hide();
	if (nullptr == _mainWindow)
		_mainWindow = std::make_shared<IPCTestDemo>();
	_mainWindow->show();
	_mainWindow->raise();
	//todo. ..发送vector给下个界面
}
