#include "AddTestItemDialog.h"
#include "ui_AddTestItemDialog.h"
#include "GlobleVar.h"
AddTestItemDialog::AddTestItemDialog(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::AddTestItemDialog)
{
	ui->setupUi(this);
	initFont();
	init();
	bindSinalSlot();
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
	m_mousePressed = false;
//	_ipcWindow = std::make_shared<IPCTestDemo>();
	_mainWindow = std::make_shared<MainWidgets>();
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	//设置主界面标题栏自绘
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	this->setFixedSize(832, 262);
	listA = new ProjectListWidget;
	listB = new ProjectListWidget;
// 	buttonAToB = new QPushButton("->");
// 	buttonBToA = new QPushButton("<-");
	buttonUp = new QPushButton();
	buttonDown = new QPushButton();
	buttonOk = new QPushButton();
	buttonSave = new QPushButton();
	buttonUp->setFixedSize(70, 30);
	buttonDown->setFixedSize(70, 30);
	buttonOk->setFixedSize(70, 30);
	buttonSave->setFixedSize(70, 30);

	//listA->addItem(QString::fromLocal8Bit("写运营商信息"));
	for each (auto var in ptr->m_testListA)
	{
		listA->addItem(QString::fromStdString(var));
	}
	for each (auto var in ptr->m_testListB)
	{
		listB->addItem(QString::fromStdString(var));
	}

// 	buttonAToB->setText("->");
// 	buttonBToA->setText("<-");
	buttonUp->setFont(font);
	buttonDown->setFont(font);
	buttonUp->setText(QChar(0xf062));
	buttonDown->setText(QChar(0xf063));
	buttonOk->setText(QString::fromLocal8Bit("设置"));
	buttonSave->setText(QString::fromLocal8Bit("保存"));

	QVBoxLayout *buttonLayout = new QVBoxLayout;
	buttonLayout->setContentsMargins(0, 40, 0, 40);
	buttonLayout->setSpacing(37);
// 	buttonLayout->addWidget(buttonAToB);
// 	buttonLayout->addWidget(buttonBToA);
	buttonLayout->addWidget(buttonUp);
	buttonLayout->addWidget(buttonDown);
	buttonLayout->addWidget(buttonOk);
	buttonLayout->addWidget(buttonSave);

	QHBoxLayout *hboxLayout = new QHBoxLayout;
	hboxLayout->setContentsMargins(16, 9, 16, 9);
	hboxLayout->setSpacing(45);
	hboxLayout->addWidget(listA);
	hboxLayout->addLayout(buttonLayout);
	hboxLayout->addWidget(listB);
	setLayout(hboxLayout);

	setFixedHeight(sizeHint().height());
	initStyle();
}

void AddTestItemDialog::bindSinalSlot()
{
	// 	bool bl = connect(buttonAToB, SIGNAL(clicked()), this, SLOT(AToB_clicked()));
// 	connect(buttonBToA, SIGNAL(clicked()), this, SLOT(BToA_clicked()));
	bool bl = connect(buttonUp, SIGNAL(clicked()), this, SLOT(Up_clicked()));
	bl = connect(buttonDown, SIGNAL(clicked()), this, SLOT(Down_clicked()));
	bl = connect(buttonOk, SIGNAL(clicked()), this, SLOT(Set_clicked()));
	bl = connect(buttonSave, SIGNAL(clicked()), this, SLOT(Save_clicked()));
	//bl = connect(this, SIGNAL(vector_test(QVector<QString>)), _ipcWindow.get(), SLOT(initTestItems(QVector<QString>)));
	bl = connect(this, SIGNAL(vector_test(QVector<QString>)), _mainWindow.get(), SLOT(slot_addItem(QVector<QString>)));
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

void AddTestItemDialog::initStyle()
{
	QFile styleFile(":/IPCTestDemo/Resources/addItem.qss");
	if (!styleFile.open(QIODevice::ReadOnly))
	{
		//CPCLOG_WARNING << "open style file error, style file path:" + styleFile.fileName().toStdString();
		return;
	}
	this->setStyleSheet(styleFile.readAll());
	//qApp->setStyleSheet(styleFile.readAll());
	styleFile.close();
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
		_mainWindow = std::make_shared<MainWidgets>();
	_mainWindow->show();
	_mainWindow->raise();	

	//todo. ..发送vector给下个界面
	emit vector_test(vector);
}

void AddTestItemDialog::Save_clicked()
{
	std::shared_ptr<GlobleVar> ptr = GlobleVar::GetInstance();
	ptr->m_testListA.clear();
	ptr->m_testListB.clear();
	for (int i = 0; i < listA->count(); i++)
	{
		QString itemText = listA->item(i)->text();
		ptr->m_testListA.push_back(itemText.toStdString());
	}
	for (int i = 0; i < listB->count(); i++)
	{
		QString itemText = listB->item(i)->text();
		ptr->m_testListB.push_back(itemText.toStdString());
	}
	ptr->savelist(ptr->m_strXmlListPath.toStdString());
}

void AddTestItemDialog::mouseMoveEvent(QMouseEvent *e)
{
	if (m_mousePressed && (e->buttons() && Qt::LeftButton) /*&& !m_bmax*/)
	{
		// delta 相对偏移量, 
		QPoint delta = e->globalPos() - m_poStartPosition;
		move(m_poFramePosition + delta);
		e->accept();
	}
}

void AddTestItemDialog::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		m_mousePressed = true;
		m_poStartPosition = e->globalPos();
		m_poFramePosition = frameGeometry().topLeft();
		e->accept();
	}
}

void AddTestItemDialog::mouseReleaseEvent(QMouseEvent *e)
{
	m_mousePressed = false;
}
