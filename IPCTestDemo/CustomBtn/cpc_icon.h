#ifndef CPC_ICON_H
#define CPC_ICON_H

#include "uibase.h"
#include <QLabel>
class CPC_Icon : public QLabel,virtual public CPC_UIBase
{
    Q_OBJECT
public:
    explicit CPC_Icon(QWidget *parent = nullptr);
    void setIconValue(int iconValue);
	void setIconString(QString iconString);
	void setIconColor(QString color);
    void seticonSize(int w, int h);
	void resetIconPoint(float xP /*= 0*/, float yp /*= 0*/) {}; //此类无需实现此需函数
	void setVisible(bool show);
	bool isShow();
private:
	void resizeEvent(QResizeEvent *event);
private:
	int m_iconValue;
	QString m_iconString;
	QString m_color;
	bool m_isShow;
signals:
	void IconSizeChanged(float w,float h);
public slots:
};

#endif // CPC_ICON_H
