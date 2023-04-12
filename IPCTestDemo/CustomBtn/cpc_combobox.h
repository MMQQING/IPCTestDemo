#pragma once
#include <QComboBox>
#include <QMouseEvent>
//#include "cpc_icon.h"
class cpc_combobox :public QComboBox/*, virtual public CPC_UIBase*/
{
	Q_OBJECT
public:
	cpc_combobox(QWidget *parent = nullptr);
	//cpc_combobox(int Icon, QString text, QWidget *parent = nullptr);
	~cpc_combobox();
private:
	void resizeEvent(QResizeEvent *event);

protected:
	virtual void mousePressEvent(QMouseEvent *e);  //添加鼠标点击事件
signals:
	void clicked();  //自定义点击信号，在mousePressEvent事件发生时触发
public slots :
};

