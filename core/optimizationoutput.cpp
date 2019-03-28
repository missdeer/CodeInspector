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

void OptimizationOutput::setContent(const OptimizationItemList &content)
{
    QString c;
    for (const auto & oi : content)
    {
        c.append(tr("Pass: "));
        c.append(oi->pass);
        c.append(tr("\nName: "));
        c.append(oi->name);
        c.append(tr("\nType: "));
        c.append(oi->type);
        c.append(tr("\nFunction: "));
        c.append(oi->function);
        c.append(tr("\nConclusion: "));
        c.append(oi->display);
        c.append("\n\n");
    }
    setContent(c);
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

QVariant OptimizationOutput::inputMethodQuery(Qt::InputMethodQuery /*query*/) const
{
    // so that it won't show input method pane on mobile device
    return QVariant(QRectF(0,0, 0, 0));
}


