#include "stdafx.h"
#include "codeinspector.h"

CodeInspector::CodeInspector(QWidget *parent)
    : ScintillaEdit (parent)
    , m_sc(this)
{
}

void CodeInspector::initialize()
{
    m_sc.initScintilla();
    m_sc.initInspectorMargins(false );
    m_sc.initLexerStyle("asm");

    connect(this, &ScintillaEdit::linesAdded, this, &CodeInspector::linesAdded);
    connect(this, &ScintillaEdit::marginClicked, this, &CodeInspector::marginClicked);

    setReadOnly(true);
}

void CodeInspector::linesAdded(int /*linesAdded*/)
{
    ScintillaEdit* sci = qobject_cast<ScintillaEdit*>(sender());
    sptr_t line_count = sci->lineCount();
    sptr_t left = sci->marginLeft() + 2;
    sptr_t right = sci->marginRight() + 2;
    sptr_t width = left + right + sci->textWidth(STYLE_LINENUMBER, QString("%1").arg(line_count).toStdString().c_str());
    if (width > sci->marginWidthN(0))
        sci->setMarginWidthN(0, width);
}

void CodeInspector::setContent(const QString &content, bool binary)
{
    auto b = content.toUtf8();
    setReadOnly(false);
    setText(b.data());
    setReadOnly(true);

    emptyUndoBuffer();
    m_sc.initLexerStyle("asm");
    m_sc.initInspectorMargins( binary );
    colourise(0, -1);
}

QMap<int, sptr_t> CodeInspector::setAsmItems(const QVector<AsmItem> &items, bool binary)
{
    const sptr_t marginStyleId = STYLE_LASTPREDEFINED + 1;
    styleSetBack(marginStyleId, 0xE4E4E4);
    styleSetFore(marginStyleId, 0x808080);
    styleSetSize(marginStyleId, 9);

    int textLength = 0;

    QMap<int, sptr_t> markerMap;
    int markerIndex = 0;
    m_sc.initMarkers();

    for (int i = 0; i < items.length(); i++)
    {
        const AsmItem& item  = items.at(i);

        if (item.source != -1)
        {
            auto it = markerMap.find(item.source);
            if (markerMap.end() == it)
            {
                // pick a color
                markerMap.insert(item.source, markerIndex);
                markerAdd(i, markerIndex);

                markerIndex++;
                if (markerIndex == 12)
                    markerIndex = 0;
            }
            else
            {
                markerAdd(i, it.value());
            }
        }

        if (binary)
        {
            QString text;
            QTextStream ts(&text) ;
            ts.setFieldWidth(2);
            ts.setIntegerBase(16);
            ts.setPadChar('0');
            if (item.address > 0)
            {
                ts << item.address;
            }

            if (!text.isEmpty())
                text.append(":");
            else
                text.append("00:");
            if (!item.opcodes.isEmpty())
            {
                for (const auto& opcode : item.opcodes)
                {
                    ts << opcode;
                }
            }

            //qDebug() << "binary: " << i << text;
            if (text.length() > 3)
            {
                marginSetStyle(i, marginStyleId);
                auto t = text.toUtf8();
                marginSetText(i, t.data());
                int tl = this->textWidth(marginStyleId, t.data());
                textLength = std::max(tl, textLength);
            }
        }
    }

    if (textLength)
        setMarginWidthN(1, textLength);
    for ( auto it = markerMap.begin(); it != markerMap.end(); ++it)
    {
        qDebug() << "source:" << it.key() << ", marker:" << it.value();
    }
    return markerMap;
}

QVariant CodeInspector::inputMethodQuery(Qt::InputMethodQuery /*query*/) const
{
    return QVariant(QRectF(0,0, 0, 0));
}
