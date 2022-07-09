#include <QCommonStyle>
#include <QStyleOptionButton>
#include <QRadioButton>
#include <QPainter>

class MediaStyle : public QCommonStyle
{
    Q_OBJECT
public:
    MediaStyle() : QCommonStyle() {}

    QRect subElementRect(SubElement sr, const QStyleOption* opt, const QWidget* widget) const
    {
        if (SE_RadioButtonIndicator == sr)
        {
            QRect rect;
            int nWidth = proxy()->pixelMetric(PM_ExclusiveIndicatorWidth, opt, widget);
            int nHeight = proxy()->pixelMetric(PM_ExclusiveIndicatorHeight, opt, widget);
            rect.setRect(opt->rect.x() + ((opt->rect.width() - nWidth) / 2 + 1),
                         opt->rect.y() + (opt->rect.height() - nHeight),
                         nWidth, nHeight);
            return visualRect(opt->direction, opt->rect, rect);
        }
        else if (SE_RadioButtonContents == sr)
        {
            QRect rect;
            QRect rc = visualRect(opt->direction, opt->rect, subElementRect(SE_RadioButtonIndicator, opt, widget));
            int nSpacing = proxy()->pixelMetric(PM_RadioButtonLabelSpacing, opt, widget);
            rect.setRect(opt->rect.x() + ((opt->rect.height()- rc.height() - nSpacing) / 2), opt->rect.y(),
                         opt->rect.width(), opt->rect.height() - rc.height() - nSpacing);
            return visualRect(opt->direction, opt->rect, rect);
        }
//        else if (SE_RadioButtonClickRect == sr)
//        {
//            QRect rect = subElementRect(SE_RadioButtonFocusRect, opt, widget);
//            rect |= subElementRect(SE_RadioButtonIndicator, opt, widget);
//        }

        return QCommonStyle::subElementRect(sr, opt, widget);
    }

    QSize sizeFromContents(ContentsType ct, const QStyleOption* opt, const QSize& csz, const QWidget* widget = 0) const
    {
        if (CT_RadioButton == ct)
        {
            QSize sz(csz);
            if (const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(opt))
            {
                int w = proxy()->pixelMetric(PM_ExclusiveIndicatorWidth, btn, widget);
                int h = proxy()->pixelMetric(PM_ExclusiveIndicatorHeight, btn, widget);

                int margins = 0;
                // we add 4 pixels for label margins
                if (!btn->icon.isNull() || !btn->text.isEmpty())
                    margins = 4 + proxy()->pixelMetric(PM_RadioButtonLabelSpacing, opt, widget);
                sz += QSize(4, h + margins);
                sz.setWidth(qMax(sz.width(), w));
                return sz;
            }
        }

        return QCommonStyle::sizeFromContents(ct, opt, csz, widget);
    }
};



//QRadioButton重写：
class CCustomRadioBtn : public QRadioButton
{
    Q_OBJECT
public:
	CCustomRadioBtn(QWidget* parent = 0) : CCustomRadioBtn("test", parent)
	{

	}


    CCustomRadioBtn(const QString& path, QWidget* parent = 0) 
		: QRadioButton(parent)
		//, m_pMediaStyle(new MediaStyle)
    {
//        this->setMinimumSize(0, 0);
//        this->setIcon(QIcon(path));
//        this->setIconSize(QSize(0, 0));
//        this->setAttribute(Qt::WA_TranslucentBackground);
//		this->setAttribute(Qt::WA_StyledBackground); // 启用 QSS
//		this->setStyleSheet("border: 2px solid red; background: pink; border-radius: 10px;"); // 设置 QSS
//        this->setStyle(m_pMediaStyle);
    }

	void setImage(const QString& path) {
		m_image.load(path);
	}

    ~CCustomRadioBtn()
    {
 //       delete m_pMediaStyle;
    }

	void paintEvent(QPaintEvent*) 
	{
//		QStyleOption opt;
//		opt.init(this);
		QPainter painter(this);
//		painter.setRenderHint(QPainter::Antialiasing, true);
//		style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
		QRect rt = rect();

		if (!m_image.isNull())
		{
			painter.drawImage(rt, m_image);
		}

		QColor cl(0, 0, 0);

		if (!isChecked())
		{
			cl = QColor(92, 92, 92);

			QLinearGradient linearGradient(rt.x(), rt.y(), rt.width(), rt.height());
			linearGradient.setColorAt(0, 0x4D4D4D);
			linearGradient.setColorAt(1.0, 0x292929);
			//				painter.setBrush(QBrush(linearGradient));

			painter.fillRect(rt, QBrush(linearGradient));
			//				painter.fillRect(rt, cl);
		}
		else
		{
			painter.drawRect(rt.adjusted(0, 0, -1, -1));
		}

		auto mydrawText = [&](const QRect &rt, const int &flag, const QString &strText) {
			if (rt.isValid() && !strText.isEmpty())
			{
				//QFont font;
				////font.setPointSize(Y_SCALE_FONT_SIZE);
				//int w = rt.width() / strText.length();
				//int textPixelSize = qMin(w, rt.height());
				//font.setPixelSize(textPixelSize *0.9);
				//font.setFamily("Microsoft YaHei");
				//font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
				//painter.setFont(font);

				painter.drawText(rt, flag, strText);
			}
		};

		mydrawText(rect(), Qt::AlignHCenter | Qt::AlignVCenter, text());
	}

private:
//    MediaStyle* m_pMediaStyle;
	QImage m_image;
};