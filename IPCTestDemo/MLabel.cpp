#include "MLabel.h"
#include <QPainter>
#include <QTimer>
#include <QFontMetrics>
#include <QTimerEvent>

const QString strSpace("   ");

MLabel::MLabel(QWidget *parent /*= nullptr*/) :QLabel(parent)
{
	//setStyleSheet(QString("color:red;"));

	timerId = -1;
	//fontSize = 28;
}

MLabel::~MLabel()
{

}

void MLabel::setText(const QString & txt)
{
	QLabel::setText(txt);

	upateLabelRollingState();
}

void MLabel::upateLabelRollingState()
{
	//  ��ȡ�ı���С��С���ı��򳤶ȣ����������
	QFont ft = font();
	//ft.setPointSize(fontSize);

	QFontMetrics fm(ft);
	int nW = fm.width(text());

	left = 0;
	//  �����ı������
	if (nW > width())
	{
		timerId = startTimer(100);
	}
	//  �ر��ı������
	else
	{
		if (timerId >= 0)
		{
			killTimer(timerId);
			timerId = -1;
		}
	}
}

void MLabel::timerEvent(QTimerEvent *e)
{
	if (e->timerId() == timerId && isVisible())
	{
		//  ÿ������1������
		left += 2;

		//  �ж��Ƿ��Ѿ����һ��ѭ���������ָ���ʼλ�ã����¿�ʼѭ��
		QFont ft = font();
		//ft.setPointSize(fontSize);
		QFontMetrics fm(ft);
		int txtWidth = fm.width(text());
		int spaceWidth = fm.width(strSpace);

		if ((txtWidth + spaceWidth) < left)
			left = 0;

		repaint();
	}

	QLabel::timerEvent(e);
}

void MLabel::paintEvent(QPaintEvent *e)
{
	QPainter p(this);

	//  ��ȡ�ı���Ĵ�С
	QRect rc = rect();
	rc.setHeight(rc.height() - 2);
	rc.setWidth(rc.width() - 2);

	//  ���ü����������ֵ�����
	QFont ft = font();
	//ft.setPointSize(fontSize);
	p.setFont(ft);
	//p.setPen(QPen(Qt::red));

	//  ���û������ֵĿ�ʼλ�ã�Ҳ���ǽ����������ƶ�����
	rc.setLeft(rc.left() - left);

	//  ��������Ѿ���ʾ��ĩβ���������һ�����֣�����ѭ��������Ч��
	QString strText = text();
	if (timerId >= 0)
		strText += strSpace + text();

	//  ��������
	p.drawText(rc, Qt::AlignVCenter, strText);
}

void MLabel::resizeEvent(QResizeEvent *e)
{
	QLabel::resizeEvent(e);

	upateLabelRollingState();
}
