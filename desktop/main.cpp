#include "stdafx.h"
#include <QSplashScreen>
#include "codeinspectorapp.h"
#include "mainwindow.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QApplication::setApplicationName("CodeInspector");
    QApplication::setApplicationVersion("1.0");

    QApplication a(argc, argv);
    
    QPixmap pixmap(":/CodeInspector.png");
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();
    
    splash.showMessage(QObject::tr("Creating application instance..."));
    CodeInspectorApp app;    
    ciApp = &app;
    
    splash.showMessage(QObject::tr("Initializing application instance..."));
    ciApp->initialize();

    Settings settings;
    g_settings = &settings;
    
    splash.showMessage(QObject::tr("Reading settings..."));
    g_settings->initialize();

    splash.showMessage(QObject::tr("Creating main window..."));
    MainWindow w;
    w.showMaximized();
    
    splash.finish(&w);
    return a.exec();
}
