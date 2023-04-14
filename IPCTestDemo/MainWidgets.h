#pragma once

#include <QWidget>
#include "ui_MainWidgets.h"
#include "IPCTestDemo.h"
#include "BigWindowClass.h"

class MainWidgets : public QWidget
{
	Q_OBJECT

public:
	explicit MainWidgets(QWidget *parent = nullptr);
	~MainWidgets();
	
private:
	void initPage();
	void bindSinalSlot();
	void initStyle();
	void showVersion(int,int,int,int);
signals:
	void vector_test(QVector<QString> vec);
	void showPicture(QImage);
public slots:
	void slot_addItem(QVector<QString> vec);
	void SlotGetOneFrame(int index, QImage img);
	void slot_changge_index(int);
	void timerUpdata(void);
private:
	Ui::MainWidgets*	ui;
	//QLabel *			paint_lable;
	QImage				m_image;
	int					_index;
	BigWindowClass*		bigWindow;
};
