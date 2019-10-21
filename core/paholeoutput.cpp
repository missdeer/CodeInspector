#include "stdafx.h"

#include "paholeoutput.h"

#include "ScintillaEdit.h"
#include "scintillaconfig.h"

PaholeOutput::PaholeOutput(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    m_toolOptions = new QLineEdit(this);
    m_toolOptions->setPlaceholderText(tr("Pahole Options"));
    m_toolOptions->setClearButtonEnabled(true);
    mainLayout->addWidget(m_toolOptions);
    m_scintillaEdit = new ScintillaEdit(this);
    mainLayout->addWidget(m_scintillaEdit);
    m_sc = new ScintillaConfig(m_scintillaEdit);
    connect(m_toolOptions, &QLineEdit::textChanged, this, &PaholeOutput::optionsChanged);
}

void PaholeOutput::initialize()
{
    m_sc->initScintilla();
    m_sc->initLexerStyle("asm");

    m_scintillaEdit->setReadOnly(true);
}

void PaholeOutput::setContent(const QString &content)
{
    auto b = content.toUtf8();
    m_scintillaEdit->setReadOnly(false);
    m_scintillaEdit->setText(b.data());
    m_scintillaEdit->setReadOnly(true);

    m_scintillaEdit->emptyUndoBuffer();
    m_sc->initLexerStyle("asm");
    m_scintillaEdit->colourise(0, -1);
}

QString PaholeOutput::getToolOptions()
{
    return m_toolOptions->text();
}
