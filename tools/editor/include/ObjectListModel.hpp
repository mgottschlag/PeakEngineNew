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

#ifndef _OBJECTLISTMODEL_HPP_
#define _OBJECTLISTMODEL_HPP_

#include <QList>
#include <QVariant>
#include <QAbstractItemModel>
#include <QIcon>

class EditorObject;
class EditorProject;

class ObjectListModel : public QAbstractItemModel
{
	Q_OBJECT
	public:
		ObjectListModel(QObject *parent = 0);
		virtual ~ObjectListModel();

		QVariant data(const QModelIndex &index, int role) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		QVariant headerData(int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const;
		QModelIndex index(int row, int column,
			const QModelIndex &parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &index) const;
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

		void setProject(EditorProject *project)
		{
			this->project = project;
		}
		EditorProject *getProject()
		{
			return project;
		}

		void addEditorObject(EditorObject *obj);
		void removeEditorObject(EditorObject *obj);
		void updateObject(EditorObject *obj);
		EditorObject *getObject(const QModelIndex &index);
		void clear();
		void setEnabled(bool enabled);
	private:
		QList<EditorObject*> objects;
		QIcon reload;
		QIcon open;
		EditorProject *project;
		bool enabled;
};

#endif

