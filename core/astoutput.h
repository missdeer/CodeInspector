#ifndef ASTOUTPUT_H
#define ASTOUTPUT_H

#include <QObject>
#include "ScintillaEdit.h"
#include "scintillaconfig.h"

class ASTOutput : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit ASTOutput(QWidget *parent = nullptr);
    void initialize();
    void setContent(const QString& content);
private:
    ScintillaConfig m_sc;
};

#endif // ASTOUTPUT_H
