#pragma once
#include <QLabel>
#include "cpc_icon.h"
#include "QStyledItemDelegate"
class cpc_label :public QLabel, virtual public CPC_UIBase
{
	Q_OBJECT
public:
	cpc_label(QWidget *parent = nullptr);
	cpc_label(int Icon, QString text, QWidget *parent = nullptr);
	~cpc_label();
private:
	void resizeEvent(QResizeEvent *event);
private:
signals:

public slots :
};

class CPC_LabelDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	CPC_LabelDelegate(QObject *parent = 0) : QStyledItemDelegate(parent) { }
	QWidget *createEditor(QWidget*parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const
	{
		cpc_label *editor = new cpc_label(0xe673, QStringLiteral("³ÊÏÖÉÏÉýÇ÷ÊÆ"));;
		return editor;
	}
	void setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		QString value = index.model()->data(index, Qt::EditRole).toString();
		cpc_label *item = static_cast<cpc_label*>(editor);
		item->setText(value);
	}
	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const
	{
		cpc_label *item = static_cast<cpc_label*>(editor);
		model->setData(index, item->text(), Qt::EditRole);
	}
	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &/* index */)
		const
	{
		editor->setGeometry(option.rect);
	}
};