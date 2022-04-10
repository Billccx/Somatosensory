#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <QObject>
#include <librealsense2/rs.hpp>
#include <iostream>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <apriltag/apriltag_pose.h>


class Calibrator : public QObject
{
    Q_OBJECT
public:
    explicit Calibrator(QObject *parent = nullptr);

signals:

public slots:


};

#endif // CALIBRATOR_H
