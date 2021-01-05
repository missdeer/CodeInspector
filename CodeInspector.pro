TEMPLATE = subdirs

SUBDIRS = \
    scintilla \
    desktop-qt 


scintilla.subdir  = 3rdparty/scintilla/qt/ScintillaEdit

desktop-qt.depends = scintilla
