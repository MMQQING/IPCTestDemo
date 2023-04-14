#pragma once

#include <QWidget>
#include "ui_IPCTestDemo.h"
#include "ParsingCmd.h"
#include "BusinessLogic.h"
#include "Mesjob.h"
#include <QThread>
#include "RTSP_Player.h"
#include "ButtonEdit.h"

class IPCTestDemo : public QWidget
{
	Q_OBJECT

public:
	explicit IPCTestDemo(int index, QWidget *parent = nullptr);
	~IPCTestDemo();
private:
	void bindSinalSlot();
	void initPage();
	void initParam();
	void initStyle();

	void PlayStart();
	void PlayStop();
	//void PlayProcess();

	void getDeviceInfo();
	void comparInfo(QMap<QString,QString> map);

	QString errCode(QString str);
	QString toTestName(QString str);
	void clearMesInfo();
	void clearMesInfoNoCMEI();
	void clearAllInfo();
protected:
	//void keyPressEvent(QKeyEvent *);//手持扫码
	void keyReleaseEvent(QKeyEvent *event) override;
	//bool eventFilter(QObject *watched, QEvent *event);
	void mouseDoubleClickEvent(QMouseEvent* e);// 重新实现鼠标双击事件
	void paintEvent(QPaintEvent *e);
	void closeEvent(QCloseEvent *e);

private slots:
	void initTestItems(QVector<QString> vec);
	void upDataToPage_20(bool, QString, QVariant);
	void upDataToPage_30(bool, QString);
	void upDataToPage_40(bool, QString, QVariant);
	void upDataFromIPC(QVariant var, QString str);

	void SlotGetOneFrame(QImage img);
	void SlotOpenUrlResult(int result);
	void SlotComparInfo();
	void SlotWriteInfo();
	void SlotDefault();

	void switch_rtsp();
	void on_pushButton_2_clicked();
	void slots_testButton();
	void slots_upButton();

	void slots_isConnectIPC(bool, QString);

	void slots_ManTest(QString testname, int revalue);
	void slots_AutoTest(QString testname, int revalue);
	void slots_NextTest();
	void slots_upReturn(QString testname, int revalue);

	void deleteLater();
signals:
	void SigPlayStart();
	void SigPlayStop();
	void sigNextTest();
	void sigChanggeIndex(int);
	void sigOneImage(int, QImage);
	void writeInfo(QVariant);

private:
	int								_index;
	Ui::IPCTestDemo*				ui;
	ParsingCmd						_ParCmd;
	BusinessLogic*					_Business;
	std::shared_ptr<Mesjob>			_Mesjob;

	bool					blRtsp;			//false 关闭  true 开启

	QString					m_playUrl;
	MyFFmpeg*				m_ffmpeg = nullptr;
	RTSP_Player*			m_player = nullptr;
	QImage					m_image;
	QThread*				m_playThread = nullptr;
	bool					IsConnectIPC;
	QMap<QString, QLabel*>	_map;
	QMap<QString, QString>	_mapResult;
	QList<QString>			_testlist;
	QVector<QString>		_testvec;
	ButtonEdit*				edit_CMEI;
	ButtonEdit*				edit_PCBASN;
	//ButtonEdit*				edit_AppVersion;
	QPushButton*			startTestBtn;
	MLabel*					show_info_label;
};
