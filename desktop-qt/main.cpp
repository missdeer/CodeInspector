#include "stdafx.h"

#include <QMessageBox>
#include <QSplashScreen>
#include <QSslSocket>

#include "codeinspectorapp.h"
#include "mainwindow.h"
#include "settings.h"

int main(int argc, char *argv[])
{
#if QT_VERSION_MAJOR <= 5
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif

    QApplication::setApplicationName("CodeInspector");
    QApplication::setApplicationVersion("1.0");

    QApplication a(argc, argv);

    QPixmap       pixmap(":/CodeInspector.png");
    QSplashScreen splash(pixmap);
    splash.show();
    QApplication::processEvents();
    Settings settings;
    g_settings = &settings;

    splash.showMessage(QObject::tr("Reading settings..."));
    g_settings->initialize();

    if (g_settings->sslRequired() && !QSslSocket::supportsSsl())
    {
        QMessageBox::critical(nullptr, QObject::tr("Critical error"), QObject::tr("SSL not supported, exit now."), QMessageBox::Ok);
        return 1;
    }

    splash.showMessage(QObject::tr("Creating application instance..."));
    CodeInspectorApp app;
    ciApp = &app;

    splash.showMessage(QObject::tr("Initializing application instance..."));
    ciApp->initialize();

    splash.showMessage(QObject::tr("Creating main window..."));
    MainWindow w;
    w.showMaximized();

    splash.finish(&w);
    return QApplication::exec();
}