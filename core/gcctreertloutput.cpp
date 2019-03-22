#include "stdafx.h"
#include "gcctreertloutput.h"

GCCTreeRTLOutput::GCCTreeRTLOutput(QWidget *parent)
    : ScintillaEdit (parent)
    , m_sc(this)
{
    
}

void GCCTreeRTLOutput::initialize()
{
    m_sc.initScintilla();
    m_sc.initLexerStyle("asm");

    setReadOnly(true);
}

void GCCTreeRTLOutput::setContent(const QString &content)
{
    auto b = content.toUtf8();
    setReadOnly(false);
    setText(b.data());
    setReadOnly(true);

    emptyUndoBuffer();
    m_sc.initLexerStyle("asm");
    colourise(0, -1);
}

QVariant GCCTreeRTLOutput::inputMethodQuery(Qt::InputMethodQuery /*query*/) const
{
    // so that it won't show input method pane on mobile device
    return QVariant(QRectF(0,0, 0, 0));
}

