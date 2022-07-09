#include "cpc_pageturn.h"
#include <QMessageBox>

cpc_pageturn::cpc_pageturn()
{
}

int cpc_pageturn::pageCount(QTableWidget *p)
{
	if (p == NULL) return -1;
	int rowcount = p->model()->rowCount();
	int rowheight = p->rowHeight(0);
	int tableHeight = p->height();
	int rowCountPerPage = tableHeight / rowheight - 1; //ÿҳ��ʾ����
	int ret = rowcount / rowCountPerPage;
	int tem = rowcount % rowCountPerPage;
	if (tem != 0) ret++;
	return ret;
}

bool cpc_pageturn::pageTo(QTableWidget *p, int pageNo)
{
	if (p == NULL) return false;
	int maxpage = pageCount(p);
	if (pageNo > maxpage) return false;
	int rowcount = p->model()->rowCount();
	int rowheight = p->rowHeight(0);
	int tableHeight = p->height();
	int rowCountPerPage = tableHeight / rowheight - 1; //ÿҳ��ʾ����
	int canNotViewCount = rowcount - rowCountPerPage;  //������������
	if (canNotViewCount == 0) return false;
	int maxValue = p->verticalScrollBar()->maximum();  //��ǰ������ʾ���ֵ
	if (maxValue == 0) return false;
	int pageValue = (maxValue * rowCountPerPage) / canNotViewCount;
	p->verticalScrollBar()->setSliderPosition(pageValue * (pageNo - 1));
	return true;
}

bool cpc_pageturn::pageUp(QTableWidget *p, bool isLoop)
{
	if (p == NULL) return false;
	int rowcount = p->model()->rowCount();
	int rowheight = p->rowHeight(0);
	int tableHeight = p->height();
	int rowCountPerPage = tableHeight / rowheight - 1; //ÿҳ��ʾ����
	int canNotViewCount = rowcount - rowCountPerPage;  //������������
	if (canNotViewCount == 0) return false;
	int maxValue = p->verticalScrollBar()->maximum();  //��ǰ������ʾ���ֵ
	if (maxValue == 0) return false;
	int pageValue = (maxValue * rowCountPerPage) / canNotViewCount;
	int nCurScroller = p->verticalScrollBar()->value();
	if(nCurScroller > 0)
		p->verticalScrollBar()->setSliderPosition(nCurScroller - pageValue);
	else
	{
		QMessageBox::question(NULL, QString::fromLocal8Bit("��ʾ��Ϣ"), QString::fromLocal8Bit("�Ѿ�����ҳ��"), QMessageBox::Ok);
		if(isLoop == true)
			p->verticalScrollBar()->setSliderPosition(maxValue);
	}
	return true;
}

bool cpc_pageturn::pageDown(QTableWidget *p, bool isLoop)
{
	if (p == NULL) return false;
	int rowcount = p->model()->rowCount();
	int rowheight = p->rowHeight(0);
	int tableHeight = p->height();
	int rowCountPerPage = tableHeight / rowheight - 1; //ÿҳ��ʾ����
	int canNotViewCount = rowcount - rowCountPerPage;  //������������
	if (canNotViewCount == 0) return false;
	int maxValue = p->verticalScrollBar()->maximum();  //��ǰ������ʾ���ֵ
	if (maxValue == 0) return false;
	int pageValue = (maxValue * rowCountPerPage) / canNotViewCount;
	int nCurScroller = p->verticalScrollBar()->value();
	if (nCurScroller < maxValue)
		p->verticalScrollBar()->setSliderPosition(nCurScroller + pageValue);
	else
	{
		QMessageBox::question(NULL, QString::fromLocal8Bit("��ʾ��Ϣ"), QString::fromLocal8Bit("�Ѿ���βҳ��"), QMessageBox::Ok);
		if (isLoop == true)
			p->verticalScrollBar()->setSliderPosition(0);
	}
	return true;
}

bool cpc_pageturn::pageHome(QTableWidget *p)
{
	if (p == NULL) return false;
	int maxValue = p->verticalScrollBar()->maximum();  //��ǰ������ʾ���ֵ
	if (maxValue == 0) return false;
	p->verticalScrollBar()->setSliderPosition(0);
	return true;
}

bool cpc_pageturn::pageEnd(QTableWidget *p)
{
	if (p == NULL) return false;
	int maxValue = p->verticalScrollBar()->maximum();  //��ǰ������ʾ���ֵ
	if (maxValue == 0) return false;
	p->verticalScrollBar()->setSliderPosition(maxValue);
	return true;
}