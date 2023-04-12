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
	virtual void mousePressEvent(QMouseEvent *e);  //���������¼�
signals:
	void clicked();  //�Զ������źţ���mousePressEvent�¼�����ʱ����
public slots :
};

