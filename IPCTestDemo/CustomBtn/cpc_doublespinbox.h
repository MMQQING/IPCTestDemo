#pragma once
#include <QDoubleSpinBox>
#include "QTimer"
#include "cpc_icon.h"
class cpc_doublespinbox :public QDoubleSpinBox, virtual public CPC_UIBase
{
	Q_OBJECT
public:
	cpc_doublespinbox(QWidget *parent = nullptr);
	cpc_doublespinbox(int Icon, QString text, QWidget *parent = nullptr);
	~cpc_doublespinbox();

	void resetIconPoint(float xP = 0, float yp = 0);
	void setIconVisiable(bool visiable);
private:
	void resizeEvent(QResizeEvent *event);
	void wheelEvent(QWheelEvent *event);
private:
	QTimer sendSignalTimer;
	double m_value;
signals:
	void valueChangedOwn(double);

public slots :
	void send_signal();
	void valueChange(double d);
	void keyPressEvent(QKeyEvent *event);
	void focusOutEvent(QFocusEvent *event);
	void UpdatePadding(float w, float h);
};

