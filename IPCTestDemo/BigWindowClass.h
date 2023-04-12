#pragma once

#include <QWidget>
#include "ui_BigWindowClass.h"
#include <QPushButton>

class BigWindowClass : public QWidget
{
	Q_OBJECT

public:
	BigWindowClass(QWidget *parent = nullptr);
	~BigWindowClass();
	void init();
protected:
	void paintEvent(QPaintEvent *e);
public slots:
	void slot_closeWindow();
	void SlotShowOneFrame(QImage img);
	void slot_changge_index(int);
private:
	Ui::BigWindowClassClass ui;
	QImage				m_image;
};
