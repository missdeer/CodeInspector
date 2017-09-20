#include "stdafx.h"
#include "qmldialog.h"
#include "ui_qmldialog.h"

QmlDialog::QmlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QmlDialog)
{
    ui->setupUi(this);
    ui->quickWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->quickWidget->setAttribute(Qt::WA_AlwaysStackOnTop, true);
    ui->quickWidget->setClearColor(Qt::transparent);
    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
}

QmlDialog::~QmlDialog()
{
    delete ui;
}

void QmlDialog::loadQml(const QUrl &u)
{
    ui->quickWidget->setSource(u);
}
