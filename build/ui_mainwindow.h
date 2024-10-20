/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton_OpenDevice;
    QLabel *labelStatus;
    QPushButton *pushButton_Enroll;
    QPushButton *pushButton_Match;
    QPushButton *pushButton_CloseDevice;
    QGraphicsView *graphicsView;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(858, 603);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton_OpenDevice = new QPushButton(centralWidget);
        pushButton_OpenDevice->setObjectName(QString::fromUtf8("pushButton_OpenDevice"));
        pushButton_OpenDevice->setGeometry(QRect(10, 480, 121, 41));
        labelStatus = new QLabel(centralWidget);
        labelStatus->setObjectName(QString::fromUtf8("labelStatus"));
        labelStatus->setGeometry(QRect(10, 430, 581, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("Abyssinica SIL"));
        font.setPointSize(20);
        labelStatus->setFont(font);
        pushButton_Enroll = new QPushButton(centralWidget);
        pushButton_Enroll->setObjectName(QString::fromUtf8("pushButton_Enroll"));
        pushButton_Enroll->setGeometry(QRect(350, 480, 121, 41));
        pushButton_Match = new QPushButton(centralWidget);
        pushButton_Match->setObjectName(QString::fromUtf8("pushButton_Match"));
        pushButton_Match->setGeometry(QRect(480, 480, 121, 41));
        pushButton_CloseDevice = new QPushButton(centralWidget);
        pushButton_CloseDevice->setObjectName(QString::fromUtf8("pushButton_CloseDevice"));
        pushButton_CloseDevice->setGeometry(QRect(140, 480, 121, 41));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(220, 0, 351, 441));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(610, 480, 141, 41));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 858, 28));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Fingerprint Demo", nullptr));
        pushButton_OpenDevice->setText(QCoreApplication::translate("MainWindow", "Open Device", nullptr));
        labelStatus->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        pushButton_Enroll->setText(QCoreApplication::translate("MainWindow", "Enroll", nullptr));
        pushButton_Match->setText(QCoreApplication::translate("MainWindow", "Match", nullptr));
        pushButton_CloseDevice->setText(QCoreApplication::translate("MainWindow", "Close Device", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Match In File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
