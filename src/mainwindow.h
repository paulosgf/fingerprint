#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void matchfile();
private:
    QTimer * timer;
    int	m_IsOpen;
    int	m_Status;
    int	m_Result;
    int  m_ProcType;
    int m_gencharsize;
    unsigned char m_genchar[512];
    int m_enrollsize;
    unsigned char m_enrolltp[512];
    unsigned char m_BmpData[93238];
    int m_BmpSize;

private slots:
    void on_pushButton_OpenDevice_clicked();

    void on_pushButton_CloseDevice_clicked();

    void onTimer();

    void on_pushButton_Enroll_clicked();

    void on_pushButton_Match_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
