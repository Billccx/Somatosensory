#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QImage>
#include <QTimer>
#include <calibrator.h>

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

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Calibrator clb;
    cv::Mat img;
    QTimer updateTimer;

};
#endif // MAINWINDOW_H
