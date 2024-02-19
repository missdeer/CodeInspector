#include "codeinspector.h"

CodeInspector::CodeInspector(QWidget *parent) : ScintillaEdit(parent), m_sc(this) {}

void CodeInspector::initialize()
{
    m_sc.initScintilla();
    m_sc.initInspectorMargins(false);
    m_sc.initLexerStyle(QStringLiteral("asm"));

    connect(this, &ScintillaEdit::marginClicked, this, &CodeInspector::marginClicked);
    setReadOnly(true);
}

void CodeInspector::setContent(const QString &content, bool binary)
{
    auto b = content.toUtf8();
    setReadOnly(false);
    setText(b.data());
    setReadOnly(true);

    emptyUndoBuffer();
    // m_sc.initLexerStyle("asm");
    m_sc.initInspectorMargins(binary);
    colourise(0, -1);
}

void CodeInspector::setAsmItems(const AsmItemList &items, bool binary, QMap<int, intptr_t> &markerMap)
{
    const intptr_t marginStyleId = STYLE_LINENUMBER;
    intptr_t       textLength    = 0;
    intptr_t       markerIndex   = 0;
    m_sc.initMarkers();
    annotationSetVisible(ANNOTATION_HIDDEN);

    for (int index = 0; index < items.length(); index++)
    {
        const auto &item = items.at(index);

        if (item->source != -1)
        {
            auto iter = markerMap.find(item->source);
            if (markerMap.end() == iter)
            {
                // pick a color
                markerMap.insert(item->source, markerIndex);
                markerAdd(index, markerIndex);

                markerIndex++;
                if (markerIndex == markerMap.size())
                {
                    markerIndex = 0;
                }
            }
            else
            {
                markerAdd(index, iter.value());
            }
        }

        if (binary)
        {
            QString     addressText;
            QTextStream ts(&addressText);
            ts.setFieldWidth(2);
            ts.setIntegerBase(16);
            ts.setPadChar('0');
            if (item->address > 0)
            {
                ts << item->address;
            }

            if (!addressText.isEmpty())
            {
                marginSetStyle(index, marginStyleId);
                auto t = " " + addressText.toUtf8();
                marginSetText(index, t.data());
                auto tl    = textWidth(marginStyleId, (t + " ").data());
                textLength = std::max(tl, textLength);
            }

            if (!item->opcodes.isEmpty())
            {
                QString     opcodeText;
                QTextStream ts(&opcodeText);
                for (const auto &opcode : qAsConst(item->opcodes))
                {
                    QByteArray ba(1, opcode);
                    ts << ba.toHex() + " ";
                }
                annotationSetText(index, opcodeText.toUtf8().data());
                annotationSetStyle(index, 3);
            }
        }
    }

    if (textLength)
    {
        setMarginWidthN(1, textLength);
    }

    if (!binary)
    {
        sptr_t left        = marginLeft() + 2;
        sptr_t right       = marginRight() + 2;
        auto   lineNumText = QString::number(lineCount()).toUtf8();
        sptr_t width       = left + right + textWidth(STYLE_LINENUMBER, lineNumText.data());
        if (width > 32)
        {
            setMarginWidthN(0, width);
        }
    }
    else
    {
        annotationSetVisible(ANNOTATION_STANDARD);
    }

#if defined(LOGS_ENABLED)
    for (auto it = markerMap.begin(); it != markerMap.end(); ++it)
    {
        qDebug() << "source:" << it.key() << ", marker:" << it.value();
    }
#endif
}

QVariant CodeInspector::inputMethodQuery(Qt::InputMethodQuery /*query*/) const
{
    // so that it won't show input method pane on mobile device
    return {QRectF(0, 0, 0, 0)};
}
