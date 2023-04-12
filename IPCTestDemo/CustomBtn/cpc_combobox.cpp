#include "cpc_combobox.h"

cpc_combobox::cpc_combobox(QWidget *parent /*= nullptr*/) :QComboBox(parent)/*, CPC_UIBase(this)*/
{
// 	m_icon = new CPC_Icon(this);
// 	m_icon->setIconValue(0xf03a);
// 	resetIconPoint(1);
// 	resetIconColor("#84C1FF");
}

// cpc_combobox::cpc_combobox(int Icon, QString text, QWidget *parent /*= nullptr*/):QComboBox(parent), CPC_UIBase(this)
// {
// 	if (Icon == 0)
// 	{
// 		Icon = 0xf03a;
// 	}
// 	m_icon = new CPC_Icon(this);
// 	resetIconColor("#84C1FF");
// 	m_icon->setIconValue(Icon);
// 	resetIconPoint(1);
// }

cpc_combobox::~cpc_combobox()
{
}
void cpc_combobox::resizeEvent(QResizeEvent *event)
{
	//resetIconSize(width(), height());
}

void cpc_combobox::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		emit clicked();  //触发clicked信号
	}

	QComboBox::mousePressEvent(e);  //将该事件传给父类处理，这句话很重要，如果没有，父类无法处理本来的点击事件
}
