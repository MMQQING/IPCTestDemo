#include "cpc_pushbutton.h"

cpc_pushbutton::cpc_pushbutton(QWidget *parent):QPushButton(parent), CPC_UIBase(this)
{
	m_icon = new CPC_Icon(this);
	m_icon->setIconValue(0);
	this->setText("BUTTON");
}
cpc_pushbutton::cpc_pushbutton(int Icon,QString text,QWidget *parent):QPushButton(parent), CPC_UIBase(this)
{
    m_icon = new CPC_Icon(this);
    m_icon->setIconValue(Icon);
    this->setText(text);
	if (text.isEmpty())
	{
		resetIconPoint(0.5, 0);
	}
}

cpc_pushbutton::~cpc_pushbutton()
{

}

void cpc_pushbutton::resizeEvent(QResizeEvent *event)
{
	resetIconSize(width(), height());
}