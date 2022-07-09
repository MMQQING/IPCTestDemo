#include "cpc_progress.h"

cpc_progress::cpc_progress(QWidget *parent /*= nullptr*/) :QProgressBar(parent), CPC_UIBase(this)
{
	m_icon = new CPC_Icon(this);
	connect(this, &QProgressBar::valueChanged, this, &cpc_progress::UpdateIconString);
	connect(m_icon, &CPC_Icon::IconSizeChanged, this, &cpc_progress::UpdatePadding);
	m_icon->setIconValue(0);
	setMinimum(0);
	setMaximum(100);
	setValue(0);
	resetIconColor("#64A600");
	setTextVisible(true);
	setIconVisiable(true);
}

cpc_progress::cpc_progress(int Icon,float min,float max, QWidget *parent /*= nullptr*/):QProgressBar(parent), CPC_UIBase(this)
{
	Icon = 1;
	m_icon = new CPC_Icon(this);
	connect(this, &QProgressBar::valueChanged, this, &cpc_progress::UpdateIconString);
	connect(m_icon, &CPC_Icon::IconSizeChanged, this, &cpc_progress::UpdatePadding);
	resetIconColor("#64A600");
	m_icon->setIconValue(Icon);
	setMinimum(min);
	setMaximum(max);
	setValue(min);
	//setAlignment(Qt::AlignCenter);
	setTextVisible(true);
	setIconVisiable(true);
}

cpc_progress::~cpc_progress()
{
}
void cpc_progress::resetIconSize(int width, int height)
{
	if (m_icon == nullptr) return;
	auto minsize = qMin(width, height);
	m_icon->resize(minsize, minsize);
	m_icon->seticonSize(minsize, minsize);
	resetIconPoint(IconPoint_x, IconPoint_y);
}
void cpc_progress::setIconVisiable(bool visiable)
{
	if (m_icon == nullptr) return;
	QString ss;
	if (visiable)
	{
		ss = "cpc_progress {padding-left:" + QString::number(m_icon->width()+1) + "px;padding-right:0px;}";
	}
	else
	{
		ss = "cpc_progress {padding-left:0px;padding-right:0px;}";
	}
	this->setStyleSheet(ss);
}
void cpc_progress::resizeEvent(QResizeEvent *event)
{
	resetIconSize(width(), height());
	UpdatePadding(m_icon->width(), m_icon->height());
}

void cpc_progress::UpdateIconString(int value)
{
	int pro = (value*1.0 / (maximum() - minimum()))*100;
	resetIconString(QString::number(pro, 'f', 0)+"%");
}
void cpc_progress::resetIconPoint(float xP /*= 0*/, float yp /*= 0*/)
{
	CPC_UIBase::resetIconPoint(xP, yp);
	UpdatePadding(m_icon->width(), m_icon->height());
}

void cpc_progress::UpdatePadding(float w, float h)
{
	w += 1;
	if (m_icon == nullptr) { return; }
	QString ss;
	if (IconPoint_x < 0.5)
	{
		if (m_icon->isShow())
		{
			ss = "cpc_progress {padding-left:" + QString::number(w) + "px;padding-right:0px;}";
		}
		else
		{
			ss = "cpc_progress {padding-left:0px;padding-right:0px;}";
		}
	}
	else if (abs(IconPoint_x - 0.5) < 0.0005)
	{
		ss = "QProgressBar {padding-left:0px;padding-right:0px;}";
	}
	else
	{
		if (m_icon->isShow())
		{
			ss = "cpc_progress {padding-left:0px;padding-right:" + QString::number(w) + "px;}";
		}
		else
		{
			ss = "cpc_progress {padding-left:0px;padding-right:0px;}";
		}
	}

	this->setStyleSheet(ss);
}
