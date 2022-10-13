#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QIcon>

class ButtonEdit : public QLineEdit {
	Q_OBJECT
public:
	explicit ButtonEdit(const QString &btnText, QWidget *parent = nullptr);
	explicit ButtonEdit(const QIcon &icon, QWidget *parent = nullptr);
	~ButtonEdit() override = default;

private:
	// �����ı���ť��ͼ�갴ť�Ĵ�С�����
	void setTextButton();
	void setIconButton();
	// ����ť��ӵ�edit
	void addButton();

	QPushButton *button;

Q_SIGNALS:
	void buttonClicked(bool);
};

// ��ť���������ݵı߾�
constexpr int buttonMargin = 3;