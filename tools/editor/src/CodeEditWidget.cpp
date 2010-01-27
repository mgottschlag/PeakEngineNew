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

#include "CodeEditWidget.hpp"

#include <QPainter>
#include <QTextBlock>

CHighlighter::CHighlighter(QTextDocument *parent)
	: QSyntaxHighlighter(parent)
{
	// Keywords
	keyword.setForeground(Qt::darkBlue);
	keyword.setFontWeight(QFont::Bold);
	QStringList keywords;
	keywords << "char" << "int" << "float" << "double" << "signed" << "inline"
		<< "unsigned" << "public" << "private" << "protected" << "class"
		<< "bool" << "operator" << "explicit" << "union" << "volatile" << "void"
		<< "struct" << "enum" << "short" << "static" << "typedef" << "virtual"
		<< "namespace" << "long" << "friend";
	HighlightingRule rule;
	foreach (const QString &pattern, keywords) {
		rule.pattern = QRegExp("\\b" + pattern + "\\b");
		rule.format = keyword;
		rules.append(rule);
	}
	// Comments
	multilinecomment.setForeground(Qt::gray);
	singlelinecomment.setForeground(Qt::gray);
	rule.pattern = QRegExp("//[^\n]*");
	rule.format = singlelinecomment;
	rules.append(rule);
	commentstart = QRegExp("/\\*");
	commentend = QRegExp("\\*/");
	// Immediate values
	quotation.setForeground(Qt::red);
	rule.pattern = QRegExp("\".*\"");
	rule.format = quotation;
	rules.append(rule);
	// TODO: Numbers
	// Preprocessor
	preprocessor.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("#[^\n]*");
	rule.format = preprocessor;
	rules.append(rule);
}
void CHighlighter::highlightBlock(const QString &text)
{
	// Simple highlighting
	foreach (const HighlightingRule &rule, rules)
	{
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0)
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}
	// Multi line comments
	int prevstate = previousBlockState();
	int startindex = 0;
	if (prevstate != 1)
		startindex = commentstart.indexIn(text);
	while (startindex != -1)
	{
		int endindex = commentend.indexIn(text, startindex);
		if (endindex == -1)
		{
			setCurrentBlockState(1);
			int length = text.length() - startindex;
			setFormat(startindex, length, multilinecomment);
			return;
		}
		setFormat(startindex, endindex, multilinecomment);
		startindex = commentstart.indexIn(text, endindex);
	}
	setCurrentBlockState(0);
}


class LineNumberArea : public QWidget
{
	public:
		LineNumberArea(CodeEditWidget *codeedit)
		: QWidget(codeedit), codeedit(codeedit)
		{
		}
		
		QSize sizeHint() const
		{
			return QSize(codeedit->lineNumberAreaWidth(), 0);
		}
	protected:
		void paintEvent(QPaintEvent *event)
		{
			codeedit->lineNumberAreaPaintEvent(event);
		}
	private:
		CodeEditWidget *codeedit;
};

CodeEditWidget::CodeEditWidget(QWidget *parent)
{
	linenumbers = new LineNumberArea(this);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(const QRect &, int)), this, SLOT(updateLineNumberArea(const QRect &, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	updateLineNumberAreaWidth(0);
	highlightCurrentLine();

	// Syntax hightlighting
	new CHighlighter(document());
	// Font
	QTextCharFormat format = currentCharFormat();
	QFont font("Monospace");
	font.setStyleHint(QFont::TypeWriter);
	format.setFont(font);
	setCurrentCharFormat(format);
	// Tab width
	setTabStopWidth(QFontMetrics(font).width("    "));
}
CodeEditWidget::~CodeEditWidget()
{
}

void CodeEditWidget::lineNumberAreaPaintEvent(QPaintEvent *event)
{
	QPainter painter(linenumbers);
	painter.fillRect(event->rect(), Qt::lightGray);
	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int)blockBoundingRect(block).height();
	while (block.isValid() && top <= event->rect().bottom())
	{
		if (block.isVisible() && bottom >= event->rect().top())
		{
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::black);
			painter.drawText(0, top, linenumbers->width(), fontMetrics().height(),
				Qt::AlignRight, number);
		}
		
		block = block.next();
		top = bottom;
		bottom = top + (int)blockBoundingRect(block).height();
		++blockNumber;
	}
}
int CodeEditWidget::lineNumberAreaWidth()
{
	int digits;
	int lines = blockCount();
	if (lines < 1)
		digits = 1;
	else
	{
		digits = 1;
		while (lines > 9)
		{
			digits++;
			lines /= 10;
		}
	}
	return 3 + fontMetrics().width(QLatin1Char('9')) * digits;
}
void CodeEditWidget::resizeEvent(QResizeEvent *event)
{
	QPlainTextEdit::resizeEvent(event);
	QRect contents = contentsRect();
	linenumbers->setGeometry(QRect(contents.left(), contents.top(),
		lineNumberAreaWidth(), contents.height()));
}
void CodeEditWidget::updateLineNumberAreaWidth(int newBlockCount)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}
void CodeEditWidget::highlightCurrentLine()
{
}
void CodeEditWidget::updateLineNumberArea(const QRect &rect, int dy)
{
	if (dy)
		linenumbers->scroll(0, dy);
	else
		linenumbers->update(0, rect.y(), linenumbers->width(), rect.height());
	
	if (rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}