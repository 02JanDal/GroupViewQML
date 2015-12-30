#include "GroupViewHelper_p.h"

#include <QQuickItem>
#include <QMimeData>
#include <cmath>

GroupViewHelper::GroupViewHelper(QObject *parent)
	: QObject(parent)
{
	connect(this, &GroupViewHelper::currentIndexChanged, this, [this]()
	{
		emit currentObjectChanged(currentObject());
	});
}

QObject *GroupViewHelper::currentObject() const
{
	return m_objects.value(m_currentIndex, nullptr);
}

void GroupViewHelper::setModel(QAbstractItemModel *model)
{
	if (m_model == model) {
		return;
	}

	if (m_model) {
		disconnect(m_model, &GroupViewProxy::rowsAboutToBeInserted, this, &GroupViewHelper::rowsChanged);
		disconnect(m_model, &GroupViewProxy::rowsAboutToBeRemoved, this, &GroupViewHelper::rowsChanged);
		disconnect(m_model, &GroupViewProxy::rowsAboutToBeMoved, this, &GroupViewHelper::rowsChanged);
	}

	setCurrentIndex(QModelIndex());

	m_model = qobject_cast<GroupViewProxy *>(model);
	Q_ASSERT_X(m_model || !model, "GroupViewHelper::setModel", "model needs to be a GroupViewProxyModel (use GroupView::makeProxy)");

	if (m_model) {
		connect(m_model, &GroupViewProxy::rowsAboutToBeInserted, this, &GroupViewHelper::rowsChanged);
		connect(m_model, &GroupViewProxy::rowsAboutToBeRemoved, this, &GroupViewHelper::rowsChanged);
		connect(m_model, &GroupViewProxy::rowsAboutToBeMoved, this, &GroupViewHelper::rowsChanged);
	}

	emit modelChanged(model);
}

void GroupViewHelper::setView(QQuickItem *view)
{
	if (m_view == view) {
		return;
	}

	m_view = view;
	emit viewChanged(view);
}
void GroupViewHelper::setMaxColumns(int maxColumns)
{
	if (m_maxColumns == maxColumns) {
		return;
	}

	m_maxColumns = maxColumns;
	emit maxColumnsChanged(maxColumns);
}
void GroupViewHelper::setCurrentIndex(const QModelIndex &currentIndex)
{
	if (m_currentIndex == currentIndex) {
		return;
	}

	Q_ASSERT(!currentIndex.isValid() || currentIndex.model() == m_model->sourceModel());
	m_currentIndex = currentIndex;
	emit currentIndexChanged(currentIndex);
	emit hasCurrentChanged(hasCurrent());
}

void GroupViewHelper::clicked(const QModelIndex &index)
{
	if (index.isValid()) {
		setCurrentIndex(index);
	}
}

void GroupViewHelper::moveCursor(const CursorMove move)
{
	if (!hasCurrent()) {
		return;
	}
	QModelIndex current = m_currentIndex;
	if (move == Left) {
		while (internalColumn(current) > 0) {
			current = current.sibling(current.row() - 1, 0);
			if (current.flags() & Qt::ItemIsSelectable) {
				setCurrentIndex(current);
				break;
			}
		}
	} else if (move == Right) {
		while (internalColumn(current) < (numSiblingColumns(current) - 1)) {
			current = current.sibling(current.row() + 1, 0);
			if (current.flags() & Qt::ItemIsSelectable) {
				setCurrentIndex(current);
				break;
			}
		}
	} else if (move == Up) {
		while (internalRow(current) > 0 || current.parent().row() > 0) {
			if (internalRow(current) > 0) {
				current = current.sibling(current.row() - m_maxColumns, 0);
				if (current.flags() & Qt::ItemIsSelectable) {
					setCurrentIndex(current);
					break;
				}
			} else {
				QModelIndex newParent = current.parent().sibling(current.parent().row() - 1, 0);
				while (!current.model()->hasChildren(newParent)) {
					newParent = newParent.sibling(newParent.row() - 1, 0);
				}
				const int newParentChildren = newParent.model()->rowCount(newParent);
				const int currentColumn = internalColumn(current);
				const int columns = numSiblingColumns(newParent.child(newParentChildren - 1, 0));
				current = newParent.child(std::min(newParentChildren - (columns - currentColumn),
												  newParentChildren - 1),
										  0);
				if (current.flags() & Qt::ItemIsSelectable) {
					setCurrentIndex(current);
					break;
				}
			}
		}
	} else if (move == Down) {
		while (internalRow(current) < (numSiblingRows(current) - 1) || current.parent().row() < (current.model()->rowCount(current.parent().parent()) - 1)) {
			if (internalRow(current) < (numSiblingRows(current) - 1)) {
				current = current.sibling(std::min(current.row() + m_maxColumns, current.model()->rowCount(current.parent()) - 1), 0);
				if (current.flags() & Qt::ItemIsSelectable) {
					setCurrentIndex(current);
					break;
				}
			} else {
				QModelIndex newParent = current.parent().sibling(current.parent().row() + 1, 0);
				while (!current.model()->hasChildren(newParent)) {
					newParent = newParent.sibling(newParent.row() + 1, 0);
				}
				current = newParent.child(std::min(internalColumn(current),
												  newParent.model()->rowCount(newParent) - 1),
										 0);
				if (current.flags() & Qt::ItemIsSelectable) {
					setCurrentIndex(current);
					break;
				}
			}
		}
	}
}

QVariant GroupViewHelper::dataForIndex(const QModelIndex &index, const QString &property)
{
	if (!index.isValid()) {
		return QVariant();
	}
	QHash<int, QByteArray> roles = index.model()->roleNames();
	return index.data(roles.key(property.toUtf8()));
}

QVariantMap GroupViewHelper::mimeDataForIndex(const QModelIndex &index) const
{
	const QMimeData *data = m_model->mimeData({index});
	QVariantMap out;
	for (const QString &type : data->formats()) {
		out.insert(type, data->data(type));
	}
	return out;
}

void GroupViewHelper::registerObject(const QPersistentModelIndex &index, QObject *obj)
{
	m_objects.insert(index, obj);
}
void GroupViewHelper::unregisterObject(QObject *obj)
{
	m_objects.remove(m_objects.key(obj));
}

void GroupViewHelper::rowsChanged()
{
	emit currentIndexChanged(currentIndex());
	emit hasCurrentChanged(hasCurrent());
}

int GroupViewHelper::internalRow(const QModelIndex &index) const
{
	return std::floor(qreal(index.row()) / qreal(m_maxColumns));
}
int GroupViewHelper::internalColumn(const QModelIndex &index) const
{
	return index.row() % m_maxColumns;
}

int GroupViewHelper::numSiblingColumns(const QModelIndex &index) const
{
	if (!index.isValid()) {
		return 0;
	}
	const int totalSiblings = index.model()->rowCount(index.parent());
	const int numRows = std::ceil(qreal(totalSiblings) / qreal(m_maxColumns));
	if (internalRow(index) < (numRows - 1)) {
		return m_maxColumns;
	} else {
		return internalColumn(index.sibling(totalSiblings - 1, 0)) + 1;
	}
}
int GroupViewHelper::numSiblingRows(const QModelIndex &index) const
{
	if (index.isValid()) {
		return internalRow(index.sibling(index.model()->rowCount(index.parent()) - 1, 1)) + 1;
	} else {
		return 0;
	}
}

