#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "fpengine.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_IsOpen=0;
    m_Status=0;
    m_Result=0;
    m_ProcType=0;
    m_gencharsize=0;
    m_enrollsize=0;

    ui->setupUi(this);

    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_OpenDevice_clicked()
{
    CloseDevice();

    if(OpenDevice())	//Open USB
    {
        ui->labelStatus->setText("Open USB Device Success");
        //FPDoUserDefine(0,1);
        //LedControl(1);
    }
    else
    {
        ui->labelStatus->setText("Open Device Fail");
        return;
    }

    if(!LinkDevice(0))
    {
        CloseDevice();
        ui->labelStatus->setText("Verify pwd fail");
        return;
    }

    ui->labelStatus->setText("Link Device Success");
}

void MainWindow::on_pushButton_CloseDevice_clicked()
{
    CloseDevice();
    ui->labelStatus->setText("Close Device Success");    
}


void MainWindow::on_pushButton_Enroll_clicked()
{
    timer->start(100);
    EnrolFpChar();
}

void MainWindow::on_pushButton_Match_clicked()
{
    timer->start(100);
    GenFpChar();
}

void MainWindow::onTimer()
{
    int msgWork=GetWorkMsg();
    int msgRet=GetRetMsg();
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
            /*
            SaveImageToFile("./finger.bmp");
            QPixmap * pm = new QPixmap("./finger.bmp");
            QGraphicsScene * gs = new QGraphicsScene();
            gs->addPixmap(*pm);
            ui->graphicsView->setScene(gs);            
            */
            GetImageBmp(m_BmpData,&m_BmpSize);
            QPixmap * pm = new QPixmap();
            pm->loadFromData(m_BmpData,m_BmpSize,"bmp");
            QGraphicsScene * gs = new QGraphicsScene();
            gs->addPixmap(*pm);
            ui->graphicsView->setScene(gs);
        }
        break;
    case FPM_GENCHAR:
        {
            if(msgRet==0)
            {
                ui->labelStatus->setText("GenChar Fail");
            }
            else
            {
                ui->labelStatus->setText("GenChar OK");

                GetFpCharByGen(m_genchar,&m_gencharsize);

                ///*
                FILE* fp;
                fp = fopen("/usr/local/fingerprint/ref2.dat", "wb+");
                if (fp == NULL)
                {
                    ui->labelStatus->setText("Open File Fail");
                }
                else
                {
                    fwrite(m_genchar,256,1,fp);
                    fclose(fp);
                }
                //*/
            }
            timer->stop();
            if(m_enrollsize>0)
            {
                QString strResult;
                int sc;

                sc=MatchTemplateOne(m_genchar,m_enrolltp,512);

                strResult.sprintf("Match Scope:%d",sc);
                ui->labelStatus->setText(strResult);
            }
        }
        break;
    case FPM_ENRFPT:
        {
            if(msgRet==0)
            {
                ui->labelStatus->setText("Enroll Fail");
            }
            else
            {
                ui->labelStatus->setText("Enroll OK");

                GetFpCharByEnl(m_enrolltp,&m_enrollsize);

                ///*
                FILE* fp;
                fp = fopen("/usr/local/fingerprint/ref1.dat", "wb+");
                if (fp == NULL)
                {
                    ui->labelStatus->setText("Open File Fail");
                }
                else
                {
                    fwrite(m_enrolltp,512,1,fp);
                    fclose(fp);
                }
                //*/
            }
            timer->stop();
        }
        break;
    }
}

void MainWindow::matchfile()
{
    QString strResult;
    int MatchScore;

    unsigned char Src[512];
    unsigned char Dst[256];

    FILE* fp1;
    FILE* fp2;
    fp1 = fopen("/usr/local/fingerprint/ref1.dat", "rb+");
    if (fp1 == NULL)
    {
        ui->labelStatus->setText("Open File Fail");
        return;
    }
    fp2 = fopen("/usr/local/fingerprint/ref2.dat", "rb+");
    if (fp2 == NULL)
    {
        ui->labelStatus->setText("Open File Fail");
        return;
    }
    fread(Src,512,1,fp2);
    fread(Dst,256,1,fp1);
    fclose(fp2);
    fclose(fp1);

    MatchScore=MatchTemplateOne(Dst,Src,512);

    strResult.sprintf("Match Scope:%d",MatchScore);
    ui->labelStatus->setText(strResult);
}

void MainWindow::on_pushButton_clicked()
{
    matchfile();
}
