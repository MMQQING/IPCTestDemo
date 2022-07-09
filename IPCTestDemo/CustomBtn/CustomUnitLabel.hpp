#include <QPainter>
#include <QLabel>
#include <QBrush>
#include <qmath.h>
#include <QMouseEvent>


class CCustomUnitLabel : public QLabel
{
	Q_OBJECT
public:
	CCustomUnitLabel(QWidget* parent = 0) : QLabel(parent)
	{
		//		show();
	}

	~CCustomUnitLabel()
	{

	}
	void setValue(const double &d) {
		if (m_temperature != d)
		{
			m_temperature = d;
			update();
		}
	}

	double getValue() const {
		return m_temperature;
	}

	void setUnit(const QString &strUnit)
	{
		m_strUnit = strUnit;
		update();
	}

	void paintEvent(QPaintEvent*)
	{
		const int framSize = 1;
		QPainter painter(this);
		QFont font;
		QRect rectWnd = rect();

		QRect rectTemperature(rectWnd);
		rectTemperature.setRight(rectTemperature.left() + rectWnd.width()*0.8);
		rectTemperature.setBottom(rectTemperature.top() + rectWnd.height()*0.8);

		QRect rectSymbol(rectWnd);
		rectSymbol.setLeft(rectTemperature.right());
		rectSymbol.setBottom(rectTemperature.bottom());

		QRect rectText(rectWnd);
		rectText.setTop(rectTemperature.bottom());
		rectText.setRight(rectText.left() + rectWnd.width()*0.7);

		QRect rectStatus(rectWnd);
		rectStatus.setTop(rectTemperature.bottom());
		rectStatus.setLeft(rectText.right());
		rectStatus.setBottom(rectText.bottom());

		auto mydrawText=[&](const QRect &rt, const int &flag, const QString &strText)
		{
			if (rt.isValid() && !strText.isEmpty())
			{				
				//font.setPointSize(12);
				int w = rt.width() / strText.length();
				int textPixelSize = qMin(w, rt.height());
				font.setPixelSize(textPixelSize * 0.6);
				font.setFamily("Microsoft YaHei");
				font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
				painter.setFont(font);
				painter.drawText(rt, flag, strText);
			}
		};

		mydrawText(rectTemperature, Qt::AlignRight | Qt::AlignBottom, QString::number(m_temperature, 'f', 2));
		if (!m_strUnit.isEmpty())
		{
			mydrawText(rectSymbol, Qt::AlignLeft | Qt::AlignBottom, m_strUnit);
		}
		
		mydrawText(rectText, Qt::AlignVCenter | Qt::AlignRight, text());

		QColor colorStatus(0, 128, 0);
		QString strStatus = "正常";

		if (m_temperature > m_WarningThreshold)
		{
			strStatus = "过高";
			colorStatus = QColor(128, 0, 0);
		}

		//mydrawText(rectStatus, Qt::AlignVCenter | Qt::AlignLeft, strStatus);

		font.setFamily("Microsoft YaHei");
		font.setPointSize(30);	
		QFontMetricsF fontMetrics(font);
		qreal wid = fontMetrics.width(strStatus);
		qreal height = fontMetrics.height();
		rectStatus.setRight(rectStatus.left() + wid);
		painter.setBrush(colorStatus);
		painter.drawRect(rectStatus);

		mydrawText(rectStatus, Qt::AlignVCenter | Qt::AlignHCenter, strStatus);
	}

private:
	QColor m_color{ 255, 0, 0 };
	double m_temperature{0};
	double m_WarningThreshold{70};
	QString m_strUnit;
};