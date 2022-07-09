#include "cpc_slider.h"

CPC_Slider::CPC_Slider(QWidget *parent /*= nullptr*/) :QSlider(parent), CPC_UIBase(this)
{
	m_icon = new CPC_Icon(this);
	connect(this, &CPC_Slider::valueChanged, this, &CPC_Slider::UpdateIconString);
	connect(m_icon, &CPC_Icon::IconSizeChanged, this, &CPC_Slider::UpdatePadding);
	m_icon->setIconValue(0);
	setMinimum(0);
	setMaximum(100);
	setValue(0);
	setIconVisiable(true);
	setOrientation(Qt::Horizontal);
}

CPC_Slider::CPC_Slider(int Icon,float min,float max, QWidget *parent /*= nullptr*/):QSlider(parent), CPC_UIBase(this)
{
	Icon = 1;
	m_icon = new CPC_Icon(this);
	connect(this, &CPC_Slider::valueChanged, this, &CPC_Slider::UpdateIconString);
	connect(m_icon, &CPC_Icon::IconSizeChanged, this, &CPC_Slider::UpdatePadding);
	connect(this, &CPC_Slider::sliderReleased, this, &CPC_Slider::UpdatePaddingRelease);
	m_icon->setIconValue(Icon);
	setMinimum(min);
	setMaximum(max);
	setValue(100);
	setValue(min);
	setIconVisiable(true);
	setOrientation(Qt::Horizontal);
}

CPC_Slider::~CPC_Slider()
{
}
void CPC_Slider::setOrientation(Qt::Orientation s)
{
	QSlider::setOrientation(s);
	m_curOri = s;
}
void CPC_Slider::resetIconSize(int width, int height)
{
	if (m_icon == nullptr) return;
	auto minsize = qMin(width, height);
	m_icon->resize(minsize, minsize);
	m_icon->seticonSize(minsize, minsize);
	resetIconPoint(IconPoint_x, IconPoint_y);
}
void CPC_Slider::setIconVisiable(bool visiable)
{
	if (m_icon == nullptr) return;
	m_icon->setVisible(visiable);
	UpdatePaddingRelease();
}
void CPC_Slider::resizeEvent(QResizeEvent *event)
{
	resetIconSize(width(), height());
	UpdatePaddingRelease();
}

void CPC_Slider::UpdateIconString(int value)
{
	int pro = (value*1.0 / (maximum() - minimum()))*100;

	resetIconString(QString::number(value, 'f', 0));
}
void CPC_Slider::resetIconPoint(float xP /*= 0*/, float yp /*= 0*/)
{
	if (m_widget == nullptr || m_icon == nullptr) return;
	IconPoint_x = xP;
	IconPoint_y = yp;
	m_icon->move(QPoint(IconPoint_x*(m_widget->width() - m_icon->width()), IconPoint_y*(m_widget->height() - m_icon->height())));
	UpdatePadding(m_icon->width(), m_icon->height());
}
void CPC_Slider::UpdatePaddingRelease()
{
	maxw = m_icon->width()+1;
	UpdatePadding(maxw, maxw);
}
void CPC_Slider::UpdatePadding(float w, float h)
{
	if (m_icon == nullptr) { return; }
	w += 1;
	maxw = std::max(w, maxw) ;
	QString ss;
	if (IconPoint_x < 0.5)
	{
		if (m_icon->isShow())
		{
			ss = "CPC_Slider::groove:horizontal {left:" + QString::number(maxw) + "px;right:0px;}CPC_Slider::groove:vertical {top:" + QString::number(maxw) + "px;bottom:0px;}";
		}
		else
		{
			ss = "CPC_Slider::groove:horizontal {left:0px;right:0px;}CPC_Slider::groove:vertical {top:0px;right:0px;}";
		}
	}
	else if (abs(IconPoint_x - 0.5) < 0.0005)
	{
		ss = "CPC_Slider::groove:horizontal {left:0px;right:0px;}CPC_Slider::groove:vertical {top:0px;bottom:0px;}";
	}
	else
	{
		if (m_icon->isShow())
		{
			ss = "CPC_Slider::groove:horizontal {left:0px;right:" + QString::number(maxw) + "px;}CPC_Slider::groove:vertical {top:0px;bottom:" + QString::number(maxw) + "px;}";
		}
		else
		{
			ss = "CPC_Slider::groove:horizontal {left:0px;right:0px;}CPC_Slider::groove:vertical {top:0px;bottom:0px;}";
		}
	}

	this->setStyleSheet(ss);
}