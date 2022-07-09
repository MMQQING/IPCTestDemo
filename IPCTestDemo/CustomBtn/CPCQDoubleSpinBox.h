#pragma once

#include <QWidget>
#include <QDoubleSpinBox>

class CPCQDoubleSpinBox : public QDoubleSpinBox
{
	Q_OBJECT

public:
	CPCQDoubleSpinBox(QWidget *parent);
	~CPCQDoubleSpinBox();

Q_SIGNALS:
	void valueChangedOwn(double);

private:
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void focusOutEvent(QFocusEvent *event);
	virtual void wheelEvent(QWheelEvent *event);

private:
	void send_signal();
private:
	double			m_value;
};
