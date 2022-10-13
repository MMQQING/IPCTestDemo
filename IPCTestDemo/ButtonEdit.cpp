#include "ButtonEdit.h"
#include <QHBoxLayout>
ButtonEdit::ButtonEdit(const QString &btnText, QWidget *parent)
	: QLineEdit(parent)
{
	button = new QPushButton(btnText);
	setTextButton();
	addButton();
}

ButtonEdit::ButtonEdit(const QIcon &icon, QWidget *parent)
	: QLineEdit(parent)
{
	button = new QPushButton;
	button->setIcon(icon);
	setIconButton();
	addButton();
}
void ButtonEdit::addButton() {
	connect(button,
		&QPushButton::clicked,
		this,
		&ButtonEdit::buttonClicked);
	// ��ť�Ѿ���edit��һ�����ˣ���Ӧ�����ܱ������۽���������ܵ�����
	button->setFocusPolicy(Qt::NoFocus);
	// ������꣬��������ťʱ��Ȼ����ʾ��������ʱ�����ͼ��
	button->setCursor(Qt::ArrowCursor);

	auto btnLayout = new QHBoxLayout;
	btnLayout->addStretch();
	btnLayout->addWidget(button);
	// ��������Ҷ��룬��ť����ʾ��edit���Ҳ�
	btnLayout->setAlignment(Qt::AlignRight);
	btnLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(btnLayout);
	// ������������ķ�Χ����edit�����󵽰�ť������(�����˰�ť���õ�buttonMargin)
	setTextMargins(0, 0, button->width(), 0);
}
// �������������ð�ť��width����С����Ϊfixed�����ɷŴ����С
static void setButtonSize(QPushButton *button, int width) {
	auto policy = button->sizePolicy();
	policy.setHorizontalPolicy(QSizePolicy::Fixed);
	button->setSizePolicy(policy);
	// �̶���ȣ����ϱ߾�
	button->setFixedWidth(width + buttonMargin * 2);
}

void ButtonEdit::setTextButton() {
	if (!button) {
		return;
	}

	// ��õ�ǰ�������ı����ݵ����ؿ��
	auto width = QWidget::fontMetrics().width(button->text());
	setButtonSize(button, width);
}

void ButtonEdit::setIconButton() {
	if (!button) {
		return;
	}

	// ��ȡͼ���width�򵥵ö�
	auto width = button->iconSize().width();
	setButtonSize(button, width);
	// ���ñ����ͱ߿��ڷǵ��ʱ���ɼ�
	button->setFlat(true);
}