#pragma once
#include <QSpinBox>
#include "QTimer"
#include "cpc_icon.h"
class CPC_SpinBox :public QSpinBox, virtual public CPC_UIBase
{
	Q_OBJECT
public:
	CPC_SpinBox(QWidget *parent = nullptr);
	CPC_SpinBox(int Icon, QString text, QWidget *parent = nullptr);
	~CPC_SpinBox();

	void resetIconPoint(float xP = 0, float yp = 0);
	void setIconVisiable(bool visiable);
	//void setValue(int val);
private:
	void resizeEvent(QResizeEvent *event);
	
	void wheelEvent(QWheelEvent *event);
private:
	QTimer sendSignalTimer;
	int m_value;
signals:
	void valueChangedOwn(int);
public slots :
	void send_signal();
	void UpdatePadding(float w, float h);
	void keyPressEvent(QKeyEvent *event);
	void focusOutEvent(QFocusEvent *event);
};

