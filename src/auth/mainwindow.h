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
    static void initializeGlobals(const char *homef);
    void on_VerifyMatch();
    int on_Compare();
    int getMatchScore() const;

private:
    QTimer * timer;
    unsigned char bmpData[93238];
    unsigned char ref2File[512];
    unsigned char ref1File[512];
    int ref1Size;
    int ref2Size;
    int bmpSize;
    int MatchScore;

private slots:
    void on_OpenDevice();
    void on_CloseDevice();
    void on_cleanup();
    void on_NewImage();
    void on_GetCapture();

public slots:

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
