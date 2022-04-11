#ifndef POSETRACKER_H
#define POSETRACKER_H

#include <QObject>

class PoseTracker : public QObject
{
    Q_OBJECT
public:
    explicit PoseTracker(QObject *parent = nullptr);

signals:

};

#endif // POSETRACKER_H
