#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QImage>
#include <QTimer>
#include <cstdlib>
#include <QThread>
#include "calibrator.h"
#include "posetracker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateImageLabel();
    void updateImageLabel2(QImage disImage);
    void updateImageLabel3(QImage disImage); //show 2d landmarks

signals:
    void sendStartcmd();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    Calibrator clb;
    cv::Mat img;
    QTimer updateTimer;
    QThread pstThread;
    PoseTracker* pst;

};
#endif // MAINWINDOW_H
