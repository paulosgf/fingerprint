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
    unsigned char regPtr[512];
    unsigned char regFile[512];
    unsigned char bmpData[93238];
    int regSize;
    int bmpSize;

private slots:
    void __registryCapture();
    void __OpenDevice();
    void __CloseDevice();
    void on_pushButton_Registry_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
