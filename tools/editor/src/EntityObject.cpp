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

#include "EntityObject.hpp"
#include "EditorProject.hpp"
#include "CodeGenerator.hpp"
#include "ObjectListModel.hpp"

EntityObject::EntityObject(EditorProject *project, QString name,
	ObjectListModel *list)
	: EditorObject(project, name, list), needsrecompile(true)
{
	
}
EntityObject::~EntityObject()
{
}

bool EntityObject::create()
{
	QString projdir = project->getDirectory();
	// Create files
	CodeGenerator codegen;
	if (!codegen.open(projdir, project->getName(), name + "Factory", true))
		return false;
	codegen.addHeaderCode("#include <PeakEngine.hpp>\n");
	codegen.addCode("#include \"" + name + "Factory.hpp\"\n");
	codegen.addClass(name + "Factory", "peak::EntityFactory");
	codegen.addConstructor(name + "Factory()", "peak::EntityFactory(\"" + name + "\")", EV_Public);
	codegen.endFunction();
	codegen.addFunction("", "~" + name + "Factory()", EV_Public);
	codegen.endFunction();
	codegen.endClass();
	codegen.close();
	// Register files in the project
	project->addSourceFile("src/generated/" + name + "Factory.cpp", true);
	project->addSourceFile("include/generated/" + name + "Factory.hpp", false);

	needsrecompile = true;
	return true;
}
bool EntityObject::remove()
{
	// TODO: Delete files
	return false;
}

EditorObjectType EntityObject::getType()
{
	return EEOT_Entity;
}

bool EntityObject::needsRecompile()
{
	return needsrecompile;
}
void EntityObject::recompiled()
{
	if (needsrecompile)
	{
		needsrecompile = false;
		list->updateObject(this);
	}
}
