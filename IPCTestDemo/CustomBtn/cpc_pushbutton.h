#ifndef cpc_pushbutton_H
#define cpc_pushbutton_H

#include <QPushButton>
#include "cpc_icon.h"
class cpc_pushbutton : public QPushButton,virtual public CPC_UIBase
{
    Q_OBJECT
public:
	cpc_pushbutton(QWidget *parent = nullptr);
	cpc_pushbutton(int Icon,QString text,QWidget *parent = nullptr);
	~cpc_pushbutton();
private:
	void resizeEvent(QResizeEvent *event);
private:
signals:

public slots:
};

#endif // cpc_pushbutton_H
