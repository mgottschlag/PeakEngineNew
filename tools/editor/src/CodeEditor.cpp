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

#include "CodeEditor.hpp"
#include "CodeEditWidget.hpp"

#include <QHBoxLayout>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QMenuBar>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

CodeEditor::CodeEditor(QString title, QString directory, QStringList files)
	: QMainWindow(), directory(directory), files(files),
	saveicon(":/icons/document-save.png")
{
	// Open files
	QList<QString> filecontents;
	for (int i = 0; i < files.size(); i++)
	{
		QString content;
		QFile file(directory + "/" + files[i]);
		if (file.open(QIODevice::ReadOnly))
			content = file.readAll();
		filecontents.append(content);
		changed.append(false);
	}
	// Build window
	QIcon windowicon;
	windowicon.addFile(QString::fromUtf8(":/icons/code-class.png"), QSize(),
		QIcon::Normal, QIcon::Off);
	setWindowIcon(windowicon);
	setWindowTitle(title);
	QWidget *centralwidget = new QWidget();
	setCentralWidget(centralwidget);
	QHBoxLayout *layout = new QHBoxLayout(centralwidget);
	centralwidget->setLayout(layout);
	filelist = new QListWidget(this);
	for (int i = 0; i < files.size(); i++)
	{
		filelist->addItem(files[i]);
	}
	filelist->setCurrentRow(0);
	connect(filelist, SIGNAL(currentRowChanged(int)), this, SLOT(fileSelected(int)));
	filestack = new QStackedWidget(this);
	for (int i = 0; i < files.size(); i++)
	{
		CodeEditWidget *textedit = new CodeEditWidget(this);
		textedit->setPlainText(filecontents[i]);
		connect(textedit, SIGNAL(textChanged()), this, SLOT(textChanged()));
		filestack->addWidget(textedit);
		this->textedit.append(textedit);
	}
	currentfile = 0;
	layout->addWidget(filelist);
	layout->addWidget(filestack, 1);
	// Actions
	QAction *saveaction = new QAction(saveicon, "&Save", this);
	connect(saveaction, SIGNAL(activated()), this, SLOT(save()));
	QIcon saveallicon(":/icons/document-save-all.png");
	QAction *saveallaction = new QAction(saveallicon, "Save a&ll", this);
	connect(saveallaction, SIGNAL(activated()), this, SLOT(saveAll()));
	QIcon exiticon(":/icons/application-exit.png");
	QAction *exitaction = new QAction(exiticon, "&Quit", this);
	connect(exitaction, SIGNAL(activated()), this, SLOT(quit()));
	// Menu
	QMenu *menu = menuBar()->addMenu(tr("&File"));
	menu->addAction(saveaction);
	menu->addAction(saveallaction);
	menu->addSeparator();
	menu->addAction(exitaction);
	QToolBar *toolbar = addToolBar(tr("File"));
	toolbar->addAction(saveaction);
	toolbar->addAction(saveallaction);
}
CodeEditor::~CodeEditor()
{
}

void CodeEditor::fileSelected(int file)
{
	currentfile = file;
	filestack->setCurrentIndex(file);
}
void CodeEditor::textChanged()
{
	changed[currentfile] = true;
	filelist->item(currentfile)->setIcon(saveicon);
}
void CodeEditor::save()
{
	save(currentfile);
}
void CodeEditor::saveAll()
{
	for (int i = 0; i < files.size(); i++)
	{
		save(i);
	}
}
void CodeEditor::quit()
{
	if (saveChanges())
		close();
}

void CodeEditor::closeEvent(QCloseEvent *event)
{
	if (saveChanges())
		event->accept();
}
bool CodeEditor::save(int fileindex)
{
	QFile file(directory + "/" + files[fileindex]);
	QTextStream out(&file);
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(this, tr("Error"), tr("Could not save file."));
		return false;
	}
	out << textedit[fileindex]->toPlainText();
	file.close();
	changed[fileindex] = false;
	QIcon noicon;
	filelist->item(fileindex)->setIcon(noicon);
	saved(files[fileindex]);
	return true;
}
bool CodeEditor::saveChanges()
{
	bool unsaved = false;
	for (int i = 0; i < changed.size(); i++)
	{
		if (changed[i])
		{
			unsaved = true;
			break;
		}
	}
	if (!unsaved)
		return true;
	// Ask for saving unsaved changes
	QMessageBox msg;
	msg.setText(tr("Unsaved changes"));
	msg.setInformativeText(tr("Do you want to save your changes?"));
	msg.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	msg.setDefaultButton(QMessageBox::Save);
	int res = msg.exec();
	if (res == QMessageBox::Save)
	{
		for (int i = 0; i < changed.size(); i++)
		{
			if (changed[i])
			{
				if (!save(i))
					return false;
			}
		}
	}
	else if (res != QMessageBox::Discard)
		return false;
	return true;
}
