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

#include "CodeGenerator.hpp"

#include <QDateTime>
#include <QStringList>

CodeGenerator::CodeGenerator()
{
	header.setDevice(&headerfile);
	source.setDevice(&sourcefile);
}
CodeGenerator::~CodeGenerator()
{
}

bool CodeGenerator::open(QString directory, QString project, QString filename,
	bool generated)
{
	// Open source files
	headerfile.setFileName(directory
	+ (generated ? "/include/generated/" : "/include/")
		+ filename + ".hpp");
	sourcefile.setFileName(directory
	+ (generated ? "/src/generated/" : "/src/")
		+ filename + ".cpp");
	if (!headerfile.open(QIODevice::WriteOnly))
		return false;
	if (!sourcefile.open(QIODevice::WriteOnly))
		return false;
	// Write headers
	writeHeader(header, project, filename + ".hpp", generated);
	writeHeader(source, project, filename + ".cpp", generated);
	// Reset code generator state
	namespaces.clear();
	classes.clear();
	srcindent = "";
	headerindent = "";
	function = false;
	// Start include guard
	addHeaderCode("#ifndef _" + filename + "_included_");
	addHeaderCode("#define _" + filename + "_included_");
	return true;
}
bool CodeGenerator::close()
{
	// Close opened brackets
	if (function)
		endFunction();
	while (classes.size() > 0)
		endClass();
	while (namespaces.size() > 0)
		endNamespace();
	// End include guard
	addHeaderCode("#endif");
	// Close files
	headerfile.close();
	sourcefile.close();
	return true;
}

void CodeGenerator::addNamespace(QString ns)
{
	addCode("namespace " + ns + "\n{");
	addHeaderCode("namespace " + ns + "\n{");
	srcindent += '\t';
	headerindent += '\t';
	namespaces.append(ns);
}
void CodeGenerator::endNamespace()
{
	srcindent = srcindent.left(srcindent.size() - 1);
	headerindent = headerindent.left(headerindent.size() - 1);
	addCode("}");
	addHeaderCode("}");
	namespaces.removeLast();
}
void CodeGenerator::addClass(QString name, QString base)
{
	if (base == "")
		addHeaderCode("class " + name + "\n{");
	else
		addHeaderCode("class " + name + " : " + base + "\n{");
	headerindent += "\t\t";
	classes.append(name);
	function = false;
	vis = EV_Private;
}
void CodeGenerator::endClass()
{
	headerindent = headerindent.left(headerindent.size() - 2);
	addHeaderCode("};");
	classes.removeLast();
}
void CodeGenerator::addConstructor(QString function, QString initializer,
	Visibility vis)
{
	changeVisibility(vis);
	addHeaderCode(function + ";");
	if (classes.size() > 0)
		function = classes[classes.size() - 1] + "::" + function;
	if (initializer == "")
		addCode(function + "\n{");
	else
		addCode(function + "\n\t: " + initializer + "\n{");
	srcindent += '\t';
	function = true;
}
void CodeGenerator::addFunction(QString rettype, QString function, Visibility vis)
{
	changeVisibility(vis);
	addHeaderCode(rettype + function + ";");
	if (classes.size() > 0)
		function = classes[classes.size() - 1] + "::" + function;
	if (rettype.startsWith("virtual "))
		rettype = rettype.right(rettype.size() - 8);
	else if (rettype.startsWith("static "))
		rettype = rettype.right(rettype.size() - 7);
	addCode(rettype + function + "\n{");
	srcindent += '\t';
	function = true;
}
void CodeGenerator::endFunction()
{
	srcindent = srcindent.left(srcindent.size() - 1);
	addCode("}");
	function = false;
}
void CodeGenerator::addCode(QString code)
{
	QStringList lines = code.split("\n");
	for (int i = 0; i < lines.size(); i++)
	{
		source << srcindent << lines[i] << "\n";
	}
}
void CodeGenerator::addHeaderCode(QString code)
{
	QStringList lines = code.split("\n");
	for (int i = 0; i < lines.size(); i++)
	{
		header << headerindent << lines[i] << "\n";
	}
}
void CodeGenerator::addVariable(QString var, Visibility vis)
{
}

void CodeGenerator::writeHeader(QTextStream &file, QString project,
	QString filename, bool generated)
{
	file << "// " << project << ": " << filename << "\n";
	if (generated)
	{
		file << "// Generated on " << QDateTime::currentDateTime().toString() << ".\n";
		file << "// AUTOMATICALLY GENERATED, DO NOT MODIFY!\n";
	}
	file << "\n";
}
void CodeGenerator::changeVisibility(Visibility vis)
{
	if (vis != this->vis)
	{
		this->vis = vis;
		headerindent = headerindent.left(headerindent.size() - 1);
		switch (vis)
		{
			case EV_Private:
				addHeaderCode("private:");
				break;
			case EV_Public:
				addHeaderCode("public:");
				break;
			case EV_Protected:
				addHeaderCode("protected:");
				break;
		}
		headerindent += "\t";
	}
}
