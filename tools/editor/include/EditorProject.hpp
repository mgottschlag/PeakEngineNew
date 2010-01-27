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

#ifndef _EDITORPROJECT_HPP_
#define _EDITORPROJECT_HPP_

#include "peakengine/core/Game.hpp"

#include "ProjectSettings.hpp"

#include <QString>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QLibrary>
#include <vector>

class EditorObject;
class ObjectListModel;

class EditorProject
{
	public:
		EditorProject(ObjectListModel *listmodel);
		~EditorProject();

		bool open(QString directory);
		bool create(QString directory);
		bool save();

		bool buildInitialSource();
		bool rebuildGameSource();
		bool recompile();

		void addSourceFile(QString path, bool compile);
		void removeSourceFile(QString path);

		void addObject(EditorObject *object);
		void removeObject(EditorObject *object);
		EditorObject *getObject(QString name);

		const QString &getCompilerOutput()
		{
			return compileroutput;
		}

		peak::Game *getGame()
		{
			return game;
		}

		ProjectSettings &getSettingsWindow()
		{
			return settings;
		}

		const QString &getName()
		{
			return name;
		}
		const QString &getDirectory()
		{
			return directory;
		}
	private:
		bool saveXML();
		QStringList getCompileFiles();

		QString name;
		QString directory;
		QDomDocument xml;
		QDomElement srcelem;
		QDomElement objelem;
		QString compileroutput;
		QLibrary library;
		peak::Game *game;
		std::vector<EditorObject*> objects;

		ObjectListModel *listmodel;

		ProjectSettings settings;
};

#endif

