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
	// ��������
	int m_fontSize = 12;
	QString m_fontFamily = "FontAwesome";
	font.setFamily(m_fontFamily);
	font.setPointSize(m_fontSize);
}

void AddTestItemDialog::init()
{
	//����������������Ի�
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	listA = new ProjectListWidget;
	listB = new ProjectListWidget;
// 	buttonAToB = new QPushButton("->");
// 	buttonBToA = new QPushButton("<-");
	buttonUp = new QPushButton();
	buttonDown = new QPushButton();
	buttonOk = new QPushButton();

	listA->addItem(QString::fromLocal8Bit("д��Ӫ����Ϣ"));
	listA->addItem(QString::fromLocal8Bit("�ָ���������"));
	listA->addItem(QString::fromLocal8Bit("���ذ׹��"));
	listA->addItem(QString::fromLocal8Bit("���غ����"));
	listA->addItem(QString::fromLocal8Bit("����IRCUT"));
	listA->addItem(QString::fromLocal8Bit("��������LED��"));
	listA->addItem(QString::fromLocal8Bit("����������"));

	listB->addItem(QString::fromLocal8Bit("������˷�"));
	listB->addItem(QString::fromLocal8Bit("���Ժ���ҹ��"));
	listB->addItem(QString::fromLocal8Bit("����ȫ��ҹ��"));
	listB->addItem(QString::fromLocal8Bit("����Rest����"));
	listB->addItem(QString::fromLocal8Bit("����TF��"));
	listB->addItem(QString::fromLocal8Bit("���Թ���"));


// 	buttonAToB->setText("->");
// 	buttonBToA->setText("<-");
	buttonUp->setFont(font);
	buttonDown->setFont(font);
	buttonUp->setText(QChar(0xf062));
	buttonDown->setText(QChar(0xf063));
	buttonOk->setText(QString::fromLocal8Bit("����"));

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
		_mainWindow = std::make_shared<IPCTestDemo>();
	_mainWindow->show();
	_mainWindow->raise();
	//todo. ..����vector���¸�����
}
