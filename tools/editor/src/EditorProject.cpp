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

#include "EditorProject.hpp"
#include "CodeGenerator.hpp"
#include "EditorObject.hpp"
#include "CodeComponentObject.hpp"
#include "EntityObject.hpp"
#include "ObjectListModel.hpp"

#include <QDir>
#include <QDateTime>
#include <QProcess>

EditorProject::EditorProject(ObjectListModel *listmodel)
	: game(0), listmodel(listmodel)
{
	listmodel->setProject(this);
}
EditorProject::~EditorProject()
{
	listmodel->clear();
	listmodel->setProject(0);
	// Delete project objects
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	// Destroy game instance
	if (game)
		delete game;
}

bool EditorProject::open(QString directory)
{
	this->directory = directory;
	// Open XML file
	xml = QDomDocument("EditorProject");
	QFile file(directory + "/PeakGame.xml");
	if (!file.open(QIODevice::ReadOnly))
		return false;
	if (!xml.setContent(&file))
	{
		file.close();
		return false;
	}
	file.close();
	// Parse document
	QDomElement root = xml.documentElement();
	if (root.tagName() != "PeakGame")
	{
		return false;
	}
	name = root.attribute("name", "");
	srcelem = root.firstChildElement("Source");
	objelem = root.firstChildElement("Objects");
	// Open objects
	QDomNodeList xmlobjs = objelem.elementsByTagName("Object");
	for (int i = 0; i < xmlobjs.size(); i++)
	{
		QDomElement element = xmlobjs.item(i).toElement();
		QString name = element.attribute("name", "");
		QString type = element.attribute("type", "");
		if (type == "CodeComponent")
		{
			CodeComponentObject *obj = new CodeComponentObject(this, name, listmodel);
			objects.push_back(obj);
			listmodel->addEditorObject(obj);
		}
		else if (type == "Entity")
		{
			EntityObject *obj = new EntityObject(this, name, listmodel);
			objects.push_back(obj);
			listmodel->addEditorObject(obj);
		}
		else
		{
			// TODO
		}
	}
	return true;
}
bool EditorProject::create(QString directory)
{
	this->directory = directory;
	name = "Unnamed project";
	// Create directories
	QDir dir(directory);
	dir.mkpath("src/generated");
	dir.mkpath("include/generated");
	dir.mkpath("objects");
	dir.mkpath("media");
	// Create XML file
	xml = QDomDocument("EditorProject");
	QDomElement root = xml.createElement("PeakGame");
	root.setAttribute("name", name);
	xml.appendChild(root);
	srcelem = xml.createElement("Source");
	root.appendChild(srcelem);
	objelem = xml.createElement("Objects");
	root.appendChild(objelem);
	if (!saveXML())
		return false;
	// Create initial sources
	if (!buildInitialSource())
		return false;
	return true;
}
bool EditorProject::save()
{
	// Save objects
	// TODO
	// Save XML file
	if (!saveXML())
		return false;
	return true;
}

