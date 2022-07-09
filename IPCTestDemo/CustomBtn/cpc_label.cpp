#include "cpc_label.h"

cpc_label::cpc_label(QWidget *parent /*= nullptr*/) :QLabel(parent), CPC_UIBase(this)
{
	m_icon = new CPC_Icon(this);
	m_icon->setIconValue(0);
	this->setText("LABEL");
}

cpc_label::cpc_label(int Icon, QString text, QWidget *parent /*= nullptr*/):QLabel(parent), CPC_UIBase(this)
{
	m_icon = new CPC_Icon(this);
	m_icon->setIconValue(Icon);
	this->setText(text);
	setAlignment(Qt::AlignCenter);
}

cpc_label::~cpc_label()
{
}
void cpc_label::resizeEvent(QResizeEvent *event)
{
	resetIconSize(width(), height());
}
