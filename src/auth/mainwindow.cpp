#include "./mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <unistd.h>
#include "../fpengine.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    // Constructor
    : QMainWindow(parent)
    , timer(new QTimer(this))
    , home(nullptr)
    , ref1(nullptr)
    , ref2(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Application directory
    home = strcat(getenv("HOME"), "/.fingerprint");

    // Global vars
    initializeGlobals();
    // vars
    ref1Size = 0;
    ref2Size = 0;

    // Open device
    on_OpenDevice();

    // Timer
    connect(timer, &QTimer::timeout, this, &MainWindow::on_VerifyMatch);

    // Using QTimer to call slot when loading window
    QTimer::singleShot(0, this, &MainWindow::on_VerifyMatch);

    // Cleanup routines on exit application
    connect(qApp, &QCoreApplication::aboutToQuit, this, &MainWindow::on_cleanup);
}

MainWindow::~MainWindow()
{
    // Destructor

    // Free memory
    free(ref1);
    free(ref2);
    delete ui;
}

void MainWindow::initializeGlobals() {
    ref1 = (char *) malloc(strlen(home) + strlen("/ref1.dat") + 1);
    sprintf(ref1, "%s%s", home, "/ref1.dat");
    ref2 = (char *) malloc(strlen(home) + strlen("/ref2.dat") + 1);
    sprintf(ref2, "%s%s", home, "/ref2.dat");
}

void MainWindow::on_VerifyMatch()
{
    // Verify match method
    MatchScore = 0;
    int msgWork=GetWorkMsg();
    int msgRet=GetRetMsg();

//    MainWindow::initializeGlobals(home);
    int ref1size = filesystem::file_size(ref1);

    if(ref1size>0) {
            timer->start(100);
            GenFpChar();

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
                    on_NewImage();
                }
                break;
            case FPM_GENCHAR:
                {
                    if(msgRet==0)
                    {
                        ui->labelStatus->setText("Verify Fail");
                    }
                    else
                    {
                        // Get sample
                        on_GetCapture();
                    }
                    // % match between sample and template
                    MatchScore = on_Compare();
                    if (MatchScore>=100)
                     {
                        timer->stop();
                        break;
                     }
                }
             }
    }
    // If match == 100%; then exit with success
    if (MatchScore>=100) {
        QCoreApplication::processEvents();
        on_CloseDevice();
        QTimer::singleShot(100, []() { qApp->exit(0); });
    }

}

void MainWindow::on_OpenDevice()
{
    // Open Device method
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
    // Close Device method
   if (!CloseDevice())
   {
        ui->labelStatus->setText("Device Close Fail");
        sleep(1);
        exit(0);
   }
}

void MainWindow::on_cleanup()
{
    // Cleanup routine on close application
    on_CloseDevice();
    exit(0);
}

void MainWindow::on_NewImage()
{
    // Get new image method
    GetImageBmp(bmpData,&bmpSize);
    QPixmap * pm = new QPixmap();

    pm->loadFromData(bmpData,bmpSize,"bmp");
    QGraphicsScene * gs = new QGraphicsScene();
    gs->addPixmap(*pm);
    ui->graphicsView->setScene(gs);
}

void MainWindow::on_GetCapture()
{
    std::cout << ref1 << endl;
    std::cout << ref2 << endl;

    // Get matching pattern
    GetFpCharByGen(ref2File,&ref2Size);
    FILE* fp2;

    try
    {
        fp2 = fopen(ref2, "wb");
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
    }

    // Debug
    cout << "ref2" << endl;
    for (int i = 0; i < 256; ++i) {
        printf("%02X ", ref2File[i]);
    }
    printf("\n");


    // Get reference template
    GetFpCharByEnl(ref1File,&ref1Size);
    FILE* fp1;

    try
    {
        fp1 = fopen(ref1, "rb");
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
        fread(ref1File,512,1,fp1);
        fclose(fp1);
    }

    // Debug
    cout << "ref1" << endl;
    for (int i = 0; i < 512; ++i) {
        printf("%02X ", ref1File[i]);
    }
    printf("\n");

}

int MainWindow::on_Compare()
{
    // Return match score
    QString strResult;
    int MatchScore = 0;

    MatchScore = MatchTemplateOne(ref2File,ref1File,512);
    strResult.sprintf("Match Scope: %d% ",MatchScore);
    ui->labelStatus->setText(strResult);

    return MatchScore;
}

int MainWindow::getMatchScore() const {
    // Return match score to main()
    return MatchScore;
}
