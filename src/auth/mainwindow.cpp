#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "../fpengine.h"

using namespace std;

char *home = strcat(getenv("HOME"), "/.fingerprint");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , timer(new QTimer(this))
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    // vars
    regSize = 0;

    // Open device
    on_OpenDevice();
    printf("Método on_OpenDevice executado\n");

    // Timer
    connect(timer, &QTimer::timeout, this, &MainWindow::on_VerifyMatch);
    printf("Timer iniciado\n");

    // Using QTimer to call slot when loading window
    QTimer::singleShot(0, this, &MainWindow::on_VerifyMatch);
    printf("Slot on_VerifyMatch conectado\n");

    // Cleanup routines on exit application
    connect(qApp, &QCoreApplication::aboutToQuit, this, &MainWindow::on_cleanup);
    printf("Rotina de saida on_cleanup conectada\n");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_VerifyMatch()
{
    printf("on_VerifyMatch\n");
    int msgWork=GetWorkMsg();
    int msgRet=GetRetMsg();
    char *ref2 = (char *) malloc(strlen(home) + strlen("/ref2.dat") + 1);
    sprintf(ref2, "%s%s", home, "/ref2.dat");

    timer->start(100);
    GenFpChar();
    printf("Timer iniciado; Iniciando captura da digital\n");

    switch(msgWork)
    {
    case FPM_PLACE:
        ui->labelStatus->setText("Place finger");
        break;
    case FPM_LIFT:
        ui->labelStatus->setText("Lift finger");
        break;
    case FPM_NEWIMAGE:
        ui->labelStatus->setText("Captured Image");
        {
            GetImageBmp(bmpData,&bmpSize);
            QPixmap * pm = new QPixmap();
            pm->loadFromData(bmpData,bmpSize,"bmp");
            QGraphicsScene * gs = new QGraphicsScene();
            gs->addPixmap(*pm);
            ui->graphicsView->setScene(gs);
        }
        break;
    case FPM_CAPTURE:
    case FPM_GENCHAR:
        {
            if(msgRet==0)
            {
                ui->labelStatus->setText("Verify Fail");
            }
            else
            {
                ui->labelStatus->setText("Verify OK");

                GetFpCharByGen(regPtr,&regSize);

                FILE* fp;
                try
                {
                    fp = fopen(ref2, "wb+");
                }
                catch(const std::runtime_error& re)
                {
                    // runtime_error
                    std::cout << ref2 << std::endl;
                    std::cerr << "Runtime error: " << re.what() << std::endl;
                }
                catch(const std::exception& ex)
                {
                    //  all exceptions except std::runtime_error
                    std::cout << ref2 << std::endl;
                    std::cerr << "Error occurred: " << ex.what() << std::endl;
                }
                catch(...)
                {
                    // catch any other errors
                    std::cout << ref2 << std::endl;
                    std::cerr << "Unknown failure occurred." << std::endl;
                }

                if (fp == NULL)
                {
                    ui->labelStatus->setText("Open File Fail");
                }
                else
                {
                    fwrite(regFile,256,1,fp);
                    fclose(fp);
                    free(ref2);
                }
            }
            timer->stop();
 /*           if(regSize>0)
            {
                QString strResult;
                int sc;
                sc=MatchTemplateOne(regFile,regSize,512);
                strResult.sprintf("Match Scope:%d",sc);
                ui->labelStatus->setText(strResult);
            }   */
        }
        break;
     }
}

void MainWindow::on_OpenDevice()
{
    CloseDevice();
    if(!OpenDevice())	//Open USB
    {
        ui->labelStatus->setText("Device Open Fail");
        sleep(1);
        exit(0);
    }
    printf("Dispositivo aberto\n");

    if(!LinkDevice(0))  // Connect USB
    {
        CloseDevice();
        ui->labelStatus->setText("Device Connection Fail");
        sleep(1);
        exit(0);
    }
    printf("Dispositivo conectado\n");
}

void MainWindow::on_CloseDevice()
{
   if (!CloseDevice())
   {
        ui->labelStatus->setText("Device Close Fail");
        sleep(1);
        exit(0);
   }
}

void MainWindow::on_cleanup()
{
    on_CloseDevice();
    printf("Dispositivo desconectado\n");
    exit(0);
}

