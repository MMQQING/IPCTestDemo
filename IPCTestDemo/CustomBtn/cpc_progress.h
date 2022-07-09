#pragma once
#include <QProgressBar>
#include "cpc_icon.h"
class cpc_progress :public QProgressBar, virtual public CPC_UIBase
{
	Q_OBJECT
public:
	cpc_progress(QWidget *parent = nullptr);
	cpc_progress(int Icon, float min = 0, float max = 100, QWidget *parent = nullptr);
	~cpc_progress();
	void resetIconSize(int width, int height);
	void setIconVisiable(bool visiable);
	void resetIconPoint(float xP = 0, float yp = 0);
	void resetIconValue(int Icon) {};
private:
	void resizeEvent(QResizeEvent *event);
	void UpdatePadding(float w, float h);
private:
signals:
public slots :
	void UpdateIconString(int value);
};

