#pragma once
#include <QLineEdit>
#include "cpc_icon.h"
class cpc_lineedit :public QLineEdit, virtual public CPC_UIBase
{
	Q_OBJECT
public:
	cpc_lineedit(QWidget *parent = nullptr);
	cpc_lineedit(int Icon, QString text, QWidget *parent = nullptr);
	~cpc_lineedit();

	void resetIconPoint(float xP = 0, float yp = 0);
	void setIconVisiable(bool visiable);
private:
	void resizeEvent(QResizeEvent *event);
private:
signals:
public slots :
};

