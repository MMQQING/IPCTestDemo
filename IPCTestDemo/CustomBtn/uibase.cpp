#include "uibase.h"
#include "cpc_icon.h"
#include <QFontDatabase>

CPC_UIBase::CPC_UIBase(QWidget* pwidget, QWidget*parent) :m_widget(pwidget), m_parent(parent)
{
    m_fontSize = 20;	
	m_fontFamily = "FontAwesome";	
    m_font.setPointSize(m_fontSize);
    m_font.setFamily(m_fontFamily);
}
CPC_UIBase::~CPC_UIBase()
{
}
void CPC_UIBase::setFontSize(int size){
    m_fontSize = size;
    m_font.setPointSize(m_fontSize);
    if(m_widget)
        m_widget->setFont(m_font);
}
void CPC_UIBase::setFontFamily(QString family){
    m_fontFamily = family;
    m_font.setFamily(m_fontFamily);
    if(m_widget)
        m_widget->setFont(m_font);
}
void CPC_UIBase::resetIconSize(int width, int height)
{
	if (m_icon == nullptr) return;
	auto minsize = qMin(width, height);
	m_icon->resize(minsize, minsize);
	m_icon->seticonSize(minsize, minsize);
	resetIconPoint(IconPoint_x, IconPoint_y);
}

void CPC_UIBase::resetIconValue(int Icon)
{
	if (m_icon == nullptr || m_widget == nullptr) return;
	m_icon->setIconValue(Icon);
	resetIconSize(m_widget->width(), m_widget->height());
	
}
void CPC_UIBase::resetIconString(QString Icon)
{
	if (m_icon == nullptr) return;
	m_icon->setIconString(Icon);
}

void CPC_UIBase::resetIconPoint(float xP /*= 0*/, float yp /*= 0*/)
{
	if (m_widget == nullptr || m_icon == nullptr) return;
	IconPoint_x = xP;
	IconPoint_y = yp;
	m_icon->move(QPoint(IconPoint_x*(m_widget->width() - m_icon->width()), IconPoint_y*(m_widget->height() - m_icon->height())));

}
void CPC_UIBase::resetIconColor(QString color)
{
	if (m_icon == nullptr) return;
	m_icon->setIconColor(color);
}

void CPC_UIBase::setIconVisiable(bool visiable)
{
	if (m_icon == nullptr) return;
	m_icon->setVisible(visiable);
}