#include <QPainter>
#include <QLabel>
#include <QBrush>
#include <qmath.h>

enum TextLabelType
{
	TextFram = 0x1,
	TextHighlight = 0x2,
};

typedef struct tagTextLabelHighlight
{
	int _nStart;
	int _nCount;
	QRgb _color;
	
	tagTextLabelHighlight():_nStart(-1), _nCount(0), _color(QColor(0, 0, 125).rgb()){}
	tagTextLabelHighlight(const int &nStart, const int &nCount, const QRgb &color) :_nStart(nStart), _nCount(nCount), _color(color) {}
}TextLabelHighlight;

class CCustomTextLabel : public QLabel
{
	Q_OBJECT
public:
	CCustomTextLabel(QWidget* parent = 0) : QLabel(parent)
	{
		//		show();
	}

	~CCustomTextLabel()
	{

	}

public:
	void setTextLabelHighlight(const QList<TextLabelHighlight> &qlistTLH) {
		m_TLHighlight = qlistTLH;
	}

	void setFramColor(const QColor &color) {
		m_colorFram = color;
	}

	void setTextLabelFlag(const int &nFlag) {
		m_nFlag = nFlag;
	}

private:
	void paintEvent(QPaintEvent*)
	{
		const int framSize = 1;
		QPainter painter(this);
		QRect rtWnd = rect();
		rtWnd.adjust(framSize, framSize, -framSize, -framSize);	

		int alignFlag = Qt::AlignCenter;

		if (m_nFlag & TextHighlight) {
			alignFlag = Qt::AlignVCenter | Qt::AlignLeft;
		}


		{
			QFont font;
			auto strText = text();
			font.setPointSize(9);
			int w = rtWnd.width() / strText.length();
			int textPixelSize = qMin(w, rtWnd.height());
			//font.setPixelSize(textPixelSize);
			font.setFamily("SimSun");
			font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
			painter.setFont(font);
			painter.setPen(QColor(m_colorText));

			QFontMetricsF fontMetrics(painter.font());
			qreal wid = fontMetrics.width(strText);
			painter.drawText(rtWnd, alignFlag, strText);
		}

		if (m_nFlag & TextFram)
		{
			painter.setPen(QPen(m_colorText, framSize));
			painter.setBrush(Qt::NoBrush);
			painter.drawRect(rtWnd);
		}

		if (m_nFlag & TextHighlight)
		{
			QFontMetricsF fontMetrics(painter.font());
			auto strText = text();

			for each (auto var in m_TLHighlight)
			{
				if (var._nCount > 0 && var._nStart >= 0)
				{
					auto tmpText = strText.left(var._nStart);
					qreal wid = fontMetrics.width(tmpText);

					painter.setPen(QColor(var._color));
			
					tmpText = strText.mid(var._nStart, var._nCount);
					qreal wid2 = fontMetrics.width(tmpText);

					QRect tmpRect(rtWnd);
					tmpRect.setLeft(tmpRect.left() + wid);
					tmpRect.setRight(tmpRect.left() + wid2);

					painter.drawText(tmpRect, alignFlag, tmpText);
				}
			}
		}
	}
private:
	QColor m_colorText{ 255, 255, 255 };
	QColor m_colorFram{ 255, 255, 255 };
	int m_nFlag{ 0 };

	QList<TextLabelHighlight> m_TLHighlight;
};