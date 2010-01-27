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

#ifndef _EDITORWINDOW_HPP_
#define _EDITORWINDOW_HPP_

#include "ui_MainWindow.h"
#include "ObjectListModel.hpp"

class EditorProject;

class EditorWindow : public QMainWindow
{
	Q_OBJECT
	public:
		EditorWindow();

	public slots:
		void about();
		void newProject();
		void openProject();
		void closeProject();
		void closeProject(bool ask);
		void recompile();
		void newObject();
		void openObject();
		void closeObject();
		void objectSelected(const QModelIndex &index);
		void configure();
	protected:
		virtual void closeEvent(QCloseEvent *event);
	private:
		void updateTitle();
		void setProjectActions(bool enabled);
		void updateObjectWidgets();
		void updatePlayActions();
		void updateInspector();

		Ui::MainWindow ui;
		ObjectListModel objectlistmodel;
		QActionGroup playactions;

		QString title;
		EditorProject *project;
		EditorObject *currentobject;
};

#endif

