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
