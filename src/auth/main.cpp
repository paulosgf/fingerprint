#include "mainwindow.h"
#include <QPalette>
#include <QColor>
#include <QStyleFactory>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <iostream>
#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int createEnv()
{
    DIR *dirptr;
    char *home = getenv("HOME");

    dirptr = opendir(home);
    if (dirptr == NULL) {
        puts("User directory not found! Creating this...\n");
        printf("%s\n", home);
        if (mkdir(home, 0777) != 0)    {
            printf("Cannot create directory. Error: %s\n", strerror(errno));
            return 1;
        }
    } else {
        closedir(dirptr);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Define dark theme
    app.setStyleSheet(R"(
        QWidget {
            background-color: #1E1E1E;
            color: #C8C8C8;
        }
        QGraphicsView {
            background-color: #2A2A2A;  /* Fundo ligeiramente mais claro */
            border: 1px solid #444444; /* Borda para destacar o contorno */
        }
        QLineEdit, QTextEdit {
            background-color: #2D2D2D;
            color: #C8C8C8;
        }
        QPushButton {
            background-color: #2D2D2D;
            color: #C8C8C8;
            border: 1px solid #444444;
            border-radius: 4px;
            padding: 5px;
        }
        QPushButton:hover {
            background-color: #3C3C3C;
        }
        QAbstractItemView::item:selected {
            background-color: #6464C8;
            color: #FFFFFF;
        }
    )");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "fingerprint_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    MainWindow wdw;
    createEnv();
    wdw.show();
    return app.exec();
}
