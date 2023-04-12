#pragma once

#include <QWidget>
#include "ui_WifiTestWidget.h"
#include "ParsingCmd.h"
#include <QProcess>
#include <QTextCodec>
#include <QTimer>

class WifiTestWidget : public QWidget
{
	Q_OBJECT

public:
	WifiTestWidget(QWidget *parent = nullptr);
	~WifiTestWidget();
	void init();
	void setPCMD(ParsingCmd* cmd);
	void initPage(QMap<QString, QString> _map);
public slots:
	void slot_connect_wifi();
	void slot_getInfo_wifi();
	//更新WiFi信息
	void slot_upWiFiInfo(QVariant);
	void slot_iper3_down();
	void slot_iper3_up();
	void slot_readyRead();
	void false_button();
	void ok_button();
private:
	void bindSinalSlot();
	float getSpeed(QString);
	void upToWidget(float, float, float);

signals:
	void wifi_signal(QString, int);
private:
	Ui::WifiTestWidgetClass ui;
	ParsingCmd*					_ParCMD;
//	QProcess *					process;
	QTimer*						_timer;
	float						_minBandwidth;
	float						_maxBandwidth;
	float						_averageBandwidth;
};
