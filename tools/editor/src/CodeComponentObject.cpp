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

#include "CodeComponentObject.hpp"
#include "EditorProject.hpp"
#include "CodeGenerator.hpp"
#include "ObjectListModel.hpp"
#include "CodeEditor.hpp"

CodeComponentObject::CodeComponentObject(EditorProject *project, QString name,
	ObjectListModel *list)
	: EditorObject(project, name, list), needsrecompile(true), isopen(false),
	code(0)
{
	
}
CodeComponentObject::~CodeComponentObject()
{
	if (code)
		delete code;
}

bool CodeComponentObject::create()
{
	QString projdir = project->getDirectory();
	// Create files
	CodeGenerator codegen;
	if (!codegen.open(projdir, project->getName(), name, false))
		return false;
	codegen.addHeaderCode("#include <PeakEngine.hpp>\n");
	codegen.addCode("#include \"" + name + ".hpp\"\n");
	codegen.addClass(name, "peak::EntityComponent");
	codegen.addConstructor(name + "(peak::Entity *entity)", "peak::EntityComponent(entity)", EV_Public);
	codegen.endFunction();
	codegen.addFunction("", "~" + name + "()", EV_Public);
	codegen.endFunction();
	codegen.endClass();
	codegen.close();
	// Register files in the project
	project->addSourceFile("src/" + name + ".cpp", true);
	project->addSourceFile("include/" + name + ".hpp", false);

	needsrecompile = true;
	return true;
}
bool CodeComponentObject::remove()
{
	// TODO: Delete files
	return false;
}

EditorObjectType CodeComponentObject::getType()
{
	return EEOT_CodeComponent;
}

bool CodeComponentObject::open()
{
	isopen = true;
	if (!code)
	{
		QStringList files;
		files.append("src/" + name + ".cpp");
		files.append("include/" + name + ".hpp");
		code = new CodeEditor(name, project->getDirectory(), files);
		connect(code, SIGNAL(saved(QString)), SLOT(codeChanged()));
	}
	return true;
}
bool CodeComponentObject::close()
{
	isopen = false;
	if (code)
	{
		delete code;
		code = 0;
	}
	return true;
}
bool CodeComponentObject::isOpen()
{
	return isopen;
}

void CodeComponentObject::editSource()
{
	code->showMaximized();
}

bool CodeComponentObject::needsRecompile()
{
	return needsrecompile;
}
void CodeComponentObject::codeChanged()
{
	if (!needsrecompile)
	{
		needsrecompile = true;
		list->updateObject(this);
	}
}
void CodeComponentObject::recompiled()
{
	if (needsrecompile)
	{
		needsrecompile = false;
		list->updateObject(this);
	}
}
