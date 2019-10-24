#include "stdafx.h"

#include "codeeditor.h"

CodeEditor::CodeEditor(QWidget *parent) : ScintillaEdit(parent), m_sc(this) {}

void CodeEditor::initialize()
{
    m_sc.initScintilla();
    m_sc.initEditorMargins();
    m_sc.initLexerStyle("cpp");
    m_sc.initEditorFolderStyle();

    connect(this, &ScintillaEdit::linesAdded, this, &CodeEditor::linesAdded);
    connect(this, &ScintillaEdit::marginClicked, this, &CodeEditor::marginClicked);
    connect(this, &ScintillaEdit::modified, this, &CodeEditor::modified);
#if defined(Q_OS_ANDROID)
    connect(this, &ScintillaEdit::textAreaClicked, [=] {
        auto im = qApp->inputMethod();
        if (!im->isVisible())
            im->show();
    });
#endif
}

void CodeEditor::linesAdded(int /*linesAdded*/)
{
    ScintillaEdit *sci        = qobject_cast<ScintillaEdit *>(sender());
    sptr_t         line_count = sci->lineCount();
    sptr_t         left       = sci->marginLeft() + 2;
    sptr_t         right      = sci->marginRight() + 2;
    sptr_t         width      = left + right + sci->textWidth(STYLE_LINENUMBER, QString("%1").arg(line_count).toStdString().c_str());
    if (width > sci->marginWidthN(0))
        sci->setMarginWidthN(0, width);
}

void CodeEditor::marginClicked(int position, int /*modifiers*/, int margin)
{
    ScintillaEdit *sci = qobject_cast<ScintillaEdit *>(sender());
    if (sci->marginTypeN(margin) == SC_MARGIN_SYMBOL)
    {
        sptr_t maskN = sci->marginMaskN(margin);
        if ((maskN & 0xFFFFFFFF) == SC_MASK_FOLDERS)
        {
            sptr_t line      = sci->lineFromPosition(position);
            sptr_t foldLevel = sci->foldLevel(line);
            if (foldLevel & SC_FOLDLEVELHEADERFLAG)
            {
                sci->toggleFold(line);
            }
        }
    }
}

void CodeEditor::modified(
    int type, int /*position*/, int /*length*/, int /*linesAdded*/, const QByteArray & /*text*/, int /*line*/, int /*foldNow*/, int /*foldPrev*/)
{
    if (type & (SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT))
        emit contentModified();
}

void CodeEditor::setLanguage(const QString &lang)
{
    m_sc.initScintilla();
    m_sc.initEditorMargins();
    m_sc.initLexerStyle(lang);
    m_sc.initEditorFolderStyle();
    colourise(0, -1);
}

void CodeEditor::setContent(const QString &content)
{
    auto b = content.toUtf8();
    setContent(b);
}

void CodeEditor::setContent(const QByteArray &content)
{
    setText(content.data());

    emptyUndoBuffer();
}

void CodeEditor::clearContent()
{
    setText("");

    emptyUndoBuffer();
}

void CodeEditor::setMarkerColor(const QMap<int, intptr_t> &markerColor)
{
    m_sc.initMarkers();

    QMapIterator<int, intptr_t> it(markerColor);
    while (it.hasNext())
    {
        it.next();
        markerAdd(it.key() - 1, it.value());
    }
}
