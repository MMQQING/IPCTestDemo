#pragma once

#include <QWidget>
#include <QTimer>
#include "QStyledItemDelegate"
class CPC_Switch : public QWidget
{
	Q_OBJECT

public:
	CPC_Switch(QWidget *parent = Q_NULLPTR);
	~CPC_Switch();
	//��ȡ��ǰ״̬
	bool isChecked() const;
	//���õ�ǰ״̬
	void setChecked(bool checked);
	//���ùر���ɫ��������ɫ
	void setColor(QColor backColor, QColor checkColor);
	//���ñ߾�
	void setMargin(float margin);
	//Բ�ǰ뾶��(height-2*margin)/2�ı��� ���Ϊ1
	void setRadius(float radius);
protected:
	void paintEvent(QPaintEvent*event);
	void mousePressEvent(QMouseEvent*event);
	void mouseReleaseEvent(QMouseEvent*event);
	void resizeEvent(QResizeEvent*event);
	QSize sizeHint()const Q_DECL_OVERRIDE;
	QSize minimumSizeHint()const Q_DECL_OVERRIDE;
signals:
	void onChecke(bool checked);
private slots:
	void onTimeOut();
private:
	bool m_bChecked =false;
	QColor m_background = QColor("#8080ffff");
	QColor m_thumbCOlor = QColor("#505050");
	QColor m_checkBGColor = QColor("#80ffff");
	QColor m_checkedColor = QColor("#80ffff");
	QColor m_disableColor = QColor(50, 50, 50);

	qreal m_radius = 0.5;
	qreal m_nX = 0;
	int16_t m_nHeight = 16;
	int16_t m_nMargin = 0;
	QTimer m_timer;

	int transTime = 100;
};

class CPC_switchDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	CPC_switchDelegate(QObject *parent = 0) : QStyledItemDelegate(parent) { }
	QWidget *createEditor(QWidget*parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const
	{
		CPC_Switch *editor = new CPC_Switch(parent);
		editor->setColor(QColor("#808080"), QColor("#128888"));
		return editor;
	}
	void setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		QString value = index.model()->data(index, Qt::DisplayRole).toString();
		CPC_Switch *item = static_cast<CPC_Switch*>(editor);
		item->setEnabled(value.toInt() != 0 ? 1 : 0);
	}
	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const
	{
		CPC_Switch *item = static_cast<CPC_Switch*>(editor);
		model->setData(index, item->isChecked() ? 1 : 0, Qt::DisplayRole);
	}
	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
	{
		editor->setGeometry(option.rect);
	}
};