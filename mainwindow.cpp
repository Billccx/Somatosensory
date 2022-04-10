#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    bool isInit=this->clb.init();
    if(!isInit){
        QMessageBox::warning(this, "Attention!","No Realsense device detected. Is it plugged in?");
        return;
    }

    this->clb.solve();
    QMessageBox::information(this, "Attention!","Camera extrinsics calibrated successfully!");
    this->clb.release();
    return;
}

