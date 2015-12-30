#pragma once

#include <QQuickItem>
#include <QModelIndex>

class GroupViewHelper;

class GroupViewDropArea : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(GroupViewHelper *groupViewHelper READ helper WRITE setHelper NOTIFY helperChanged)
	Q_PROPERTY(bool containsDrag READ containsDrag NOTIFY containsDragChanged)
	Q_PROPERTY(QModelIndex dropTargetIndex READ dropTargetIndex WRITE setDropTargetIndex NOTIFY dropTargetIndexChanged)
	Q_PROPERTY(QObject *showIndicatorNextTo READ showIndicatorNextTo NOTIFY showIndicatorNextToChanged)
	Q_PROPERTY(bool showIndicator READ showIndicator WRITE setShowIndicator NOTIFY showIndicatorChanged)

public:
	explicit GroupViewDropArea(QQuickItem *parent = nullptr);

	GroupViewHelper *helper() const { return m_helper; }
	void setHelper(GroupViewHelper *helper);

	bool containsDrag() const { return m_containsDrag; }
	QModelIndex dropTargetIndex() const { return m_dropTargetIndex; }

	QObject *showIndicatorNextTo() const { return m_showIndicatorNextTo; }
	bool showIndicator() const { return m_showIndicator; }

public slots:
	void setDropTargetIndex(QModelIndex dropTargetIndex);

signals:
	void helperChanged(GroupViewHelper *helper);
	void containsDragChanged(const bool containsDrag);
	void dropTargetIndexChanged(QModelIndex dropTargetIndex);
	void showIndicatorNextToChanged(QObject *showIndicatorNextTo);
	void showIndicatorChanged(bool showIndicator);

protected:
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dragLeaveEvent(QDragLeaveEvent *event) override;
	void dropEvent(QDropEvent *event) override;

private:
	GroupViewHelper *m_helper = nullptr;
	bool m_containsDrag = false;
	QModelIndex m_dropTargetIndex;
	QObject * m_showIndicatorNextTo = nullptr;
	bool m_showIndicator;

	void setContainsDrag(const bool containsDrag);
	void setShowIndicator(bool showIndicator, QObject *nextTo = nullptr);

	void itemAt(QDropEvent *event, int &row, int &col, QModelIndex &parent) const;
	void itemAt(QDropEvent *event, QQuickItem **item, QModelIndex &index) const;
	bool canDrop(QDropEvent *event) const;
};
