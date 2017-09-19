#include "outputwindow.h"

OutputWindow::OutputWindow(QWidget *parent)
    : ScintillaEdit (parent)
    , m_sc(this)
{

}

void OutputWindow::initialize()
{
    m_sc.initScintilla();
    m_sc.initEditorMargins();
    m_sc.initLexerStyle("cpp");

    setMarginWidthN(2, 0);

    setReadOnly(true);
}

void OutputWindow::setContent(const QString &content)
{
    auto b = content.toUtf8();
    setContent(b);
}

void OutputWindow::setContent(const QByteArray &content)
{
    setReadOnly(false);
    setText(content.data());
    setReadOnly(true);

    int linesAdded = lineCount();
    sptr_t left = marginLeft() + 2;
    sptr_t right = marginRight() + 2;
    auto b = QString("%1").arg(linesAdded).toUtf8();
    sptr_t width = left + right + textWidth(STYLE_LINENUMBER, b.data());
    if (width > 32)
        setMarginWidthN(0, width);

    emptyUndoBuffer();
}

QVariant OutputWindow::inputMethodQuery(Qt::InputMethodQuery /*query*/) const
{
    return QVariant(QRectF(0,0, 0, 0));
}
