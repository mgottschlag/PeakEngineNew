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

#include "NewObjectDialog.hpp"

#include <QPushButton>
#include <QLabel>

NewObjectDialog::NewObjectDialog(QWidget *parent)
	: QDialog(parent), type(EEOT_Entity), name("")
{
	QPushButton *button = buttons.addButton(QDialogButtonBox::Cancel);
	QObject::connect(button, SIGNAL(clicked()), this, SLOT(reject()));
	button = buttons.addButton(QDialogButtonBox::Ok);
	QObject::connect(button, SIGNAL(clicked()), this, SLOT(accept()));
	typelist.addItem("Entity");
	typelist.addItem("CodeComponent");
	typelist.addItem("ScriptComponent");
	typelist.addItem("GraphicsComponent");
	typelist.addItem("PhysicsComponent");
	typelist.addItem("GUILayout");
	typelist.addItem("World");
	typelist.setCurrentIndex(0);
	QObject::connect(&typelist, SIGNAL(currentIndexChanged(int)), this, SLOT(typeSelected(int)));
	namelabel.setText(tr("Name"));
	namelayout.addWidget(&namelabel);
	namelayout.addWidget(&nameedit);
	layout.addWidget(&typelist);
	layout.addLayout(&namelayout);
	layout.addWidget(&buttons);
	setLayout(&layout);
}

void NewObjectDialog::typeSelected(int type)
{
	this->type = (EditorObjectType)type;
}
