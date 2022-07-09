#include <QPainter>
#include <QLabel>
#include <QBrush>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QHash>
#include <qmath.h>

using namespace QtCharts;

//QRadioButton重写：
class CCustomRateLabel : public QLabel
{
	Q_OBJECT
public:
	CCustomRateLabel(QWidget* parent = 0) : QLabel(parent)
	{
	}

	void setValue(const double &d) {
		if (m_value != d)
		{
			m_value = d;
			update();
		}
	}

	double getValue() const {
		return m_value;
	}

	~CCustomRateLabel()
	{
	}	
	void paintEvent(QPaintEvent*)
	{
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);

		int showTextH = 40;
		QRect rt = rect();
		QRect showRect(QPoint(rt.left(), rt.bottom() - showTextH), rt.bottomRight());
		QRect chartRect = rt.adjusted(0, 0, 0, -showTextH);


		QPoint pt = chartRect.center();
		int radius = qMin(chartRect.width(), chartRect.height()) - 2; //直径
		QRect rect(pt.x() - radius / 2, pt.y() - radius / 2, radius, radius);

		qreal rateRadius = 0.7;

		painter.save();
		painter.setPen(QPen(QColor(m_color), 1));
		painter.setBrush(Qt::NoBrush);
		painter.drawEllipse(pt, radius / 2, radius / 2);   //大圆
		painter.drawEllipse(pt, int(radius*rateRadius / 2), int(radius*rateRadius / 2));   //小圆
		painter.restore();

		painter.save();
		painter.setPen(Qt::NoPen);

		QRadialGradient firstGradient(pt, radius / 2);
		firstGradient.setColorAt(0, Qt::transparent);
		firstGradient.setColorAt(rateRadius, Qt::transparent);
		firstGradient.setColorAt(rateRadius + 0.01, m_color);
		firstGradient.setColorAt(1.0, m_color);

		painter.setBrush(firstGradient);
		int endPos = 90;
		int countPos = (m_value / m_valueMax) * 360;
		int startPos = endPos - countPos;

		printf("startPos=%d\n", startPos);

		painter.drawPie(rect, startPos * 16, countPos * 16);
		painter.restore();
		{
			auto radius2 = radius*rateRadius;
			QRect rtText(pt.x() - radius2 / 2, pt.y() - radius2 / 2, radius2, radius2);

			QString strText;
			strText.sprintf("%.0f", floor(m_value));
			QFont font;

			int w = rtText.width() / strText.length();
			int textPixelSize = qMin(w, rtText.height());
			font.setPixelSize(textPixelSize);

			font.setFamily("Microsoft YaHei");
			font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
			painter.setFont(font);
			painter.drawText(rtText, Qt::AlignCenter, strText);
		}
		{
			showRect.moveTop(pt.y() + radius / 2);

			QString strText = text();// "测试";
			QFont font;
			font.setPointSize(12);
			int w = showRect.width() / strText.length();
			int textPixelSize = qMin(w, showRect.height());
			//font.setPixelSize(textPixelSize);
			font.setFamily("SimSun");
			font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
			painter.setFont(font);

			painter.drawText(showRect, Qt::AlignCenter, strText);
		}


	}

private:
	double AngleToArc(const double& Angle)
	{
		return Angle*m_PI / 180;
	}


	QPoint getMovePoint(qreal angel)
	{
		int wl = 0;
		int hl = 0;
		qreal ang = AngleToArc(angel);

		wl = 20 * qCos(ang);
		hl = 20 * qSin(ang);

		if (angel > 90 && angel < 270)
			wl = qAbs(wl)*-1;
		else
			wl = qAbs(wl);

		if (angel < 180)
			hl = qAbs(hl)*-1;
		else
			hl = qAbs(hl);

		return QPoint(wl, hl);
	}
private:
	const double m_PI{ 3.14159265358979323846 };   // pi
	double m_value{ 0.00 };
	double m_valueMax{ 100.00 };
	QColor m_color{ 255, 255, 255 };
};