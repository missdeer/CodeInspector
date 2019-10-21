#include "stdafx.h"

#include "LLVMIRView.h"

LLVMIRView::LLVMIRView(QWidget *parent) : ScintillaEdit(parent), m_sc(this) {}

void LLVMIRView::initialize()
{
    m_sc.initScintilla();
    m_sc.initInspectorMargins(false);
    m_sc.initLexerStyle("asm");

    connect(this, &ScintillaEdit::marginClicked, this, &LLVMIRView::marginClicked);
    setReadOnly(true);
}

void LLVMIRView::setContent(const QString &content)
{
    auto b = content.toUtf8();
    setReadOnly(false);
    setText(b.data());
    setReadOnly(true);

    emptyUndoBuffer();
    m_sc.initLexerStyle("asm");
    colourise(0, -1);
}

QVariant LLVMIRView::inputMethodQuery(Qt::InputMethodQuery /*query*/) const
{
    // so that it won't show input method pane on mobile device
    return QVariant(QRectF(0, 0, 0, 0));
}
