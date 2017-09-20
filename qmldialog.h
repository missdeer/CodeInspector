#ifndef QMLDIALOG_H
#define QMLDIALOG_H

#include <QDialog>

namespace Ui {
class QmlDialog;
}

class QmlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QmlDialog(QWidget *parent = 0);
    ~QmlDialog();

private:
    Ui::QmlDialog *ui;
};

#endif // QMLDIALOG_H
