#pragma once
#include <QTableWidget>
#include <QScrollBar>

class cpc_pageturn
{
public:
	cpc_pageturn();
	//总页数
	int pageCount(QTableWidget *p);
	//跳转到指定页
	bool pageTo(QTableWidget *p, int pageNo);
	//上一页
	bool pageUp(QTableWidget *p, bool isLoop);
	//下一页
	bool pageDown(QTableWidget *p, bool isLoop);
	//首页
	bool pageHome(QTableWidget *p);
	//尾页
	bool pageEnd(QTableWidget *p);
	~cpc_pageturn();
};
