#pragma once
#include <QTableWidget>
#include <QScrollBar>

class cpc_pageturn
{
public:
	cpc_pageturn();
	//��ҳ��
	int pageCount(QTableWidget *p);
	//��ת��ָ��ҳ
	bool pageTo(QTableWidget *p, int pageNo);
	//��һҳ
	bool pageUp(QTableWidget *p, bool isLoop);
	//��һҳ
	bool pageDown(QTableWidget *p, bool isLoop);
	//��ҳ
	bool pageHome(QTableWidget *p);
	//βҳ
	bool pageEnd(QTableWidget *p);
	~cpc_pageturn();
};
