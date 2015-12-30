#include "GroupViewDropArea_p.h"

#include <QMimeData>

#include "GroupViewHelper_p.h"

class MimeDataSourceStealer : public QMimeData
{
	Q_OBJECT
public:
	QObject *source() const { return m_source; }
private:
	QObject *m_source;
	Qt::DropActions m_supportedActions;
	QStringList m_keys;
};
QObject *sourceInMimeData(const QMimeData *data)
{
	if (data->inherits("QQuickDragMimeData")) {
		return reinterpret_cast<const MimeDataSourceStealer *>(data)->source();
	} else {
		return nullptr;
	}
}

GroupViewDropArea::GroupViewDropArea(QQuickItem *parent)
	: QQuickItem(parent)
{
	setFlags(ItemAcceptsDrops);
}

void GroupViewDropArea::setHelper(GroupViewHelper *helper)
{
	if (m_helper == helper) {
		return;
	}

	m_helper = helper;
	emit helperChanged(helper);
}
void GroupViewDropArea::setDropTargetIndex(QModelIndex dropTargetIndex)
{
	if (m_dropTargetIndex == dropTargetIndex) {
		return;
	}

	m_dropTargetIndex = dropTargetIndex;
	emit dropTargetIndexChanged(dropTargetIndex);
}
void GroupViewDropArea::setShowIndicator(bool showIndicator, QObject *nextTo)
{
	if (m_showIndicator == showIndicator && m_showIndicatorNextTo == nextTo) {
		return;
	}

	m_showIndicator = showIndicator;
	m_showIndicatorNextTo = nextTo;
	emit showIndicatorChanged(showIndicator);
	emit showIndicatorNextToChanged(nextTo);
}

void GroupViewDropArea::dragEnterEvent(QDragEnterEvent *event)
{
	setShowIndicator(false);
	if (canDrop(event)) {
		event->acceptProposedAction();
		setContainsDrag(true);
	} else {
		event->ignore();
		setContainsDrag(false);
	}
}
void GroupViewDropArea::dragMoveEvent(QDragMoveEvent *event)
{
	if (canDrop(event)) {
		event->acceptProposedAction();
		setContainsDrag(true);

		QQuickItem *item;
		QModelIndex index;
		itemAt(event, &item, index);
		qDebug() << item;
		setShowIndicator(true, item);
	} else {
		event->ignore();
		setContainsDrag(false);
		setShowIndicator(false);
	}
}
void GroupViewDropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
	setContainsDrag(false);
}

void GroupViewDropArea::dropEvent(QDropEvent *event)
{
	setContainsDrag(false);
}

void GroupViewDropArea::setContainsDrag(const bool containsDrag)
{
	if (m_containsDrag == containsDrag) {
		return;
	}
	m_containsDrag = containsDrag;
	emit containsDragChanged(containsDrag);
}

void GroupViewDropArea::itemAt(QDropEvent *event, int &row, int &col, QModelIndex &parent) const
{
	QQuickItem *item;
	QModelIndex index;
	itemAt(event, &item, index);
	if (item) {
		row = index.row();
		col = index.column();
		parent = index.parent();
	} else {
		row = -1;
		col = -1;
		parent = m_dropTargetIndex;
	}
}
void GroupViewDropArea::itemAt(QDropEvent *event, QQuickItem **item, QModelIndex &index) const
{
	const QHash<QPersistentModelIndex, QObject *> objects = m_helper->objects();
	for (QObject *obj : objects.values()) {
		QQuickItem *it = qobject_cast<QQuickItem *>(obj);
		if (it->contains(mapToItem(it, event->posF()))) {
			*item = it;
			index = objects.key(obj);
			return;
		}
	}
	*item = nullptr;
}

bool GroupViewDropArea::canDrop(QDropEvent *event) const
{
	if (event->dropAction() & m_helper->model()->supportedDropActions()) {
		const QObject *source = sourceInMimeData(event->mimeData());
		if (source != nullptr && source->property("modelIndex").toModelIndex().isValid()) {
			return true;
		}

		bool found = false;
		for (const QString &type : m_helper->model()->mimeTypes()) {
			if (event->mimeData()->hasFormat(type)) {
				found = true;
				break;
			}
		}
		if (found) {
			int row, col;
			QModelIndex parent;
			itemAt(event, row, col, parent);
			return m_helper->model()->canDropMimeData(event->mimeData(), event->dropAction(), row, col, parent);
		}
	}
	return false;
}

#include "GroupViewDropArea.moc"
