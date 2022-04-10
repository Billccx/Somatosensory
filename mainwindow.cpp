#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , updateTimer(new QTimer)
{
    ui->setupUi(this);
    this->updateTimer.setInterval(30);
    connect(&this->updateTimer,&QTimer::timeout,this,&MainWindow::updateImageLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    this->updateTimer.start();
    std::cout<<"timer start"<<std::endl;
    bool isInit=this->clb.init();
    if(!isInit){
        QMessageBox::warning(this, "Attention!","No Realsense device detected. Is it plugged in?");
        return;
    }

    this->clb.solve();
    QMessageBox::information(this, "Attention!","Camera extrinsics calibrated successfully!");
    this->clb.release();
    this->updateTimer.stop();
    std::cout<<"timer stop"<<std::endl;
    return;
}

void MainWindow::updateImageLabel(){
    cv::Mat img=this->clb.getMerge();
    if(img.empty()){
        std::cout<<"no pic"<<std::endl;
        return;
    }
    else{
        std::cout<<"pic width"<<img.cols<<std::endl;
    }
    //cv::cvtColor(img,img,cv::COLOR_BGR2RGB);
    QImage disImage=QImage((const unsigned char*)(img.data),img.cols,img.rows,QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(disImage));
    qApp->processEvents();
    return;
}

