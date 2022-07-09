#include "cpc_lineedit.h"


cpc_lineedit::cpc_lineedit(QWidget *parent /*= nullptr*/) :QLineEdit(parent),  CPC_UIBase(this)
{
	m_icon = new CPC_Icon(this);
	m_icon->setIconValue(0);
	this->setText("");
}


cpc_lineedit::cpc_lineedit(int Icon, QString text, QWidget *parent /*= nullptr*/) :QLineEdit(parent), CPC_UIBase(this)
{
	m_icon = new CPC_Icon(this);
	m_icon->setIconValue(Icon);
	this->setText(text);
	setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	IconPoint_x = 1;	
}

cpc_lineedit::~cpc_lineedit()
{
}

void cpc_lineedit::resetIconPoint(float xP /*= 0*/, float yp /*= 0*/)
{
	if (m_icon == nullptr) return;
	IconPoint_x = xP;
	IconPoint_y = yp;
	m_icon->move(QPoint(IconPoint_x*(width() - m_icon->width()), IconPoint_y*(height() - m_icon->height())));
	if (IconPoint_x > 0.5)
	{
		setTextMargins(0, 0, m_icon->width(), 0);
	}
	else if(IconPoint_x < 0.5)
	{
		setTextMargins(m_icon->width(), 0, 0, 0);
	}
	if (!m_icon->isShow())
	{
		setTextMargins(0, 0, 0, 0);
	}
}
void cpc_lineedit::setIconVisiable(bool visiable)
{
	if (m_icon == nullptr) return;
	m_icon->setVisible(visiable);
	if (IconPoint_x > 0.5)
	{
		setTextMargins(0, 0, m_icon->width(), 0);
	}
	else if (IconPoint_x < 0.5)
	{
		setTextMargins(m_icon->width(), 0, 0, 0);
	}
	if (!visiable)
	{
		setTextMargins(0, 0, 0, 0);
	}
}
void cpc_lineedit::resizeEvent(QResizeEvent *event)
{
	resetIconSize(width(), height());
}
