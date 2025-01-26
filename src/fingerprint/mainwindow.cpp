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
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // vars
    regSize = 0;
    timer=new QTimer(this);

    on_OpenDevice();
    connect(timer, SIGNAL(timeout()), this, SLOT(on_RegistryCapture()));
}

MainWindow::~MainWindow()
{
    // Destructor

    // Free memory
    free(ref1);
    delete ui;
}

void MainWindow::on_RegistryCapture()
{
    int msgWork=GetWorkMsg();
    int msgRet=GetRetMsg();
    ref1 = (char *) malloc(strlen(home) + strlen("/ref1.dat") + 1);
    sprintf(ref1, "%s%s", home, "/ref1.dat");

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
    case FPM_ENRFPT:
        {
        if(msgRet==0)
            {
            ui->labelStatus->setText("Register Fail");
            }
        else {
            GetFpCharByEnl(regPtr,&regSize);
            ui->labelStatus->setText("Register OK");

            FILE* fp;
            try
            {
                    fp = fopen(ref1, "wb");
            }
            catch(const runtime_error& re)
            {
                // runtime_error
                cout << ref1 << endl;
                cerr << "Runtime error: " << re.what() << endl;
            }
            catch(const exception& ex)
            {
                //  all exceptions except std::runtime_error
                cout << ref1 << endl;
                cerr << "Error occurred: " << ex.what() << endl;
            }
            catch(...)
            {
                // catch any other errors
                cout << ref1 << endl;
                cerr << "Error occurred: " << endl;
            }
            if (fp == NULL)
            {
                ui->labelStatus->setText("Open Database Fail");
            }
            else
            {
                    fwrite(regPtr,512,1,fp);
                    fclose(fp);
            }
            timer->stop();
        }
         break;
     }

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

    if(!LinkDevice(0))  // Connect USB
    {
        CloseDevice();
        ui->labelStatus->setText("Device Connection Fail");
        sleep(1);
        exit(0);
    }
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

void MainWindow::on_pushButton_Registry_clicked()
{
    timer->start(100);
    EnrolFpChar();
}

void MainWindow::on_pushButton_Exit_clicked()
{
    on_CloseDevice();
    exit(0);
}

