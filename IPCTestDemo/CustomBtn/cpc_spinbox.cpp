#include "CPC_SpinBox.h"
#include <QKeyEvent>

CPC_SpinBox::CPC_SpinBox(QWidget *parent /*= nullptr*/) :QSpinBox(parent),  CPC_UIBase(this)
{
	this->stepEnabled();
	sendSignalTimer.setSingleShot(true);
	connect(&sendSignalTimer, &QTimer::timeout, this, &CPC_SpinBox::send_signal);
	//connect(this, &QDoubleSpinBox::editingFinished, this, &CPC_SpinBox::send_signal);

	m_icon = new CPC_Icon(this);
	m_icon->setIconValue(0);
	setIconVisiable(false);
	m_value = this->value();
}

CPC_SpinBox::CPC_SpinBox(int Icon, QString text, QWidget *parent /*= nullptr*/) :QSpinBox(parent), CPC_UIBase(this)
{
	this->stepEnabled();
	sendSignalTimer.setSingleShot(true);
	connect(&sendSignalTimer, &QTimer::timeout, this, &CPC_SpinBox::send_signal);
	//connect(this, &QDoubleSpinBox::editingFinished, this, &CPC_SpinBox::send_signal);

	m_icon = new CPC_Icon(this);
	m_icon->setIconValue(Icon);
	setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	setSuffix(text);
	resetIconPoint(0, 0);	
	setIconVisiable(false);
	m_value = this->value();
}

CPC_SpinBox::~CPC_SpinBox()
{
	sendSignalTimer.stop();
}

void CPC_SpinBox::resetIconPoint(float xP /*= 0*/, float yp /*= 0*/)
{
	if (m_icon == nullptr) return;
	IconPoint_x = xP;
	IconPoint_y = yp;
	m_icon->move(QPoint(IconPoint_x*(width() - m_icon->width()) - (IconPoint_x*10), IconPoint_y*(height() - m_icon->height())));
	UpdatePadding(m_icon->width(), m_icon->height());
}

void CPC_SpinBox::setIconVisiable(bool visiable)
{
	if (m_icon == nullptr) return;
	m_icon->setVisible(visiable);
	UpdatePadding(m_icon->width(), m_icon->height());
}

// void CPC_SpinBox::setValue(int val)
// {
// 	this->setValue(val);
// 	send_signal();
// }

void CPC_SpinBox::UpdatePadding(float w, float h)
{
	QString ss;
	if (IconPoint_x < 0.5)
	{
		if (m_icon->isShow())
		{
			ss = "QSpinBox{padding-left:" + QString::number(w) + "px;padding-right:0px;}";
		}
		else
		{
			ss = "QSpinBox {padding-left:0px;padding-right:0px;}";
		}
	}
	else if (abs(IconPoint_x - 0.5) < 0.0005)
	{
		ss = "QSpinBox {padding-left:0px;padding-right:0px;}";
	}
	else
	{
		if (m_icon->isShow())
		{
			ss = "QSpinBox{padding-left:0px;padding-right:" + QString::number(w) + "px;}";
		}
		else
		{
			ss = "QSpinBox{margin-left:0px;margin-right:0px;}";
		}
	}

	this->setStyleSheet(ss);
}

void CPC_SpinBox::keyPressEvent(QKeyEvent *event)
{
	QSpinBox::keyPressEvent(event);
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

void CPC_SpinBox::focusOutEvent(QFocusEvent *event)
{
	send_signal();
	QSpinBox::focusOutEvent(event);
}

void CPC_SpinBox::resizeEvent(QResizeEvent *event)
{
	resetIconSize(width(), height());
}
void CPC_SpinBox::wheelEvent(QWheelEvent *event)
{
	QSpinBox::wheelEvent(event);
	sendSignalTimer.start(20);
}
void CPC_SpinBox::send_signal()
{
	sendSignalTimer.stop();
	if (m_value != this->value() || this->value() == 0)
	{
		m_value = this->value();
		emit valueChangedOwn(this->value());
	}
}