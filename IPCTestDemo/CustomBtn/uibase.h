#ifndef UIBASE_H
#define UIBASE_H
#include <QFont>
#include <QString>
#include <QDebug>
#include <QColor>
#include <QWidget>
class CPC_Icon;
class CPC_UIBase
{
public:
    CPC_UIBase(QWidget* pwidget =nullptr,QWidget*parent = nullptr);
    virtual ~CPC_UIBase();
    virtual void setFontSize(int size);
    virtual void setFontFamily(QString family);

	virtual void resetIconSize(int width, int height);
	virtual void resetIconValue(int Icon);
	virtual void resetIconString(QString Icon);
	virtual void resetIconPoint(float xP = 0, float yp = 0);
	virtual void resetIconColor(QString color);
	virtual void setIconVisiable(bool visiable);

protected:
    int m_fontSize;
    QString m_fontFamily;
    QFont m_font;
    QColor m_fontColor;
    QWidget* m_widget = nullptr;
	QWidget* m_parent = nullptr;
	//icon
	CPC_Icon *m_icon = nullptr;
	float IconPoint_x = 0;
	float IconPoint_y = 0;

};

#endif // UIBASE_H
