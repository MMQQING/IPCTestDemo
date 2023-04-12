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
	// ��������
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
	//����������������Ի�
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

	//listA->addItem(QString::fromLocal8Bit("д��Ӫ����Ϣ"));
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
	buttonOk->setText(QString::fromLocal8Bit("����"));
	buttonSave->setText(QString::fromLocal8Bit("����"));

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
	QListWidgetItem *selitem = listA->currentItem(); //��ȡ�û���ǰѡ����
	if (selitem != NULL) //�жϷǿգ������������
	{
		int currRow = listA->currentRow();//��ȡ��ǰ�к�
		QString selstr = selitem->text(); //��ȡ��ǰ������
		if (currRow > 0) //���ƣ���������0����ֹ��
		{
			delete listA->takeItem(currRow);//ɾ����ǰ��
			listA->insertItem(currRow - 1, selstr);//����һ�в������У�����Ϊ֮ǰѡ������
		}
		listA->setCurrentRow(currRow - 1);//�����ƺ������ѡ�У�ʵ����������
	}
}

void AddTestItemDialog::Down_clicked()
{
	QListWidgetItem *selitem = listA->currentItem();
	if (selitem != NULL)
	{
		int currRow = listA->currentRow();
		QString selstr = selitem->text();
		int rowAll = listA->count();//��ȡ��ǰlist������
		if ((rowAll - currRow) > 0)//���ƣ����������һ����ֹ��
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

	//todo. ..����vector���¸�����
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
		// delta ���ƫ����, 
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
