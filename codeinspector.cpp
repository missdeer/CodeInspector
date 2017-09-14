#include "stdafx.h"
#include "codeinspector.h"

CodeInspector::CodeInspector(QWidget *parent)
    : ScintillaEdit (parent)
{
}

void CodeInspector::initialize()
{
    m_sc.initScintilla(this);
    m_sc.initInspectorMargins( this, false );
    m_sc.initLexerStyle(this, "asm");

    connect(this, &ScintillaEdit::linesAdded, this, &CodeInspector::linesAdded);
    connect(this, &ScintillaEdit::marginClicked, this, &CodeInspector::marginClicked);

    setReadOnly(true);
}

void CodeInspector::linesAdded(int /*linesAdded*/)
{
    ScintillaEdit* sci = qobject_cast<ScintillaEdit*>(sender());
    sptr_t line_count = sci->lineCount();
    sptr_t left = sci->marginLeft() + 2;
    sptr_t right = sci->marginRight() + 2;
    sptr_t width = left + right + sci->textWidth(STYLE_LINENUMBER, QString("%1").arg(line_count).toStdString().c_str());
    if (width > sci->marginWidthN(0))
        sci->setMarginWidthN(0, width);
}

void CodeInspector::marginClicked(int /*position*/, int /*modifiers*/, int /*margin*/)
{
}

void CodeInspector::setContent(const QString &content)
{
    auto b = content.toUtf8();
    setReadOnly(false);
    setText(b.data());
    setReadOnly(true);

    emptyUndoBuffer();
    m_sc.initLexerStyle(this, "asm");
    colourise(0, -1);
}

void CodeInspector::setBinaryMode(bool binary)
{
    m_sc.initInspectorMargins( this, binary );
}
