#include <QApplication>
#include "mainwindow.h"
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
    QApplication a(argc, argv);
    MainWindow w;
    createEnv();
    w.show();
    
    return a.exec();
}
