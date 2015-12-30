/* Copyright 2015 Jan Dalheimer <jan@dalheimer.de>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "InstanceModel.h"

#include <QColor>
#include <QMimeData>

InstanceModel::InstanceModel(QObject *parent)
	: QAbstractItemModel(parent)
{
	insertRows(0, 5, QModelIndex());
	const QModelIndex devices = index(0, 0);
	const QModelIndex settings = index(1, 0);
	const QModelIndex applications = index(2, 0);
	const QModelIndex mimetypes = index(3, 0);
	const QModelIndex python = index(4, 0);
	setData(devices, "Devices", Qt::DisplayRole);
	setData(settings, "Settings", Qt::DisplayRole);
	setData(applications, "Applications", Qt::DisplayRole);
	setData(mimetypes, "Mime types", Qt::DisplayRole);
	setData(python, "Silly", Qt::DisplayRole);

	auto add = [this](const QModelIndex &parent, const QString &name, const QString &icon = QString())
	{
		insertRow(rowCount(parent), parent);
		const QModelIndex i = index(rowCount(parent)-1, 0, parent);
		setData(i, name, Qt::DisplayRole);
		setData(i, "qrc:/images/icons/" + (icon.isNull() ? name.toLower().remove(' ') + ".png" : icon), Qt::DecorationRole);
	};

	add(devices, "Audio Card");
	add(devices, "Audio Headphones");
	add(devices, "Camera");
	add(devices, "Desktop");
	add(devices, "Laptop");
	add(devices, "Harddisk");
	add(devices, "Optical Drive");
	add(devices, "USB Pendrive");
	add(devices, "Removable Media");
	add(devices, "Keyboard");
	add(devices, "Mouse");
	add(devices, "Flash Memory Stick");
	add(devices, "Flash SD MMC");
	add(devices, "Flash Smart Media");
	add(devices, "Floppy");
	add(devices, "Blu-Ray");
	add(devices, "DVD");
	add(devices, "CD");
	add(devices, "Network Wired");
	add(devices, "Network Wireless");
	add(devices, "Printer");
	add(devices, "Smartphone");
	add(devices, "Tablet");
	add(devices, "Display");

	add(settings, "General");
	add(settings, "Network");
	add(settings, "Appearance");
	add(settings, "System");

	add(applications, "Engineering");
	add(applications, "Graphics");
	add(applications, "Internet");
	add(applications, "Multimedia");
	add(applications, "Office");
	add(applications, "Other");
	add(applications, "Science");

	add(mimetypes, "EPub");
	add(mimetypes, "Google Earth");
	add(mimetypes, "Illustrator");
	add(mimetypes, "Javascript");
	add(mimetypes, "MS Access");
	add(mimetypes, "MS Excel");
	add(mimetypes, "MS Powerpoint");
	add(mimetypes, "MS Word");
	add(mimetypes, "MS Word Template");
	add(mimetypes, "OpenDocument Chart");
	add(mimetypes, "OpenDocument Database");
	add(mimetypes, "OpenDocument Formula");
	add(mimetypes, "OpenDocument Formula Template");
	add(mimetypes, "OpenDocument Graphics");
	add(mimetypes, "OpenDocument Image");
	add(mimetypes, "OpenDocument Presentation");
	add(mimetypes, "OpenDocument Presentation Template");
	add(mimetypes, "OpenDocument Spreadsheet");
	add(mimetypes, "OpenDocument Spreadsheet Template");
	add(mimetypes, "OpenDocument Text");
	add(mimetypes, "PDF");
	add(mimetypes, "PGP Encrypted");
	add(mimetypes, "PGP Keys");
	add(mimetypes, "PostScript");
	add(mimetypes, "RSS");
	add(mimetypes, "RTF");
	add(mimetypes, "Scribus");

	add(python, "My", "my.jpg");
	add(python, "hovercraft", "hovercraft.jpg");
	add(python, "is", "is.jpg");
	add(python, "full", "full.jpg");
	add(python, "of", "of.jpg");
	add(python, "eels", "eels.jpg");
}

QModelIndex InstanceModel::index(int row, int column, const QModelIndex &parent) const
{
	Item *parentItem = parent.isValid() ? static_cast<Item *>(parent.internalPointer()) : m_root;
	return createIndex(row, column, parentItem->items.at(row));
}
QModelIndex InstanceModel::parent(const QModelIndex &child) const
{
	if (!child.isValid()) {
		return QModelIndex();
	}
	Item *item = static_cast<Item *>(child.internalPointer())->parent;
	if (!item || item == m_root) {
		return QModelIndex();
	} else {
		return createIndex(item->parent->items.indexOf(item), 0, item);
	}
}

int InstanceModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid()) {
		return static_cast<Item *>(parent.internalPointer())->items.size();
	} else {
		return m_root->items.size();
	}
}
int InstanceModel::columnCount(const QModelIndex &) const
{
	return 1;
}

QVariant InstanceModel::data(const QModelIndex &index, int role) const
{
	Item *item = static_cast<Item *>(index.internalPointer());
	if (!item) {
		return QVariant();
	}
	return item->values.value(role);
}
bool InstanceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	static_cast<Item *>(index.internalPointer())->values[role] = value;
	emit dataChanged(index, index, QVector<int>() << role);
	return true;
}

Qt::ItemFlags InstanceModel::flags(const QModelIndex &index) const
{
	if (index.parent().isValid()) {
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
	} else {
		return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
	}
}

bool InstanceModel::insertRows(int row, int count, const QModelIndex &parent)
{
	Item *parentItem = parent.isValid() ? static_cast<Item *>(parent.internalPointer()) : m_root;
	beginInsertRows(parent, row, row + count);
	for (int i = 0; i < count; ++i) {
		parentItem->items.insert(row, new Item(parentItem));
	}
	endInsertRows();
	return true;
}
bool InstanceModel::removeRows(int row, int count, const QModelIndex &parent)
{
	Item *parentItem = parent.isValid() ? static_cast<Item *>(parent.internalPointer()) : m_root;
	beginRemoveRows(parent, row, row + count);
	for (int i = 0; i < count; ++i) {
		delete parentItem->items.at(row + i);
	}
	parentItem->items.remove(row, count);
	endRemoveRows();
	return true;
}
bool InstanceModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
	Item *sourceParentItem = sourceParent.isValid() ? static_cast<Item *>(sourceParent.internalPointer()) : m_root;
	Item *destParentItem = destinationParent.isValid() ? static_cast<Item *>(destinationParent.internalPointer()) : m_root;
	beginMoveRows(sourceParent, sourceRow, sourceRow + count, destinationParent, destinationChild);
	for (int i = 0; i < count; ++i) {
		Item *item = sourceParentItem->items.at(sourceRow + i);
		sourceParentItem->items.remove(sourceRow + i);
		item->parent = destParentItem;
		destParentItem->items.insert(destinationChild + i, item);
	}
	endMoveRows();
	return true;
}

InstanceModel::Item::~Item()
{
	qDeleteAll(items);
}

QStringList InstanceModel::mimeTypes() const
{
	return {"application/x-instance"};
}
QMimeData *InstanceModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *data = new QMimeData;
	QStringList items;
	for (const QModelIndex &index : indexes) {
		items.append(index.data().toString());
	}
	data->setData("application/x-instance", items.join(":").toUtf8());

	return data;
}

bool InstanceModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
	if (!QAbstractItemModel::canDropMimeData(data, action, row, column, parent)) {
		return false;
	} else {
		return true;
	}
}
bool InstanceModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	QModelIndex newParent = parent.isValid() ? parent : index(row, column);
	QModelIndexList list;
	const QStringList names = QString::fromUtf8(data->data("application/x-instance")).split(":");
	for (const QString &name : names) {
		for (Item * const item : m_root->items) {
			for (Item * const child : item->items) {
				if (child->values.value(Qt::DisplayRole).toString() == name) {
					list.append(indexFor(child));
				}
			}
		}
	}
	if (action == Qt::MoveAction) {
		for (const QModelIndex &index : list) {
			moveRow(index.parent(), index.row(), newParent, row == -1 ? rowCount(newParent) : row);
		}
		return true;
	} else {
		return false;
	}
}

Qt::DropActions InstanceModel::supportedDropActions() const
{
	return Qt::MoveAction;
}
Qt::DropActions InstanceModel::supportedDragActions() const
{
	return Qt::MoveAction;
}

QModelIndex InstanceModel::indexFor(InstanceModel::Item * const item) const
{
	if (item == m_root) {
		return QModelIndex();
	} else {
		return createIndex(item->parent->items.indexOf(item), 0, item);
	}
}
