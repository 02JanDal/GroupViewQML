#pragma once

#include <QIdentityProxyModel>

class GroupViewProxy : public QIdentityProxyModel
{
	Q_OBJECT
	Q_PROPERTY(QModelIndex rootIndex READ rootIndex WRITE setRootIndex NOTIFY rootIndexChanged)

public:
	GroupViewProxy(QObject *parent = nullptr);

	QPersistentModelIndex rootIndex() const { return m_rootIndex; }
	void setRootIndex(const QPersistentModelIndex &rootIndex);

	void setSourceModel(QAbstractItemModel *model) override;

	enum
	{
		ChildrenListRole = Qt::UserRole + 42,
		ModelIndexRole
	};

	QVariant data(const QModelIndex &index, int role) const override;
	QHash<int, QByteArray> roleNames() const override;

	QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
	QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;

signals:
	void rootIndexChanged(const QPersistentModelIndex &rootIndex);

private:
	QPersistentModelIndex m_rootIndex;
	QHash<QPersistentModelIndex, GroupViewProxy *> m_proxies;

	void handleRowsRemoved(const QModelIndex &parent, const int first, const int last);
};
