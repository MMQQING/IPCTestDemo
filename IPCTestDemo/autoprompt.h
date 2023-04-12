#pragma once
#include <QTimer>
#include <QWidget>
#include "ui_autoprompt.h"
#include "ParsingCmd.h"

class autoprompt : public QWidget
{
	Q_OBJECT

public:
	autoprompt(QWidget *parent = nullptr);
	~autoprompt();
	void setPCMD(ParsingCmd* cmd);
	void setTitle(QString, int, int);
	void close_window();
private slots:
	void slots_close_window();
	void slots_timer_out(QString testname, int time);

private:
	Ui::autoprompt				ui;
	ParsingCmd*					_ParCMD;
	QString						_TestName;
	QTimer*						_Timer;
};
