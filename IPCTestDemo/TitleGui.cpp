#include "TitleGui.h"
#include "QDesktopWidget"
#include "QMenuBar"

TitleGui::TitleGui(QWidget *parent)
	: QWidget(parent)
{
	m_bmax = true;
	ui.setupUi(this);
	m_location.setWidth(WIN_WIDTH);
	m_location.setHeight(WIN_HEIGHT);
	InitMenu();
	InitConnect();	
}

TitleGui::~TitleGui()
{
}

void TitleGui::InitMenu()
{
	//min max close
	ui.btnMenu_Close->resetIconColor("#FF3838");

	ui.btnMenu_Close->setIconVisiable(true);
	ui.btnMenu_Max->setIconVisiable(true);
	ui.btnMenu_Min->setIconVisiable(true);

	ui.btnMenu_Close->resetIconValue(CLO_ICON);
	ui.btnMenu_Max->resetIconValue(MID_ICON);
	ui.btnMenu_Min->resetIconValue(MIN_ICON);

	ui.btnMenu_Close->resetIconPoint(0.5);
	ui.btnMenu_Max->resetIconPoint(0.5);
	ui.btnMenu_Min->resetIconPoint(0.5);
}

void TitleGui::InitConnect()
{
	connect(ui.btnMenu_Min, &cpc_pushbutton::clicked, this, &TitleGui::on_btnMenu_Min_clicked);
	connect(ui.btnMenu_Max, &cpc_pushbutton::clicked, this, &TitleGui::on_btnMenu_Max_clicked);
	connect(ui.btnMenu_Close, &cpc_pushbutton::clicked, this, &TitleGui::on_btnMenu_Close_clicked);
}

void TitleGui::on_btnMenu_Close_clicked()
{
	qApp->exit();
}

void TitleGui::on_btnMenu_Max_clicked()
{
	if (m_bmax)
	{
		QRect rec = overspreadScreen();
		int oldw = WIN_WIDTH;
		int oldh = WIN_HEIGHT;
		m_location.setX(rec.x()+(rec.width() - oldw)/2.0);
		m_location.setY(rec.y() + (rec.height() - oldh)/2.0);
		m_location.setWidth(oldw);
		m_location.setHeight(oldh);
		((QWidget*)parent())->setGeometry(m_location);

		ui.btnMenu_Max->resetIconValue(MAX_ICON);
		ui.btnMenu_Max->setToolTip(QStringLiteral("最大化"));
	}
	else
	{
		m_location = ((QWidget*)parent())->geometry();
		((QWidget*)parent())->setGeometry(overspreadScreen());
		ui.btnMenu_Max->resetIconValue(MID_ICON);
		ui.btnMenu_Max->setToolTip(QStringLiteral("还原"));
	}
	m_bmax = !m_bmax;
}

void TitleGui::on_btnMenu_Min_clicked()
{
	((QWidget*)parent())->showMinimized();
}

QRect TitleGui::overspreadScreen()
{
	QDesktopWidget * desktop = QApplication::desktop();
	int current_screen = desktop->screenNumber(((QWidget*)parent()));
	QRect rect = desktop->availableGeometry(current_screen);
	return rect;
}

void TitleGui::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		m_mousePressed = true;
		m_mousePoint = e->globalPos() - ((QWidget*)parent())->pos();
		e->accept();
	}
}
void TitleGui::mouseMoveEvent(QMouseEvent *e)
{
	if (m_mousePressed && (e->buttons() && Qt::LeftButton) && !m_bmax)
	{
		((QWidget*)parent())->move(e->globalPos() - m_mousePoint);
		e->accept();
	}
}

void TitleGui::mouseReleaseEvent(QMouseEvent *)
{
	m_mousePressed = false;
}

void TitleGui::showEvent(QShowEvent *event)
{
	if (this->isVisible())
	{
		if (!m_bmax) {
			QRect rec = overspreadScreen();
			int oldw = WIN_WIDTH;
			int oldh = WIN_HEIGHT;
			m_location.setX(rec.x() + (rec.width() - oldw) / 2.0);
			m_location.setY(rec.y() + (rec.height() - oldh) / 2.0);
			m_location.setWidth(oldw);
			m_location.setHeight(oldh);
			((QWidget*)parent())->setGeometry(m_location);

			ui.btnMenu_Max->resetIconValue(MAX_ICON);
			ui.btnMenu_Max->setToolTip(QStringLiteral("最大化"));
		}
	}
}

void TitleGui::mouseDoubleClickEvent(QMouseEvent *event)
{
	on_btnMenu_Max_clicked();
}

void TitleGui::addWidget(QWidget* widget, layoutChoose lr)
{
	if (lr == leftLayout)
	{
		ui.leftLayout_widget->addWidget(static_cast<QWidget*>(widget));
	}
	else
	{
		ui.rightLayout_widget->addWidget(static_cast<QWidget*>(widget));
	}
	
}
