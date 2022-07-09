#include "cpc_switch.h"
#include "QPainter"
#include "QMouseEvent"
CPC_Switch::CPC_Switch(QWidget *parent)
	: QWidget(parent)
{
	connect(&m_timer, &QTimer::timeout, this, &CPC_Switch::onTimeOut);
	setMinimumHeight(20);
	setColor(QColor("#a0a0a0"), QColor("#08D82F"));
}

CPC_Switch::~CPC_Switch()
{
}

bool CPC_Switch::isChecked() const
{
	return m_bChecked;
}

void CPC_Switch::setChecked(bool checked)
{
	m_bChecked = checked;
	m_timer.start(1);
}


void CPC_Switch::setColor(QColor backColor, QColor checkColor)
{
	m_background = QColor(backColor.red(), backColor.green(), backColor.blue(), backColor.alpha()*0.5);
	m_checkBGColor = QColor(checkColor.red(), checkColor.green(), checkColor.blue(), checkColor.alpha()*0.8);
	m_thumbCOlor = backColor;
	m_checkedColor = checkColor;
}

void CPC_Switch::setMargin(float margin)
{
	m_nMargin = margin;
}

void CPC_Switch::setRadius(float radius)
{
	radius = radius > 1 ? 1 : radius;
	radius = radius < 0 ? 0 : radius;
	m_radius = radius*0.5;
}

void CPC_Switch::paintEvent(QPaintEvent*event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setRenderHint(QPainter::Antialiasing);
	QPainterPath pathback;
	QPainterPath pathhandle;
	QColor background;
	QColor thumbColor;
	qreal dOpacity;
	if (isEnabled()) {
		if (m_bChecked) {
			background = m_checkBGColor;
			thumbColor = m_checkedColor;
			dOpacity = 1;
		}
		else
		{
			background = m_background;
			thumbColor = m_thumbCOlor;
			dOpacity = 0.5;
		}
	}
	else
	{
		background = m_disableColor;
		thumbColor = m_disableColor;
		dOpacity = 0.5;
	}
	painter.setBrush(background);
	painter.setOpacity(dOpacity);
	float handSize = height() - 2 * m_nMargin;
	pathback.addRoundedRect(QRectF(m_nMargin, m_nMargin, width() - 2 * m_nMargin, handSize), handSize*m_radius, handSize*m_radius);
	painter.drawPath(pathback.simplified());

	painter.setBrush(thumbColor);
	painter.setOpacity(1.0);
	pathhandle.addRoundedRect(QRectF(m_nX, m_nMargin, handSize, handSize), handSize*m_radius, handSize*m_radius);
	painter.drawPath(pathhandle.simplified());
	//painter.drawEllipse(QRectF(m_nX,m_nMargin, height() - 2 * m_nMargin, height() - 2 * m_nMargin));
}

void CPC_Switch::mousePressEvent(QMouseEvent*event)
{
	if (isEnabled()) {
		if (event->buttons()&Qt::LeftButton) {
			event->accept();
		}
		else
		{
			event->ignore();
		}
	}
}

void CPC_Switch::mouseReleaseEvent(QMouseEvent*event)
{
	if(isEnabled())
	{
		if ((event->type() == QMouseEvent::MouseButtonRelease) && (event->button() == Qt::LeftButton))
		{
			event->accept();
			m_bChecked = !m_bChecked;
			emit onChecke(m_bChecked);
			m_timer.start(1);
		}
		else
		{
			event->ignore();
		}
	}
}

void CPC_Switch::resizeEvent(QResizeEvent*event)
{
	if (m_bChecked) {
		m_nX = width() - height() + m_nMargin;
	}
	else
	{
		m_nX = m_nMargin;
	}
	QWidget::resizeEvent(event);
}

QSize CPC_Switch::sizeHint() const 
{
	return minimumSizeHint();
}

QSize CPC_Switch::minimumSizeHint() const 
{
	return QSize(2 * (m_nHeight + m_nMargin), m_nHeight + 2 * m_nMargin);
}

void CPC_Switch::onTimeOut()
{
	if (m_bChecked) {
		m_nX += width()/100.0;
		if (m_nX >= width() - height() + m_nMargin)
		{
			m_nX = width() - height() + m_nMargin;
			m_timer.stop();
		}
	}
	else
	{
		m_nX -= width() / 100.0;
		if (m_nX <= m_nMargin) {
			m_nX = m_nMargin;
			m_timer.stop();
		}
	}
	update();
}

