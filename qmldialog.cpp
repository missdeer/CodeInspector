#include "stdafx.h"
#include "qmldialog.h"
#include "ui_qmldialog.h"

QmlDialog::QmlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QmlDialog)
{
    ui->setupUi(this);
}

QmlDialog::~QmlDialog()
{
    delete ui;
}

void QmlDialog::loadQml(const QUrl &u)
{
    ui->quickWidget->setSource(u);
}
