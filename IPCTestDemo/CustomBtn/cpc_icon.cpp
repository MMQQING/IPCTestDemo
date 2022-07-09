#include "cpc_icon.h"

CPC_Icon::CPC_Icon(QWidget *parent) : QLabel(parent) ,CPC_UIBase(this,parent)
{
    m_widget = this;
    this->setFont(m_font);
    qDebug()<<m_font.family();
    this->setText(QChar(m_iconValue));

    this->setMinimumSize(QSize(10,10));
	setAlignment(Qt::AlignCenter);
	m_isShow = true;
	this->adjustSize();
}
void CPC_Icon::setIconValue(int iconValue)
{
	m_iconString.clear();
    m_iconValue = iconValue;
	if (m_iconValue == 0 && m_iconString.isEmpty())
	{
		setVisible(false);
	}
    this->setText(QChar(m_iconValue));
}
void CPC_Icon::setIconString(QString iconString)
{
	m_iconString = iconString;
	if (m_iconValue == 0 && m_iconString.isEmpty())
	{
		setVisible(false);
	}
	this->setText(m_iconString);
	if (!m_iconString.isEmpty())
	{
		this->adjustSize();
		//填满高度
		resize(this->width(), m_parent->height());
	}
}
void CPC_Icon::setIconColor(QString color)
{
    m_color = color;
    this->setStyleSheet("CPC_Icon:enabled{color:"+m_color+"}");
}
void CPC_Icon::seticonSize(int w, int h){
    setFontSize((w<h?w:h)*0.5);
	resize(w, h);
    QLabel::resize(w,h);
	if (!m_iconString.isEmpty())
	{
		this->adjustSize();
		//填满高度
		resize(this->width(), m_parent->height());
	}
		
}
void CPC_Icon::setVisible(bool show)
{
	QWidget::setVisible(show);
	m_isShow = show;
}

bool CPC_Icon::isShow()
{
	return m_isShow;
}

void CPC_Icon::resizeEvent(QResizeEvent *event)
{
	emit IconSizeChanged((float)this->width(), (float)this->height());
}
