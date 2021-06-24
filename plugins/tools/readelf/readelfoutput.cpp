#include <QLineEdit>
#include <QVBoxLayout>

#include "readelfoutput.h"
#include "ScintillaEdit.h"
#include "scintillaconfig.h"

ReadElfOutput::ReadElfOutput(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    m_toolOptions = new QLineEdit(this);
    m_toolOptions->setPlaceholderText(tr("ReadElf Input"));
    m_toolOptions->setClearButtonEnabled(true);
    mainLayout->addWidget(m_toolOptions);
    m_scintillaEdit = new ScintillaEdit(this);
    mainLayout->addWidget(m_scintillaEdit);
    m_sc = new ScintillaConfig(m_scintillaEdit);
    connect(m_toolOptions, &QLineEdit::textChanged, this, &ReadElfOutput::optionsChanged);
}

ReadElfOutput::~ReadElfOutput()
{
    delete m_sc;
}

void ReadElfOutput::initialize()
{
    m_sc->initScintilla();
    m_sc->initLexerStyle("asm");

    m_scintillaEdit->setReadOnly(true);
}

void ReadElfOutput::setContent(const QString &content)
{
    auto b = content.toUtf8();
    m_scintillaEdit->setReadOnly(false);
    m_scintillaEdit->setText(b.data());
    m_scintillaEdit->setReadOnly(true);

    m_scintillaEdit->emptyUndoBuffer();
    m_scintillaEdit->colourise(0, -1);
}

QString ReadElfOutput::getToolOptions()
{
    return m_toolOptions->text();
}
