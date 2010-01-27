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

#ifndef _EDITOROBJECT_HPP_
#define _EDITOROBJECT_HPP_

#include <QString>
#include <QObject>

enum EditorObjectType
{
	EEOT_Entity,
	EEOT_CodeComponent,
	EEOT_ScriptComponent,
	EEOT_GraphicsComponent,
	EEOT_PhysicsComponent,
	EEOT_GUILayout,
	EEOT_World
};

class EditorProject;
class ObjectListModel;

class EditorObject : public QObject
{
	Q_OBJECT
	public:
		EditorObject(EditorProject *project, QString name, ObjectListModel *list)
		: QObject(), project(project), name(name), list(list)
		{
		}
		~EditorObject()
		{
		}

		virtual bool create() = 0;
		virtual bool remove() = 0;

		const QString &getName()
		{
			return name;
		}
		virtual EditorObjectType getType() = 0;

		virtual bool open()
		{
			return false;
		}
		virtual bool close()
		{
			return false;
		}
		virtual bool isOpen()
		{
			return false;
		}
		virtual bool needsRecompile()
		{
			return false;
		}
	public slots:
		virtual void codeChanged()
		{
		}
		virtual void recompiled()
		{
		}
	protected:
		EditorProject *project;
		QString name;
		ObjectListModel *list;
};

#endif

