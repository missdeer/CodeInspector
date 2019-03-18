#include "stdafx.h"
#include "optimizationoutput.h"

OptimizationOutput::OptimizationOutput(QWidget *parent)
    : ScintillaEdit (parent)
    , m_sc(this)
{
    
}

void OptimizationOutput::initialize()
{
    m_sc.initScintilla();
    m_sc.initLexerStyle("asm");

    setReadOnly(true);
}

void OptimizationOutput::setContent(const QString &content)
{
    auto b = content.toUtf8();
    setReadOnly(false);
    setText(b.data());
    setReadOnly(true);

    emptyUndoBuffer();
    m_sc.initLexerStyle("asm");
    colourise(0, -1);
}
