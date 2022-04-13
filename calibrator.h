#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <QObject>
#include <QTimer>
#include <QMessageBox>
#include <librealsense2/rs.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <apriltag/apriltag_pose.h>

extern "C" {
#include "apriltag.h"
#include "tag36h11.h"
#include "tag25h9.h"
#include "tag16h5.h"
#include "tagCircle21h7.h"
#include "tagCircle49h12.h"
#include "tagCustom48h12.h"
#include "tagStandard41h12.h"
#include "tagStandard52h13.h"
#include "common/getopt.h"
}


class Calibrator : public QObject
{
    Q_OBJECT
private:
    rs2::context ctx;
    rs2::align align_to_color;
    std::vector<rs2::pipeline> pipelines;
    std::vector<std::string> serials;
    std::vector<rs2_intrinsics> intrins;
    apriltag_family_t *tf;
    apriltag_detector_t *td0,*td1;
    bool isFirstInit;
    cv::Mat merge;


public:
    static cv::Mat frame_to_mat(const rs2::frame& f);
    explicit Calibrator(QObject *parent = nullptr);
    bool init();
    bool solve();
    bool release();
    cv::Mat getMerge();

signals:
    //void sendImage(QImage img);

public slots:


};

#endif // CALIBRATOR_H
