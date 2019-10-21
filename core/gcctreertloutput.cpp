#include "stdafx.h"

#include "gcctreertloutput.h"

#include "ScintillaEdit.h"
#include "scintillaconfig.h"

#include <QCheckBox>

GCCTreeRTLOutput::GCCTreeRTLOutput(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *topLayout = new QHBoxLayout();
    m_passes               = new QComboBox(this);
    topLayout->addWidget(m_passes);
    m_gccTree = new QCheckBox(tr("GCC Tree"), this);
    m_gccTree->setChecked(true);
    topLayout->addWidget(m_gccTree);
    m_rtl = new QCheckBox(tr("RTL"), this);
    m_rtl->setChecked(true);
    topLayout->addWidget(m_rtl);
    topLayout->setStretch(0, 1);
    topLayout->setSpacing(2);

    mainLayout->addLayout(topLayout);
    m_scintillaEdit = new ScintillaEdit(this);
    mainLayout->addWidget(m_scintillaEdit);
    m_sc = new ScintillaConfig(m_scintillaEdit);

    connect(m_passes, &QComboBox::currentTextChanged, this, &GCCTreeRTLOutput::onCurrentTextChanged);
    connect(m_gccTree, &QCheckBox::stateChanged, this, &GCCTreeRTLOutput::onStateChanged);
    connect(m_rtl, &QCheckBox::stateChanged, this, &GCCTreeRTLOutput::onStateChanged);
}

void GCCTreeRTLOutput::initialize()
{
    m_sc->initScintilla();
    m_sc->initLexerStyle("asm");

    m_scintillaEdit->setReadOnly(true);
}

void GCCTreeRTLOutput::setContent(const QString &content)
{
    auto b = content.toUtf8();
    m_scintillaEdit->setReadOnly(false);
    m_scintillaEdit->setText(b.data());
    m_scintillaEdit->setReadOnly(true);

    m_scintillaEdit->emptyUndoBuffer();
    m_sc->initLexerStyle("asm");
    m_scintillaEdit->colourise(0, -1);
}

void GCCTreeRTLOutput::setPasses(const QStringList &passes)
{
    bool diff = false;
    for (const auto &pass : passes)
    {
        if (m_passes->findText(pass) < 0)
        {
            diff = true;
            break;
        }
    }
    if (!diff)
        return;
    m_passes->clear();
    m_passes->addItem("");
    m_passes->addItems(passes);
}

void GCCTreeRTLOutput::setCurrentSelectedPass(const QString &pass)
{
    if (m_passes->currentText() != pass)
    {
        m_passes->setCurrentText(pass);
    }
}

QString GCCTreeRTLOutput::getCurrentSelectedPass()
{
    return m_passes->currentText();
}

bool GCCTreeRTLOutput::isGCCTreeEnabled()
{
    return m_gccTree->isChecked();
}

bool GCCTreeRTLOutput::isRTLEnabled()
{
    return m_rtl->isChecked();
}

void GCCTreeRTLOutput::onCurrentTextChanged(const QString &text)
{
    if (text.isEmpty())
        return;
    setContent(tr("<Compiling...>"));
    emit refresh();
}

void GCCTreeRTLOutput::onStateChanged(int)
{
    if (m_passes->currentText().isEmpty())
        return;
    setContent(tr("<Compiling...>"));
    emit refresh();
}
