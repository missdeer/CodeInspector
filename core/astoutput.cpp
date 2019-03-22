#include "stdafx.h"
#include "astoutput.h"

ASTOutput::ASTOutput(QWidget *parent)
    : ScintillaEdit (parent)
    , m_sc(this)
{
    
}

void ASTOutput::initialize()
{
    m_sc.initScintilla();
    m_sc.initLexerStyle("asm");

    setReadOnly(true);
}

void ASTOutput::setContent(const QString &content)
{
    auto b = content.toUtf8();
    setReadOnly(false);
    setText(b.data());
    setReadOnly(true);

    emptyUndoBuffer();
    m_sc.initLexerStyle("asm");
    colourise(0, -1);
}

QVariant ASTOutput::inputMethodQuery(Qt::InputMethodQuery /*query*/) const
{
    // so that it won't show input method pane on mobile device
    return QVariant(QRectF(0,0, 0, 0));
}
