#include "cpc_doublespinbox.h"
#include <QKeyEvent>

cpc_doublespinbox::cpc_doublespinbox(QWidget *parent /*= nullptr*/) :QDoubleSpinBox(parent),  CPC_UIBase(this)
{
	this->stepEnabled();
	sendSignalTimer.setSingleShot(true);
	connect(&sendSignalTimer, &QTimer::timeout, this, &cpc_doublespinbox::send_signal);
	m_icon = new CPC_Icon(this);
	m_icon->setIconValue(0);
	resetIconPoint(0, 0);
	setIconVisiable(false);
	m_value = this->value();
}


cpc_doublespinbox::cpc_doublespinbox(int Icon, QString text, QWidget *parent /*= nullptr*/) :QDoubleSpinBox(parent), CPC_UIBase(this)
{
	this->stepEnabled();
	sendSignalTimer.setSingleShot(true);
	connect(&sendSignalTimer, &QTimer::timeout, this, &cpc_doublespinbox::send_signal);

	m_icon = new CPC_Icon(this);
	m_icon->setIconValue(Icon);
	setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	setSuffix(text);
	resetIconPoint(0, 0);	
	setIconVisiable(false);
	m_value = this->value();
}

cpc_doublespinbox::~cpc_doublespinbox()
{
}

void cpc_doublespinbox::resetIconPoint(float xP /*= 0*/, float yp /*= 0*/)
{
	if (m_icon == nullptr) return;
	IconPoint_x = xP;
	IconPoint_y = yp;
	m_icon->move(QPoint(IconPoint_x*(width()  - m_icon->width()) - (IconPoint_x*10), IconPoint_y*(height() - m_icon->height())));
	UpdatePadding(m_icon->width(), m_icon->height());
}
void cpc_doublespinbox::setIconVisiable(bool visiable)
{
	if (m_icon == nullptr) return;
	m_icon->setVisible(visiable);
	UpdatePadding(m_icon->width(), m_icon->height());
}

void cpc_doublespinbox::UpdatePadding(float w, float h)
{
	QString ss;
	if (IconPoint_x < 0.5)
	{
		if (m_icon->isShow())
		{
			ss = "QDoubleSpinBox {padding-left:" + QString::number(w) + "px;padding-right:0px;}";
		}
		else
		{
			ss = "QDoubleSpinBox {padding-left:0px;padding-right:0px;}";
		}
	}
	else if (abs(IconPoint_x - 0.5) < 0.0005)
	{
		ss = "QDoubleSpinBox {padding-left:0px;padding-right:0px;}";
	}
	else
	{
		if (m_icon->isShow())
		{
			ss = "QDoubleSpinBox {padding-left:0px;padding-right:" + QString::number(w) + "px;}";
		}
		else
		{
			ss = "QDoubleSpinBox{margin-left:0px;margin-right:0px;}";
		}
	}

	this->setStyleSheet(ss);
}

void cpc_doublespinbox::resizeEvent(QResizeEvent *event)
{
	resetIconSize(width(), height());
}

void cpc_doublespinbox::valueChange(double d)
{
	send_signal();
}

void cpc_doublespinbox::wheelEvent(QWheelEvent *event)
{
	QDoubleSpinBox::wheelEvent(event);
	sendSignalTimer.start(20);
}
void cpc_doublespinbox::send_signal()
{
	sendSignalTimer.stop();
	if (m_value != this->value() || this->value() == 0)
	{
		m_value = this->value();
		emit valueChangedOwn(this->value());			
	}
}
void cpc_doublespinbox::keyPressEvent(QKeyEvent *event)
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

void cpc_doublespinbox::focusOutEvent(QFocusEvent *event)
{
	send_signal();
	QDoubleSpinBox::focusOutEvent(event);
}