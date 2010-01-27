/*
Copyright (c) 2010, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "ObjectListModel.hpp"
#include "EditorObject.hpp"

ObjectListModel::ObjectListModel(QObject *parent)
: QAbstractItemModel(parent), project(0), enabled(true)
{
	open.addFile(QString::fromUtf8(":/icons/document-open.png"), QSize(), QIcon::Normal, QIcon::Off);
	reload.addFile(QString::fromUtf8(":/icons/view-refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
}
ObjectListModel::~ObjectListModel()
{
}

QVariant ObjectListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	EditorObject *item = static_cast<EditorObject*>(index.internalPointer());

	if (role == Qt::DecorationRole)
	{
		if (index.column() == 0 && item->isOpen())
			return open;
		if (index.column() == 1 && item->needsRecompile())
			return reload;
	}
	else if (role == Qt::DisplayRole)
	{
		if (index.column() == 2)
			return item->getName();
		if (index.column() == 3)
		{
			switch (item->getType())
			{
				case EEOT_Entity:
					return "Entity";
				case EEOT_CodeComponent:
					return "CodeComponent";
				case EEOT_ScriptComponent:
					return "ScriptComponent";
				case EEOT_GraphicsComponent:
					return "GraphicsComponent";
				case EEOT_PhysicsComponent:
					return "PhysicsComponent";
				case EEOT_GUILayout:
					return "GUILayout";
				case EEOT_World:
					return "World";
			}
		}
	}
	return QVariant();
}
Qt::ItemFlags ObjectListModel::flags(const QModelIndex &index) const
{
	if (!enabled)
		return 0;
	if (!index.isValid())
		return Qt::ItemIsEnabled;
	if (index.column() < 2)
		return Qt::ItemIsEnabled;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
QVariant ObjectListModel::headerData(int section, Qt::Orientation orientation,
	int role) const
{
	if (role == Qt::DisplayRole && section == 2)
		return "Name";
	if (role == Qt::DisplayRole && section == 3)
		return "Type";
	if (role == Qt::DecorationRole && section == 0)
		return open;
	if (role == Qt::DecorationRole && section == 1)
		return reload;

	return QVariant();
}
QModelIndex ObjectListModel::index(int row, int column,
	const QModelIndex &parent) const
{
	if (row < objects.size())
		return createIndex(row, column, objects[row]);
	else
		return QModelIndex();
}
QModelIndex ObjectListModel::parent(const QModelIndex &index) const
{
	return QModelIndex();
}
int ObjectListModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;

	return objects.size();
}
int ObjectListModel::columnCount(const QModelIndex &parent) const
{
	return 4;
}

void ObjectListModel::addEditorObject(EditorObject *obj)
{
	beginInsertRows(QModelIndex(), objects.size(), objects.size());
	objects.append(obj);
	endInsertRows();
}
void ObjectListModel::removeEditorObject(EditorObject *obj)
{
	int index = objects.indexOf(obj);
	beginRemoveRows(QModelIndex(), index, index);
	objects.removeOne(obj);
	endRemoveRows();
}
void ObjectListModel::updateObject(EditorObject *obj)
{
	int index = objects.indexOf(obj);
	dataChanged(createIndex(index, 0, obj), createIndex(index, 3, obj));
	// TODO
}
EditorObject *ObjectListModel::getObject(const QModelIndex &index)
{
	return static_cast<EditorObject*>(index.internalPointer());
}

void ObjectListModel::clear()
{
	objects.clear();
	reset();
}
void ObjectListModel::setEnabled(bool enabled)
{
	this->enabled = enabled;
	reset();
}
