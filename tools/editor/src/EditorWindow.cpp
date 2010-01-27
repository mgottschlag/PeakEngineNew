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

#include "EditorWindow.hpp"
#include "EditorObject.hpp"
#include "EditorProject.hpp"
#include "NewObjectDialog.hpp"
#include "CodeComponentObject.hpp"
#include "EntityObject.hpp"

#include <QMessageBox>
#include <QFileDialog>
#include <QRegExp>
#include <iostream>

EditorWindow::EditorWindow()
: QMainWindow(), playactions(this), project(0), currentobject(0)
{
	ui.setupUi(this);
	setProjectActions(false);
	// Play actions
	playactions.addAction(ui.actionStop);
	playactions.addAction(ui.actionPause);
	playactions.addAction(ui.actionPlay);
	// Set up input
	QObject::connect(ui.actionAbout, SIGNAL(activated()), this, SLOT(about()));
	QObject::connect(ui.actionNewProject, SIGNAL(activated()), this, SLOT(newProject()));
	QObject::connect(ui.actionOpenProject, SIGNAL(activated()), this, SLOT(openProject()));
	QObject::connect(ui.actionCloseProject, SIGNAL(activated()), this, SLOT(closeProject()));
	QObject::connect(ui.actionRecompile, SIGNAL(activated()), this, SLOT(recompile()));
	QObject::connect(ui.actionNewObject, SIGNAL(activated()), this, SLOT(newObject()));
	QObject::connect(ui.actionOpenObject, SIGNAL(activated()), this, SLOT(openObject()));
	QObject::connect(ui.actionCloseObject, SIGNAL(activated()), this, SLOT(closeObject()));
	QObject::connect(ui.objectList, SIGNAL(activated(QModelIndex)), this, SLOT(objectSelected(QModelIndex)));
	QObject::connect(ui.actionConfigure, SIGNAL(activated()), this, SLOT(configure()));
	// Initialize object list
	ui.objectList->setModel(&objectlistmodel);
	ui.objectList->setIndentation(0);
	ui.objectList->header()->resizeSection(0, 25);
	ui.objectList->header()->setResizeMode(0, QHeaderView::Fixed);
	ui.objectList->header()->resizeSection(1, 25);
	ui.objectList->header()->setResizeMode(1, QHeaderView::Fixed);
	// Title
	title = tr("PeakEngine Editor");
	updateTitle();
	// Inspector
	updateInspector();
}

void EditorWindow::about()
{
	QMessageBox::about(this, "PeakEngine Editor", "PeakEngine Editor");
}
void EditorWindow::newProject()
{
	// Close old project
	closeProject(true);
	// Select directory
	QFileDialog dialog(this, tr("Create project directory"), ".");
	dialog.setFileMode(QFileDialog::Directory);
	if (!dialog.exec())
		return;
	QString directory = dialog.selectedFiles()[0];
	// Check whether the directory is empty
	QDir dir(directory);
	if (dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot).size() > 0)
	{
		QMessageBox msg;
		msg.setText(tr("Overwriting directory"));
		msg.setInformativeText(tr("You are creating a project in a non-empty directory. "\
			"This might delete the content of the directory. Do you really want to continue?"));
		msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		if (msg.exec() != QMessageBox::Yes)
			return;
	}
	// Create project
	EditorProject *newproject = new EditorProject(&objectlistmodel);
	if (!newproject->create(directory))
	{
		delete newproject;
		QMessageBox::critical(this, tr("Error"), tr("Could not create project."));
		return;
	}
	project = newproject;
	setProjectActions(true);
	updateTitle();
}
void EditorWindow::openProject()
{
	// Close old project
	closeProject(true);
	// Select directory
	QFileDialog dialog(this, tr("Open project directory"), ".");
	dialog.setFileMode(QFileDialog::Directory);
	if (!dialog.exec())
		return;
	QString directory = dialog.selectedFiles()[0];
	// Open project
	EditorProject *newproject = new EditorProject(&objectlistmodel);
	if (!newproject->open(directory))
	{
		delete newproject;
		QMessageBox::critical(this, tr("Error"), tr("Could not open project."));
		return;
	}
	project = newproject;
	setProjectActions(true);
	updateTitle();
}
void EditorWindow::closeProject()
{
	closeProject(false);
}
void EditorWindow::closeProject(bool ask)
{
	if (!project)
		return;
	if (currentobject)
		currentobject = 0;
	// Close project
	setProjectActions(false);
	if (!project->save())
	{
		QMessageBox::critical(this, tr("Error"), tr("Could not save the project."));
	}
	objectlistmodel.clear();
	delete project;
	project = 0;
	updateTitle();
	updateInspector();
}
void EditorWindow::recompile()
{
	if (!project)
		return;
	// TODO: Close everything
	// Recompile project
	if (!project->recompile())
		QMessageBox::critical(this, tr("Error"), tr("Compiling the project sources failed."));
	ui.console->setPlainText(project->getCompilerOutput());
	// TODO: Get Game instance
	updatePlayActions();
}
void EditorWindow::newObject()
{
	if (!project)
		return;
	// Show dialog
	NewObjectDialog dialog;
	dialog.exec();
	// Validate name
	QString name = dialog.getName();
	QRegExp regex("[a-zA-Z]+[a-zA-Z0-9]*");
	if (!regex.exactMatch(name))
	{
		QMessageBox::critical(this, tr("Error"), tr("Invalid object name."));
		return;
	}
	// Check whether an object with the given name already exists
	if (project->getObject(name) != 0)
	{
		QMessageBox::critical(this, tr("Error"), tr("Name already in use."));
		return;
	}
	// Create object
	if (dialog.getType() == EEOT_CodeComponent)
	{
		CodeComponentObject *object = new CodeComponentObject(project, name,
			&objectlistmodel);
		object->create();
		project->addObject(object);
	}
	else if (dialog.getType() == EEOT_Entity)
	{
		EntityObject *object = new EntityObject(project, name,
			&objectlistmodel);
		object->create();
		project->addObject(object);
	}
	else
	{
		QMessageBox::critical(this, tr("Error"), tr("Object type not yet implemented."));
	}
}
void EditorWindow::openObject()
{
	if (!currentobject || currentobject->isOpen())
		return;
	// Open object
	if (!currentobject->open())
	{
		QMessageBox::critical(this, tr("Error"), tr("Could not open the currently active object."));
		return;
	}
	// Update inspector
	updateInspector();
}
void EditorWindow::closeObject()
{
	if (!currentobject || !currentobject->isOpen())
		return;
	// Close object
	if (!currentobject->close())
	{
		QMessageBox::critical(this, tr("Error"), tr("Could not properly close the object."));
		return;
	}
	// Update inspector
	updateInspector();
}
void EditorWindow::objectSelected(const QModelIndex &index)
{
	EditorObject *obj = objectlistmodel.getObject(index);
	currentobject = obj;
	updateTitle();
	updateObjectWidgets();
	updateInspector();
}
void EditorWindow::configure()
{
	if (!project)
		return;
	project->getSettingsWindow().show();
}

