#ifndef CODEINSPECTOR_H
#define CODEINSPECTOR_H

#include "ScintillaEdit.h"
#include "scintillaconfig.h"
#include "godboltagent.h"

class CodeInspector : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit CodeInspector(QWidget *parent = 0);
    void initialize();
    void setContent(const QString& content);
    void setBinaryMode(bool binary);
    void setAsmItems(const QVector<AsmItem>& items);
private slots:
    void linesAdded(int linesAdded);
    void marginClicked(int position, int modifiers, int margin);
private:
    ScintillaConfig m_sc;
};

#endif // CODEINSPECTOR_H
