#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>
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
    ref1Size = 0;
    ref2Size = 0;

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
    char *ref1 = (char *) malloc(strlen(home) + strlen("/ref1.dat") + 1);
    sprintf(ref1, "%s%s", home, "/ref1.dat");
    char *ref2 = (char *) malloc(strlen(home) + strlen("/ref2.dat") + 1);
    sprintf(ref2, "%s%s", home, "/ref2.dat");
    int ref1size = filesystem::file_size(ref1);
    printf("ref1 = %d bytes\n", ref1size);

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
//                ui->labelStatus->setText("Verify OK");

                // Get matching pattern
                GetFpCharByGen(ref2File,&ref2Size);
                FILE* fp2;

                try
                {
                    fp2 = fopen(ref2, "wb+");
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

                if (fp2 == NULL)
                {
                    ui->labelStatus->setText("Open File Fail");
                }
                else
                {
                    fwrite(ref2File,256,1,fp2);
                    fclose(fp2);
                    free(ref2);
                }

                // Get reference template
                GetFpCharByEnl(ref1File,&ref1Size);
                FILE* fp1;

                try
                {
                    fp1 = fopen(ref1, "ab+");
                }
                catch(const std::runtime_error& re)
                {
                    // runtime_error
                    std::cout << ref1 << std::endl;
                    std::cerr << "Runtime error: " << re.what() << std::endl;
                }
                catch(const std::exception& ex)
                {
                    //  all exceptions except std::runtime_error
                    std::cout << ref1 << std::endl;
                    std::cerr << "Error occurred: " << ex.what() << std::endl;
                }
                catch(...)
                {
                    // catch any other errors
                    std::cout << ref1 << std::endl;
                    std::cerr << "Unknown failure occurred." << std::endl;
                }

                if (fp1 == NULL)
                {
                    ui->labelStatus->setText("Open File Fail");
                }
                else
                {
                    fread(ref1File,256,1,fp1);
                    fclose(fp1);
                    free(ref1);
                }
            }
            timer->stop();

            printf("ref1 = %d bytes\n", ref1size);
            // % match between sample and template
            if(ref1size>0)
            {
                QString strResult;
                int MatchScore =0;

                MatchScore=MatchTemplateOne(ref2File,ref1File,512);
                strResult.sprintf("Match Scope:%d",MatchScore);
                ui->labelStatus->setText(strResult);
            }
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

