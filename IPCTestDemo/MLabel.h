#pragma once
#include <QLabel>
class MLabel:public QLabel
{
	Q_OBJECT
public:
	MLabel(QWidget *parent = nullptr);
	~MLabel();
public slots:
	void setText(const QString &);
	
protected:
	void timerEvent(QTimerEvent *e);
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	void upateLabelRollingState();
private:
	int left;
	int timerId;
	int fontSize;
};

