#pragma once

#include <QWidget>
#include "ui_IPCTestDemo.h"
#include "ParsingCmd.h"
#include "BusinessLogic.h"
#include "Mesjob.h"
#include <QThread>
#include "RTSP_Player.h"

class IPCTestDemo : public QWidget
{
	Q_OBJECT

public:
	explicit IPCTestDemo(QWidget *parent = nullptr);
	~IPCTestDemo();
private:
	void bindSinalSlot();
	void initPage();
	void initParam();

	void PlayStart();
	void PlayStop();
	void PlayProcess();

	void getDeviceInfo();
	void comparInfo(QMap<QString,QString> map);

	QString errCode(QString str);
	QString toTestName(QString str);
protected:
	void keyPressEvent(QKeyEvent *);// ÷≥÷…®¬Î
	//bool eventFilter(QObject *watched, QEvent *event);

	void paintEvent(QPaintEvent *e);
	void closeEvent(QCloseEvent *e);

private slots:
	void initTestItems(QVector<QString> vec);
	void upDataToPage_20(bool bl, QString str);
	void upDataToPage_40(bool bl, QString str);
	void upDataFromIPC(QVariant var, QString str);

	void SlotGetOneFrame(QImage img);
	void SlotOpenUrlResult(int result);
	void SlotComparInfo();
	void SlotWriteInfo();
	void SlotDefault();

	void on_pushButton_clicked();
	void on_pushButton_2_clicked();
	void slots_testButton();
	void slots_upButton();

	void slots_isConnectIPC(bool);

	void slots_ManTest(QString testname, int revalue);
	void slots_AutoTest(QString testname, int revalue);
	void slots_NextTest();
	void slots_upReturn(QString testname, int revalue);

signals:
	void SigPlayStart();
	void sigNextTest();
private:
	Ui::IPCTestDemo*				ui;
	ParsingCmd						_ParCmd;
	BusinessLogic					_Business;
	std::shared_ptr<Mesjob>			_Mesjob;

	QString      m_playUrl;
	MyFFmpeg    *m_ffmpeg = nullptr;
	RTSP_Player *m_player = nullptr;
	QImage       m_image;
	QThread     *m_playThread = nullptr;
	bool		IsConnectIPC;
	QMap<QString, QLabel*> _map;
	QMap<QString, QString> _mapResult;
	QList<QString> _testlist;
	QVector<QString> _testvec;
};
