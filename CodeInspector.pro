TEMPLATE = subdirs

SUBDIRS =  \
    plugins \
    desktop-qt

desktop-qt.depends = plugins
