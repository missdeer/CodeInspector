#include "codeeditor.h"
#include "ScintillaTypes.h"

CodeEditor::CodeEditor(QWidget *parent) : ScintillaEdit(parent), m_sc(this) {}

void CodeEditor::initialize()
{
    connect(this, &ScintillaEditBase::linesAdded, this, &CodeEditor::onLinesAdded);
    connect(this, &ScintillaEditBase::marginClicked, this, &CodeEditor::onMarginClicked);
    connect(this, &ScintillaEditBase::modified, this, &CodeEditor::onModified);
#if defined(Q_OS_ANDROID)
    connect(this, &ScintillaEdit::textAreaClicked, [=] {
        auto im = qApp->inputMethod();
        if (!im->isVisible())
            im->show();
    });
#endif
}

void CodeEditor::onLinesAdded(Scintilla::Position /*linesAdded*/)
{
    auto  *sci        = qobject_cast<ScintillaEdit *>(sender());
    sptr_t line_count = sci->lineCount();
    sptr_t left       = sci->marginLeft() + 2;
    sptr_t right      = sci->marginRight() + 2;
    sptr_t width      = left + right + sci->textWidth(STYLE_LINENUMBER, std::to_string(line_count).c_str());
    if (width > sci->marginWidthN(0))
    {
        sci->setMarginWidthN(0, width);
    }
}

void CodeEditor::onMarginClicked(Scintilla::Position position, Scintilla::KeyMod /*modifiers*/, int margin)
{
    auto *sci = qobject_cast<ScintillaEdit *>(sender());
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

void CodeEditor::onModified(Scintilla::ModificationFlags type,
                            Scintilla::Position /*position*/,
                            Scintilla::Position /*length*/,
                            Scintilla::Position /*linesAdded*/,
                            const QByteArray & /*text*/,
                            Scintilla::Position /*line*/,
                            Scintilla::FoldLevel /*foldNow*/,
                            Scintilla::FoldLevel /*foldPrev*/)
{
    if (static_cast<int>(type) &
        static_cast<int>(Scintilla::ModificationFlags::InsertText | Scintilla::ModificationFlags::DeleteText | Scintilla::ModificationFlags::Undo |
                         Scintilla::ModificationFlags::Redo | Scintilla::ModificationFlags::MultilineUndoRedo))
    {
        emit contentModified();
    }
}

void CodeEditor::setLanguage(const QString &lang)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO << __LINE__ << lang;
#endif
    if (!lang.isEmpty())
    {
        m_sc.initScintilla();
        m_sc.initEditorMargins();
        m_sc.initLexerStyle(lang);
        m_sc.initEditorFolderStyle();
        colourise(0, -1);
    }
}

void CodeEditor::setContent(const QString &content)
{
    auto barr = content.toUtf8();
    setContent(barr);
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

    QMapIterator<int, intptr_t> iter(markerColor);
    while (iter.hasNext())
    {
        iter.next();
        markerAdd(iter.key() - 1, iter.value());
    }
}