bool EditorProject::buildInitialSource()
{
	if (!rebuildGameSource())
		return false;
	// Game class
	CodeGenerator codegen;
	if (!codegen.open(directory, name, "Game"))
		return false;
	codegen.addHeaderCode("#include \"GameTemplate.hpp\"\n");
	codegen.addCode("#include \"Game.hpp\"\n");
	codegen.addClass("Game", "GameTemplate");
	codegen.addConstructor("Game()", "GameTemplate()", EV_Public);
	codegen.endFunction();
	codegen.addFunction("", "~Game()", EV_Public);
	codegen.endFunction();
	codegen.addFunction("virtual bool ", "load()", EV_Public);
	codegen.addCode("return true;");
	codegen.endFunction();
	codegen.addFunction("virtual bool ", "shutdown()", EV_Public);
	codegen.addCode("return true;");
	codegen.endFunction();
	codegen.endClass();
	codegen.close();
	// Add files to source list
	addSourceFile("src/generated/GameTemplate.cpp", true);
	addSourceFile("include/generated/GameTemplate.hpp", false);
	addSourceFile("src/Game.cpp", true);
	addSourceFile("include/Game.hpp", false);
	return true;
}
bool EditorProject::rebuildGameSource()
{
	// Game template class
	CodeGenerator codegen;
	if (!codegen.open(directory, name, "GameTemplate", true))
		return false;
	codegen.addHeaderCode("#include <PeakEngine.hpp>\n");
	codegen.addCode("#include \"GameTemplate.hpp\"\n");
	codegen.addCode("#include \"Game.hpp\"\n");
	codegen.addClass("GameTemplate", "peak::Game");
	codegen.addConstructor("GameTemplate()", "peak::Game()", EV_Public);
	codegen.endFunction();
	codegen.addFunction("", "~GameTemplate()", EV_Public);
	codegen.endFunction();
	codegen.endClass();
	codegen.addCode("extern \"C\" Game *createGame()\n{\n\treturn new Game;\n}\n");
	codegen.close();
	return true;
}
bool EditorProject::recompile()
{
	// Close library
	if (game)
		delete game;
	library.unload();
	// Rebuild generated sources
	rebuildGameSource();
	// Rebuild cmake file
	QStringList files = getCompileFiles();
	QFile file(directory + "/CMakeLists.txt");
	if (!file.open(QIODevice::WriteOnly))
		return false;
	QTextStream out(&file);
	out << "project(Game)\ncmake_minimum_required(VERSION 2.4.0)\n"
		<< "set(CMAKE_CXX_FLAGS \"-Wall -Wextra -Wno-unused-parameter\")\n";
	out << "set(SRC\n";
	for (int i = 0; i < files.size(); i++)
		out << files[i] << "\n";
	out << ")\n";
	out << "include_directories(include include/generated)\n";
	out << "add_library(Game SHARED ${SRC})\n";
	file.close();
	// Call make
	// TODO: Needs to be done asynchronous
	compileroutput = "";
	QProcess cmake;
	cmake.setWorkingDirectory(directory);
	cmake.setProcessChannelMode(QProcess::MergedChannels);
	cmake.start("cmake " + directory);
	cmake.waitForFinished();
	compileroutput += QString::fromLocal8Bit(cmake.readAllStandardOutput().data());
	if (cmake.exitCode() != 0)
		return false;
	QProcess make;
	make.setWorkingDirectory(directory);
	make.setProcessChannelMode(QProcess::MergedChannels);
	make.start("make");
	make.waitForFinished();
	compileroutput += QString::fromLocal8Bit(make.readAllStandardOutput().data());
	if (make.exitCode() != 0)
		return false;
	// Open library
	library.setFileName(directory + "/libGame");
	if (!library.load())
	{
		compileroutput += "Could not open shared library!\n";
		return false;
	}
	// Create game
	peak::Game*(*createFunction)() = (peak::Game*(*)())library.resolve("createGame");
	if (createFunction == 0)
	{
		compileroutput += "Did not find createFunction()!\n";
		return false;
	}
	game = createFunction();
	if (game == 0)
	{
		compileroutput += "Could not create Game instance!\n";
		return false;
	}
	compileroutput += "Created Game instance!\n";
	// Update object state
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->recompiled();
	}
	return true;
}
void EditorProject::addSourceFile(QString path, bool compile)
{
	// TODO: Check whether the file already exists
	QDomElement xmlentry = xml.createElement("File");
	srcelem.appendChild(xmlentry);
	xmlentry.setAttribute("path", path);
	xmlentry.setAttribute("compile", compile ? "yes" : "no");
}
void EditorProject::removeSourceFile(QString path)
{
	QDomNodeList files = srcelem.elementsByTagName("File");
	for (int i = 0; i < files.size(); i++)
	{
		QDomElement element = files.item(i).toElement();
		if (element.attribute("path", "") == path)
		{
			srcelem.removeChild(element);
			return;
		}
	}
}

void EditorProject::addObject(EditorObject *object)
{
	objects.push_back(object);
	listmodel->addEditorObject(object);
	// Add to XML file
	QDomElement xmlentry = xml.createElement("Object");
	objelem.appendChild(xmlentry);
	xmlentry.setAttribute("name", object->getName());
	if (object->getType() == EEOT_CodeComponent)
	{
		xmlentry.setAttribute("type", "CodeComponent");
	}
	else if (object->getType() == EEOT_Entity)
	{
		xmlentry.setAttribute("type", "Entity");
	}
	else
	{
		// TODO
	}
}
void EditorProject::removeObject(EditorObject *object)
{
	listmodel->removeEditorObject(object);
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		if (objects[i] == object)
		{
			objects.erase(objects.begin() + i);
			break;
		}
	}
	// Remove from XML file
	QDomNodeList xmlobjs = objelem.elementsByTagName("Object");
	for (int i = 0; i < xmlobjs.size(); i++)
	{
		QDomElement element = xmlobjs.item(i).toElement();
		if (element.attribute("name", "") == object->getName())
		{
			objelem.removeChild(element);
			return;
		}
	}
}

EditorObject *EditorProject::getObject(QString name)
{
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->getName() == name)
		{
			return objects[i];
		}
	}
	return 0;
}

bool EditorProject::saveXML()
{
	QFile file(directory + "/PeakGame.xml");
	if (!file.open(QIODevice::WriteOnly))
		return false;
	file.write(xml.toByteArray());
	file.close();
	return true;
}
QStringList EditorProject::getCompileFiles()
{
	QStringList list;
	QDomNodeList files = srcelem.elementsByTagName("File");
	for (int i = 0; i < files.size(); i++)
	{
		QDomElement element = files.item(i).toElement();
		if (element.attribute("compile", "no") == "yes")
			list.append(element.attribute("path", ""));
	}
	return list;
}
