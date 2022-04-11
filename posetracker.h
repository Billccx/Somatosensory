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
    bool shouldRun;


public:
    rs2::context ctx;
    std::vector<rs2::pipeline> pipelines;
    std::vector<std::string> serials;
    rs2::align align_to_color;

    explicit PoseTracker(QObject *parent = nullptr);
    bool init(const std::string graphPath);
    bool solve();
    bool release();
    void setshouldRun(bool x);


signals:

};

#endif // POSETRACKER_H
