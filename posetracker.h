#ifndef POSETRACKER_H
#define POSETRACKER_H

#include <QObject>
#include <QSharedPointer>
#include <pose.h>
#include <string>
#include <librealsense2/rs.hpp>
#include <vector>

class PoseTracker : public QObject
{
    Q_OBJECT
private:
    QSharedPointer<mediapipe::Pose> ptracker;


public:
    rs2::context ctx;
    std::vector<rs2::pipeline> pipelines;
    std::vector<std::string> serials;
    rs2::align align_to_color;

    explicit PoseTracker(QObject *parent = nullptr);
    bool init(const std::string graphPath);
    bool solve(rs2::frameset fs0,rs2::frameset fs1);


signals:

};

#endif // POSETRACKER_H
