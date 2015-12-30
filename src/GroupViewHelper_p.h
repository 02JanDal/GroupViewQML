#pragma once

#include <QObject>
#include <QAbstractItemModel>
#include <QModelIndex>

#include "GroupViewProxy_p.h"

class QQuickItem;

class GroupViewHelper : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged)
	Q_PROPERTY(QQuickItem *view READ view WRITE setView NOTIFY viewChanged)
	Q_PROPERTY(int maxColumns READ maxColumns WRITE setMaxColumns NOTIFY maxColumnsChanged)
	Q_PROPERTY(QModelIndex currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
	Q_PROPERTY(QObject *currentObject READ currentObject NOTIFY currentObjectChanged)
	Q_PROPERTY(bool hasCurrent READ hasCurrent NOTIFY hasCurrentChanged STORED false)

public:
	explicit GroupViewHelper(QObject *parent = 0);

	QAbstractItemModel *model() const { return m_model; }
	QQuickItem *view() const { return m_view; }
	int maxColumns() const { return m_maxColumns; }
	QModelIndex currentIndex() const { return m_currentIndex; }
	QObject *currentObject() const;
	bool hasCurrent() const { return m_currentIndex.isValid(); }

	QHash<QPersistentModelIndex, QObject *> objects() const { return m_objects; }

	enum CursorMove
	{
		Left,
		Right,
		Up,
		Down
	};
	Q_ENUM(CursorMove);

signals:
	void modelChanged(QAbstractItemModel *model);
	void viewChanged(QQuickItem *view);
	void maxColumnsChanged(int maxColumns);
	void currentIndexChanged(const QModelIndex &currentIndex);
	void currentObjectChanged(QObject *currentObject);
	void hasCurrentChanged(bool hasCurrent);

public slots:
	void setModel(QAbstractItemModel *model);
	void setView(QQuickItem *view);
	void setMaxColumns(int maxColumns);
	void setCurrentIndex(const QModelIndex &currentIndex);

	void clicked(const QModelIndex &index);
	void moveCursor(const CursorMove move);

	QVariant dataForIndex(const QModelIndex &index, const QString &property = "display");

	QVariantMap mimeDataForIndex(const QModelIndex &index) const;

	void registerObject(const QPersistentModelIndex &index, QObject *obj);
	void unregisterObject(QObject *obj);

private slots:
	void rowsChanged();

private:
	GroupViewProxy *m_model = nullptr;
	QQuickItem *m_view;
	int m_maxColumns;
	QPersistentModelIndex m_currentIndex;
	QHash<QPersistentModelIndex, QObject *> m_objects;

private:
	int internalRow(const QModelIndex &index) const;
	int internalColumn(const QModelIndex &index) const;
	int numSiblingColumns(const QModelIndex &index) const;
	int numSiblingRows(const QModelIndex &index) const;
};
