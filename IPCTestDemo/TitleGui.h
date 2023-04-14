#pragma once

#include <QWidget>
#include "ui_TitleGui.h"
#include "QRect"
#include "QMouseEvent"

#define WIN_WIDTH  1920
#define WIN_HEIGHT  1032

#define MAX_ICON 0xf096
#define MID_ICON 0xf079
#define MIN_ICON 0xf068
#define CLO_ICON 0xf00d

enum layoutChoose {
	leftLayout = 0,
	rightLayout = 1
};
class TitleGui : public QWidget
{
	Q_OBJECT
private:
	void initStyle();
public:
	TitleGui(QWidget *parent = Q_NULLPTR);
	~TitleGui();
	void InitMenu();
	void InitConnect();
	void on_btnMenu_Close_clicked();
	void on_btnMenu_Max_clicked();
	void on_btnMenu_Min_clicked();
	QRect overspreadScreen();

	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

	void showEvent(QShowEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event); 

	//void addWidget(QWidget* widget, layoutChoose lr);
private:
	Ui::TitleGui ui;

	bool										m_mousePressed;
	QPoint										m_mousePoint;
	bool										m_bmax;
	QRect										m_location;
};
