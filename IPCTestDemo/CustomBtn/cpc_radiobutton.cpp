#include "cpc_radiobutton.h"

CPC_RadioButton::CPC_RadioButton(QWidget *parent /*= nullptr*/) :QRadioButton(parent), CPC_UIBase(this)
{
	m_icon = new CPC_Icon(this);
	m_icon->setIconValue(0);
	this->setText("Radio");
	setIconVisiable(false);
}

CPC_RadioButton::CPC_RadioButton(int Icon, QString text, QWidget *parent /*= nullptr*/):QRadioButton(parent), CPC_UIBase(this)
{
	m_icon = new CPC_Icon(this);
	//resetIconColor("#562584");
	m_icon->setIconValue(Icon);
	this->setText(text);
	setIconVisiable(false);
}

CPC_RadioButton::~CPC_RadioButton()
{
}
void CPC_RadioButton::resizeEvent(QResizeEvent *event)
{
	resetIconSize(width(), height());
}
