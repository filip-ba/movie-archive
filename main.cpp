#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>


int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "MovieArchiveApp_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;

    w.setWindowTitle("Movie Archive");
    w.setWindowIcon(QIcon(":/icons/icon.png"));

    w.show();
    return a.exec();
}
