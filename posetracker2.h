#ifndef POSETRACKER2_H
#define POSETRACKER2_H

#include <QThread>
#include <QObject>
#include <QSharedPointer>
#include <QImage>
#include <string>
#include <librealsense2/rs.hpp>
#include <vector>
#include "pose.h"


class PoseTracker2 : public QThread
{
    Q_OBJECT
private:
    QSharedPointer<mediapipe::Pose> ptracker;
    bool shouldRun;
public:
    explicit PoseTracker2(QObject *parent = nullptr);

    rs2::context ctx;
    std::vector<rs2::pipeline> pipelines;
    std::vector<std::string> serials;

    bool init(const std::string graphPath);
    bool solve();
    bool release();
    void setshouldRun(bool x);
};

#endif // POSETRACKER2_H
