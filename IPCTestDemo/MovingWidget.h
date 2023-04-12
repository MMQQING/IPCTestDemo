#pragma once

#include <QWidget>
#include "ParsingCmd.h"
#include "ui_MovingWidget.h"

class MovingWidget : public QWidget
{
	Q_OBJECT

public:
	MovingWidget(QWidget *parent = nullptr);
	~MovingWidget();
	void init();
	void setPCMD(ParsingCmd* cmd);
public slots:
	void up_button();
	void down_button();
	void left_button();
	void right_button();
	void false_button();
	void ok_button();
signals:
	void ptz_signal(QString, int);
private:
	void bindSinalSlot();
	void turnZero();

private:
	Ui::MovingWidgetClass ui;
	ParsingCmd*					_ParCMD;
};
