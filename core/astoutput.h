#ifndef ASTOUTPUT_H
#define ASTOUTPUT_H

#include "ScintillaEdit.h"
#include "scintillaconfig.h"

#include <QObject>

class ASTOutput : public ScintillaEdit
{
    Q_OBJECT
  public:
    explicit ASTOutput(QWidget *parent = nullptr);
    void initialize();
    void setContent(const QString &content);

  protected:
    QVariant inputMethodQuery(Qt::InputMethodQuery) const override;

  private:
    ScintillaConfig m_sc;
};

#endif // ASTOUTPUT_H
