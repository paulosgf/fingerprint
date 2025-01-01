#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTimer * timer;
    unsigned char ref2File[512];
    unsigned char ref1File[512];
    unsigned char bmpData[93238];
    int ref1Size;
    int ref2Size;
    int bmpSize;

private slots:

    void on_OpenDevice();
    void on_CloseDevice();
    void on_cleanup();

public slots:
    void on_VerifyMatch();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
