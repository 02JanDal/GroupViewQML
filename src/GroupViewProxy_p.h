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
