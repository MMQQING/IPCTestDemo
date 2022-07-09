#include "CPCQDoubleSpinBox.h"
#include <QEvent>
#include <QKeyEvent>

CPCQDoubleSpinBox::CPCQDoubleSpinBox(QWidget *parent)
	: QDoubleSpinBox(parent)
{
	m_value = this->value();
	this->setButtonSymbols(NoButtons);
}

CPCQDoubleSpinBox::~CPCQDoubleSpinBox()
{
}

void CPCQDoubleSpinBox::keyPressEvent(QKeyEvent *event)
{
	QDoubleSpinBox::keyPressEvent(event);
	switch (event->key())
	{
	case Qt::Key_Enter:
	case Qt::Key_Up:
	case Qt::Key_Down:
	case Qt::Key_Return:
		send_signal();
		return;
	default:
		break;
	}
}

void CPCQDoubleSpinBox::focusOutEvent(QFocusEvent *event)
{
	send_signal();
	QDoubleSpinBox::focusOutEvent(event);
}

void CPCQDoubleSpinBox::wheelEvent(QWheelEvent *event)
{
	QDoubleSpinBox::wheelEvent(event);
	send_signal();
}

void CPCQDoubleSpinBox::send_signal()
{ 
	if (m_value != this->value() || this->value() == 0)
	{
		m_value = this->value();
		emit valueChangedOwn(this->value());
	}
}

