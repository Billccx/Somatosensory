#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , updateTimer(new QTimer)
{
    pst=new PoseTracker();
    pst->moveToThread(&pstThread);
    pstThread.start();
    ui->setupUi(this);
    this->updateTimer.setInterval(30);
    //connect(&this->updateTimer,&QTimer::timeout,this,&MainWindow::updateImageLabel);
    //connect(&this->clb,&Calibrator::sendImage,this,&MainWindow::updateImageLabel2);
    //connect(&this->pst,&PoseTracker::sendImage,this,&MainWindow::updateImageLabel3);
    connect(this,&MainWindow::sendStartcmd,pst,&PoseTracker::solve);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    //this->updateTimer.start();
    //std::cout<<"timer start"<<std::endl;
    bool isInit=this->clb.init();
    if(!isInit){
        QMessageBox::warning(this, "Attention!","No Realsense device detected. Is it plugged in?");
        return;
    }

    this->clb.solve();
    QMessageBox::information(this, "Attention!","Camera extrinsics calibrated successfully!");
    this->clb.release();
    //this->updateTimer.stop();
    //std::cout<<"timer stop"<<std::endl;
    //ui->label->clear();
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
    //QImage disImage2=disImage.scaled(1024,384,Qt::KeepAspectRatio);
    ui->label->setPixmap(QPixmap::fromImage(disImage));
    qApp->processEvents();
    return;
}

void MainWindow::updateImageLabel2(QImage disImage){
    //cv::cvtColor(img,img,cv::COLOR_BGR2RGB);
    std::cout<<"is Image null? "<<disImage.isNull()<<std::endl;
    ui->label->setPixmap(QPixmap::fromImage(disImage));
    qApp->processEvents();
    return;
}


//start tracking button
void MainWindow::on_pushButton_2_clicked()
{
    const std::string graphPath="/home/cuichenxi/code/Qt/qmakeProj/calculatorgraph/1.pbtxt";
    bool isInit=this->pst->init(graphPath);
    if(!isInit){
        QMessageBox::warning(this, "Attention!","No Realsense device detected. Is it plugged in?");
        return;
    }
    this->pst->setshouldRun(true);
    //this->pst->solve();
    emit sendStartcmd();
}


//stop tracking button
void MainWindow::on_pushButton_3_clicked()
{
    this->pst->setshouldRun(false);
    this->pst->release();
}


void MainWindow::updateImageLabel3(QImage disImage){
    //cv::cvtColor(img,img,cv::COLOR_BGR2RGB);
    std::cout<<"is Image null? "<<disImage.isNull()<<std::endl;
    ui->label->setPixmap(QPixmap::fromImage(disImage));
    qApp->processEvents();
    return;
}


void MainWindow::on_pushButton_4_clicked()
{
    //system("cd ~/mediapipe");
    //system("cd ~/mediapipe; GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/mypose9/mypose9_cpu --calculator_graph_config_file=mediapipe/examples/desktop/mypose9/mypose_tracking9.pbtxt");
    system("cd /home/cuichenxi/mediapipe/mediapipe/examples/desktop/visiualize; nohup python server.py &");
}


void MainWindow::on_pushButton_5_clicked()
{
    system("pkill -f server.py");
}

