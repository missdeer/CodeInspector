#include <QLineEdit>
#include <QVBoxLayout>

#include "includewhatyouuseoutput.h"
#include "ScintillaEdit.h"
#include "scintillaconfig.h"

IncludeWhatYouUseOutput::IncludeWhatYouUseOutput(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    m_toolOptions = new QLineEdit(this);
    m_toolOptions->setPlaceholderText(tr("X86To6502 Input"));
    m_toolOptions->setClearButtonEnabled(true);
    mainLayout->addWidget(m_toolOptions);
    m_scintillaEdit = new ScintillaEdit(this);
    mainLayout->addWidget(m_scintillaEdit);
    m_sc = new ScintillaConfig(m_scintillaEdit);
    connect(m_toolOptions, &QLineEdit::textChanged, this, &IncludeWhatYouUseOutput::optionsChanged);
}

IncludeWhatYouUseOutput::~IncludeWhatYouUseOutput()
{
    delete m_sc;
}

void IncludeWhatYouUseOutput::initialize()
{
    m_sc->initScintilla();
    m_sc->initLexerStyle("asm");

    m_scintillaEdit->setReadOnly(true);
}

void IncludeWhatYouUseOutput::setContent(const QString &content)
{
    auto b = content.toUtf8();
    m_scintillaEdit->setReadOnly(false);
    m_scintillaEdit->setText(b.data());
    m_scintillaEdit->setReadOnly(true);

    m_scintillaEdit->emptyUndoBuffer();
    m_scintillaEdit->colourise(0, -1);
}

QString IncludeWhatYouUseOutput::getToolOptions()
{
    return m_toolOptions->text();
}
