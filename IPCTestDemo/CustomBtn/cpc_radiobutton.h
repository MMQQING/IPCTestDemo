#pragma once
#include <QRadioButton>
#include "cpc_icon.h"
#include "QStyledItemDelegate"
class CPC_RadioButton :public QRadioButton, virtual public CPC_UIBase
{
	Q_OBJECT
public:
	CPC_RadioButton(QWidget *parent = nullptr);
	CPC_RadioButton(int Icon, QString text, QWidget *parent = nullptr);
	~CPC_RadioButton();
private:
	void resizeEvent(QResizeEvent *event);
private:
signals:

public slots :
};