#pragma once
#include <QSlider>
#include "cpc_icon.h"
class CPC_Slider :public QSlider, virtual public CPC_UIBase
{
	Q_OBJECT
public:
	CPC_Slider(QWidget *parent = nullptr);
	CPC_Slider(int Icon, float min = 0, float max = 100, QWidget *parent = nullptr);
	~CPC_Slider();
	void setOrientation(Qt::Orientation);
	void resetIconSize(int width, int height);
	void setIconVisiable(bool visiable);
	void resetIconPoint(float xP = 0, float yp = 0);
	void resetIconValue(int Icon) {};
private:
	void resizeEvent(QResizeEvent *event);
	void UpdatePaddingRelease();
	void UpdatePadding(float w, float h);
private:
	Qt::Orientation m_curOri = Qt::Horizontal;
	float maxw;
signals:
public slots :
	void UpdateIconString(int value);

};

