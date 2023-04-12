#pragma once

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
//#include "IPCTestDemo.h"
#include "projectlistwidget.h"
#include "MainWidgets.h"

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
	void bindSinalSlot();
	void moveCurrentItem(QListWidget *source, QListWidget *target);
	void initStyle();
signals:
	void vector_test(QVector<QString> vec);
public slots:
// 	void AToB_clicked();
// 	void BToA_clicked();
	void Up_clicked();
	void Down_clicked();
	void Set_clicked();
	void Save_clicked();
protected:
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
private:
	Ui::AddTestItemDialog *ui;
//	std::shared_ptr<IPCTestDemo> _ipcWindow;
	std::shared_ptr<MainWidgets> _mainWindow;
	QFont font;
	bool   m_mousePressed;
	QPoint m_poStartPosition;	// 拖动开始前的鼠标位置
	QPoint m_poFramePosition;	// 窗体的原始位置
private:
	QListWidget *listA;
	QListWidget *listB;
// 	QPushButton *buttonAToB;
// 	QPushButton *buttonBToA;
	QPushButton *buttonUp;
	QPushButton *buttonDown;

	QPushButton *buttonOk;
	QPushButton *buttonSave;
};
