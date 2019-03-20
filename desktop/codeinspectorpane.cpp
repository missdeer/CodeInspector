#include "stdafx.h"
#include "codeinspectorapp.h"
#include "godboltagent.h"
#include "codeinspectortabwidget.h"
#include "outputwindow.h"
#include "popupmenutoolbutton.h"
#include "codeinspectorpane.h"

CodeInspectorPane::CodeInspectorPane(QWidget *parent)
    : QWidget(parent)
    , m_backend(new GodboltAgent(ciApp->networkAccessManager(), this))
{
    auto *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    
    auto *topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(0,0,0,0);
    topBarLayout->setSpacing(0);
    
    m_compilerList = new QComboBox(this);
    topBarLayout->addWidget(m_compilerList);
    
    m_compilerArguments = new QLineEdit(this);
    m_compilerArguments->setPlaceholderText(tr("Build Options"));
    topBarLayout->addWidget(m_compilerArguments);
            
    QMenu* popupMenu = new QMenu(this);
    QAction* actionBinrary  = new QAction(tr("Binary"), this);
    actionBinrary->setIcon(QIcon(":/resource/image/binary.png"));
    actionBinrary->setToolTip(tr("Compile to binary and disassemble the output"));
    actionBinrary->setCheckable(true);
    connect(actionBinrary, &QAction::triggered, this, &CodeInspectorPane::onActionBinaryTriggered);
    popupMenu->addAction(actionBinrary);
    
    QAction* actionLabel  = new QAction(tr("Label"), this);
    actionLabel->setIcon(QIcon(":/resource/image/label.png"));
    actionLabel->setToolTip(tr("Filter unused labels from the output"));
    actionLabel->setCheckable(true);
    connect(actionLabel, &QAction::triggered, this, &CodeInspectorPane::onActionLabelTriggered);
    popupMenu->addAction(actionLabel);
    
    QAction* actionFunctions  = new QAction(tr("Functions"), this);
    actionFunctions->setIcon(QIcon(":/resource/image/function.png"));
    actionFunctions->setToolTip(tr("Filter functions from other libraries from the output"));
    actionFunctions->setCheckable(true);
    connect(actionFunctions, &QAction::triggered, this, &CodeInspectorPane::onActionFunctionsTriggered);
    popupMenu->addAction(actionFunctions);
    
    QAction* actionDirectives  = new QAction(tr("Directives"), this);
    actionDirectives->setIcon(QIcon(":/resource/image/directives.png"));
    actionDirectives->setToolTip(tr("Filter all assembler directives from the output"));
    actionDirectives->setCheckable(true);
    connect(actionDirectives, &QAction::triggered, this, &CodeInspectorPane::onActionDirectivesTriggered);
    popupMenu->addAction(actionDirectives);
    
    QAction* actionComments  = new QAction(tr("Comments"), this);
    actionComments->setIcon(QIcon(":/resource/image/comment.png"));
    actionComments->setToolTip(tr("Remove all lines which are only comments from the output"));
    actionComments->setCheckable(true);
    connect(actionComments, &QAction::triggered, this, &CodeInspectorPane::onActionCommentsTriggered);
    popupMenu->addAction(actionComments);
    
    QAction* actionTrim  = new QAction(tr("Trim"), this);
    actionTrim->setIcon(QIcon(":/resource/image/trim.png"));
    actionTrim->setToolTip(tr("Trim intra-line whitespace"));
    actionTrim->setCheckable(true);
    connect(actionTrim, &QAction::triggered, this, &CodeInspectorPane::onActionTrimTriggered);
    popupMenu->addAction(actionTrim);
    
    QAction* actionIntel  = new QAction(tr("Intel"), this);
    actionIntel->setIcon(QIcon(":/resource/image/intel.png"));
    actionIntel->setToolTip(tr("Output disassembly in Intel syntax"));
    actionIntel->setCheckable(true);
    connect(actionIntel, &QAction::triggered, this, &CodeInspectorPane::onActionIntelTriggered);
    popupMenu->addAction(actionIntel);
    
    QAction* actionDemangle  = new QAction(tr("Demangle"), this);
    actionDemangle->setIcon(QIcon(":/resource/image/demangle.png"));
    actionDemangle->setToolTip(tr("Demangle output"));
    actionDemangle->setCheckable(true);
    connect(actionDemangle, &QAction::triggered, this, &CodeInspectorPane::onActionDemangleTriggered);
    popupMenu->addAction(actionDemangle);
    
    m_btnInspectorOptions = new PopupMenuToolButton(this);
    m_btnInspectorOptions->setIcon(QIcon(":/resource/image/preferences-activities.png"));
    m_btnInspectorOptions->setMenu(popupMenu);
    topBarLayout->addWidget(m_btnInspectorOptions);
    
    topBarLayout->setStretch(0, 1);
    topBarLayout->setStretch(1, 1);
    mainLayout->addLayout(topBarLayout);
    
    m_splitter = new QSplitter(Qt::Vertical, this);
    mainLayout->addWidget(m_splitter);
    
    m_codeInspectorTabWidget = new CodeInspectorTabWidget(this);
    m_splitter->addWidget(m_codeInspectorTabWidget);
    
    QWidget *outputPane = new QWidget(this);
    
    auto* outputBarLayout = new QHBoxLayout();
    outputBarLayout->setContentsMargins(0,0,0,0);
    outputBarLayout->setSpacing(0);
    
    auto* outputPaneLayout = new QVBoxLayout();
    outputPaneLayout->setContentsMargins(0,0,0,0);
    outputPaneLayout->setSpacing(0);
    outputPane->setLayout(outputPaneLayout);
    
    m_btnToggleOutput = new QPushButton(QIcon(":/resource/image/errmsg.png"), "");
    m_btnToggleOutput->setIconSize(QSize(32, 32));
    m_btnToggleOutput->setFlat(true);
    m_btnToggleOutput->setToolTip(tr("Toggle output window"));
    outputBarLayout->addWidget(m_btnToggleOutput);
    outputBarLayout->addStretch(1);

    outputPaneLayout->addLayout(outputBarLayout);

    m_output = new OutputWindow(this);
    m_output->initialize();
    outputPaneLayout->addWidget(m_output);
    
    m_splitter->addWidget(outputPane);
    
    m_output->setVisible(false);
    m_btnToggleOutput->setVisible(false);
    m_splitter->setSizes(QList<int>() << 4096 << 0);
    
    m_backend->initialize(ciApp);
    connect(m_btnToggleOutput, &QPushButton::clicked, this, &CodeInspectorPane::onToggleOutput);
    connect(m_compilerList, &QComboBox::currentTextChanged, this, &CodeInspectorPane::currentCompilerChanged);
    connect(m_compilerArguments, &QLineEdit::textChanged, this, &CodeInspectorPane::currentCompilerArgumentsChanged);
}

void CodeInspectorPane::initialize()
{

}

void CodeInspectorPane::setCompilerList(CompilerListPtr cl)
{
    m_compilerList->clear();
    for (const auto c : *cl)
    {
        m_compilerList->addItem(c->name);
    }
}

void CodeInspectorPane::onToggleOutput()
{
    bool visible = !m_output->isVisible();
    m_output->setVisible(visible);
    if (!visible)
    {
        m_splitter->setSizes(QList<int>() << 4096 << m_btnToggleOutput->height());
    }
}

void CodeInspectorPane::onActionBinaryTriggered()
{
    
}

void CodeInspectorPane::onActionLabelTriggered()
{
    
}

void CodeInspectorPane::onActionFunctionsTriggered()
{
    
}

void CodeInspectorPane::onActionDirectivesTriggered()
{
    
}

void CodeInspectorPane::onActionCommentsTriggered()
{
    
}

void CodeInspectorPane::onActionTrimTriggered()
{
    
}

void CodeInspectorPane::onActionIntelTriggered()
{
    
}

void CodeInspectorPane::onActionDemangleTriggered()
{
    
}
