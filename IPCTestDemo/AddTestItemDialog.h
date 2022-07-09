#pragma once

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "IPCTestDemo.h"
#include "projectlistwidget.h"

namespace Ui {
	class AddTestItemDialog;
}
class AddTestItemDialog : public QDialog
{
	Q_OBJECT

public:
	AddTestItemDialog(QWidget *parent = nullptr);
	~AddTestItemDialog();

private:
	void initFont();
	void init();
	void moveCurrentItem(QListWidget *source, QListWidget *target);
public slots:
// 	void AToB_clicked();
// 	void BToA_clicked();
	void Up_clicked();
	void Down_clicked();
	void Set_clicked();
private:
	Ui::AddTestItemDialog *ui;
	std::shared_ptr<IPCTestDemo> _mainWindow;
	QFont font;
private:
	QListWidget *listA;
	QListWidget *listB;
// 	QPushButton *buttonAToB;
// 	QPushButton *buttonBToA;
	QPushButton *buttonUp;
	QPushButton *buttonDown;

	QPushButton *buttonOk;
};
