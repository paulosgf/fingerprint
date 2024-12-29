#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include "../fpengine.h"

char *home = strcat(getenv("HOME"), "/.fingerprint");

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
    printf("Registry button clicked\n");
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
    char *ref1 = (char *) malloc(strlen(home) + strlen("/ref1.dat") + 1);
    sprintf(ref1, "%s%s", home, "/ref1.dat");
    char *ref2 = (char *) malloc(strlen(home) + strlen("/ref2.dat") + 1);
    sprintf(ref2, "%s%s", home, "/ref2.dat");

    printf("GetWorkMsg: %d\n", msgWork);
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
                    fwrite(m_genchar,256,1,fp);
                    fclose(fp);
                    free(ref2);
                }
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

                FILE* fp;
                try
                {
                    fp = fopen(ref1, "wb+");
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

                if (fp == NULL)
                {
                    ui->labelStatus->setText("Open File Fail");
                }
                else
                {
                    fwrite(m_enrolltp,512,1,fp);
                    fclose(fp);
                    free(ref1);
                }
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
    char *ref1 = (char *) malloc(strlen(home) + strlen("/ref1.dat") + 1);
    sprintf(ref1, "%s%s", home, "/ref1.dat");
    char *ref2 = (char *) malloc(strlen(home) + strlen("/ref2.dat") + 1);
    sprintf(ref2, "%s%s", home, "/ref2.dat");

    FILE* fp1;
    FILE* fp2;

    fp1 = fopen(ref1, "rb+");
    if (fp1 == NULL)
    {
        ui->labelStatus->setText("Open File Fail");
        return;
    }
    fp2 = fopen(ref2, "rb+");
    if (fp2 == NULL)
    {
        ui->labelStatus->setText("Open File Fail");
        return;
    }
    fread(Src,512,1,fp2);
    fread(Dst,256,1,fp1);
    fclose(fp2);
    free(ref2);
    fclose(fp1);
    free(ref1);

    MatchScore=MatchTemplateOne(Dst,Src,512);

    strResult.sprintf("Match Scope:%d",MatchScore);
    ui->labelStatus->setText(strResult);    
}

void MainWindow::on_pushButton_clicked()
{
    matchfile();
}
