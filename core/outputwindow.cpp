

#include "outputwindow.h"

OutputWindow::OutputWindow(QWidget *parent) : ScintillaEdit(parent), m_sc(this) {}

void OutputWindow::initialize()
{
    m_sc.initScintilla();
    m_sc.inttOuputWindowMargins();
    m_sc.initLexerStyle(QStringLiteral("cpp"));

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

    emptyUndoBuffer();
}

QVariant OutputWindow::inputMethodQuery(Qt::InputMethodQuery /*query*/) const
{
    return QVariant(QRectF(0, 0, 0, 0));
}
