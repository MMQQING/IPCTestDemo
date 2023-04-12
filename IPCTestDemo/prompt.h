#pragma once
#include <QTimer>
#include <QWidget>
#include "ParsingCmd.h"
#include "ui_prompt.h"

class prompt : public QWidget
{
	Q_OBJECT

public:
	prompt(QWidget *parent = nullptr);
	~prompt();
	void init();
	void setPCMD(ParsingCmd* cmd);
	void setTitle(QString, int, int);
private:
	void bindSinalSlot();
private slots:
	void slots_pass_button();
	void slots_false_button();
	void slots_close_window();
	void slots_timer_out(QString testname, int time);
private:
	Ui::prompt					ui;
	ParsingCmd*					_ParCMD;
	QString						_TestName;
};