void EditorWindow::closeEvent(QCloseEvent *event)
{
	closeProject(false);
	event->accept();
}

void EditorWindow::updateTitle()
{
	QString title = this->title + " - ";
	if (project)
	{
		title += project->getName();
		if (currentobject)
			title +=  " - " + currentobject->getName();
	}
	else
		title += tr("No project opened");
	setWindowTitle(title);
}
void EditorWindow::setProjectActions(bool enabled)
{
	ui.actionNewObject->setEnabled(enabled);
	ui.actionConfigure->setEnabled(enabled);
	if (!enabled)
	{
		ui.actionOpenObject->setEnabled(false);
		ui.actionCloseObject->setEnabled(false);
	}
	else
		updateObjectWidgets();
	if (!enabled)
	{
		ui.actionStop->setEnabled(enabled);
		ui.actionPause->setEnabled(enabled);
		ui.actionPlay->setEnabled(enabled);
		ui.actionStop->setChecked(true);
		ui.actionLaunch->setEnabled(enabled);
	}
	else
		updatePlayActions();
	ui.actionRecompile->setEnabled(enabled);
	objectlistmodel.setEnabled(enabled);
	ui.inspectorStack->setCurrentWidget(ui.nothingInspector);
}
void EditorWindow::updateObjectWidgets()
{
	if (!currentobject)
	{
		ui.actionOpenObject->setEnabled(false);
		ui.actionCloseObject->setEnabled(false);
	}
	else
	{
		bool open = currentobject->isOpen();
		ui.actionOpenObject->setEnabled(!open);
		ui.actionCloseObject->setEnabled(open);
	}
}
void EditorWindow::updatePlayActions()
{
	bool enableactions = false;
	if (project && project->getGame())
		enableactions = true;
	else
		ui.actionStop->setEnabled(false);
	ui.actionStop->setEnabled(enableactions);
	ui.actionPause->setEnabled(enableactions);
	ui.actionPlay->setEnabled(enableactions);
	ui.actionLaunch->setEnabled(enableactions);
}

void EditorWindow::updateInspector()
{
	if (!currentobject || !currentobject->isOpen())
	{
		ui.inspectorStack->setCurrentWidget(ui.nothingInspector);
		return;
	}
	switch (currentobject->getType())
	{
		case EEOT_CodeComponent:
		{
			ui.inspectorStack->setCurrentWidget(ui.codeComponentInspector);
			CodeComponentObject *object = (CodeComponentObject*)currentobject;
			disconnect(ui.codeCompShow, SIGNAL(clicked()), 0, 0);
			connect(ui.codeCompShow, SIGNAL(clicked()), object, SLOT(editSource()));
			break;
		}
		default:
			ui.inspectorStack->setCurrentWidget(ui.nothingInspector);
			break;
	}
}
