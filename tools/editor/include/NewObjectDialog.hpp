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

#ifndef _NEWOBJECTDIALOG_HPP_
#define _NEWOBJECTDIALOG_HPP_

#include "EditorObject.hpp"

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

class NewObjectDialog : public QDialog
{
	Q_OBJECT
	public:
		NewObjectDialog(QWidget *parent = 0);

		EditorObjectType getType()
		{
			return type;
		}
		QString getName()
		{
			return nameedit.text();
		}
	public slots:
		void typeSelected(int type);
	private:
		QComboBox typelist;
		QVBoxLayout layout;
		QDialogButtonBox buttons;
		QHBoxLayout namelayout;
		QLabel namelabel;
		QLineEdit nameedit;

		EditorObjectType type;
		QString name;
};

#endif
