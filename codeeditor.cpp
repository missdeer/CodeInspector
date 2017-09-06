#include "stdafx.h"
#include "codeeditor.h"

CodeEditor::CodeEditor(QWidget *parent)
    : ScintillaEdit (parent)
{
}

void CodeEditor::initialize()
{
    m_sc.initScintilla(this);
    m_sc.initEditorStyle(this);

    connect(this, &ScintillaEdit::linesAdded, this, &CodeEditor::linesAdded);
    connect(this, &ScintillaEdit::marginClicked, this, &CodeEditor::marginClicked);
}

void CodeEditor::linesAdded(int /*linesAdded*/)
{
    ScintillaEdit* sci = qobject_cast<ScintillaEdit*>(sender());
    sptr_t line_count = sci->lineCount();
    sptr_t left = sci->marginLeft() + 2;
    sptr_t right = sci->marginRight() + 2;
    sptr_t width = left + right + sci->textWidth(STYLE_LINENUMBER, QString("%1").arg(line_count).toStdString().c_str());
    if (width > sci->marginWidthN(0))
        sci->setMarginWidthN(0, width);
}

void CodeEditor::marginClicked(int position, int /*modifiers*/, int margin)
{
    ScintillaEdit* sci = qobject_cast<ScintillaEdit*>(sender());
    if (sci->marginTypeN(margin) == SC_MARGIN_SYMBOL)
    {
        sptr_t maskN = sci->marginMaskN(margin);
        if ((maskN & 0xFFFFFFFF) == SC_MASK_FOLDERS)
        {
            sptr_t line = sci->lineFromPosition(position);
            sptr_t foldLevel = sci->foldLevel(line);
            if (foldLevel & SC_FOLDLEVELHEADERFLAG)
            {
                sci->toggleFold(line);
            }
        }
    }
}

void CodeEditor::setLanguage(const QString &lang)
{
    m_sc.initEditorStyle(this, lang);
}

void CodeEditor::setContent(const QString &content)
{
    auto b = content.toUtf8();
    setText(b.data());

    emptyUndoBuffer();
    m_sc.initEditorStyle(this);
    colourise(0, -1);
}
