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

#ifndef _CODEEDITOR_HPP_
#define _CODEEDITOR_HPP_

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QListWidget>
#include <QStackedWidget>

class CodeEditWidget;

class CodeEditor : public QMainWindow
{
	Q_OBJECT
	public:
		CodeEditor(QString title, QString directory, QStringList files);
		~CodeEditor();

	private slots:
		void fileSelected(int file);
		void textChanged();
		void save();
		void saveAll();
		void quit();
	signals:
		void saved(QString file);
	private:
		virtual void closeEvent(QCloseEvent *event);
		bool save(int file);
		bool saveChanges();

		QListWidget *filelist;
		QString directory;
		QStringList files;
		QStackedWidget *filestack;
		QList<CodeEditWidget*> textedit;
		QList<bool> changed;
		int currentfile;
		QIcon saveicon;
};

#endif
