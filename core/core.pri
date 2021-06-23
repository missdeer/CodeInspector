INCLUDEPATH += $$PWD \
    $$PWD/../3rdparty/scintilla/qt/ScintillaEdit \
    $$PWD/../3rdparty/scintilla/qt/ScintillaEditBase \
    $$PWD/../3rdparty/scintilla/include \
    $$PWD/../3rdparty/scintilla/src

DEFINES += SCINTILLA_QT=1 SCI_LEXER=1 _CRT_SECURE_NO_DEPRECATE=1 SCI_STATIC_LINK=1 EXPORT_IMPORT_API=
