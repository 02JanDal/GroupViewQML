#pragma once

#include <QAbstractItemModel>

class InstanceModel : public QAbstractItemModel
{
public:
	InstanceModel(QObject *parent = nullptr);

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &child) const override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const override;

	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;

	QStringList mimeTypes() const override;
	QMimeData *mimeData(const QModelIndexList &indexes) const override;
	bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
	Qt::DropActions supportedDropActions() const override;
	Qt::DropActions supportedDragActions() const override;

private:
	struct Item
	{
		explicit Item(Item *p = nullptr) : parent(p) {}
		~Item();
		QHash<int, QVariant> values;
		Item *parent = nullptr;
		QVector<Item *> items;
	};
	Item *m_root = new Item;

	QModelIndex indexFor(Item * const item) const;
};
