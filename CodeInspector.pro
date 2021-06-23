TEMPLATE = subdirs

SUBDIRS =  \
    core \
    plugins \
    desktop-qt

plugins.depends = core
desktop-qt.depends = plugins
