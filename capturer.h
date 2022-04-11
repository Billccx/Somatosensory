#ifndef CAPTURER_H
#define CAPTURER_H

#include <QObject>
#include <librealsense2/rs.hpp>
#include <vector>
#include <iostream>
#include <string>

class Capturer : public QObject
{
    Q_OBJECT
public:
    rs2::context ctx;
    std::vector<rs2::pipeline> pipelines;
    //rs2::align align_to_color;
    explicit Capturer(QObject *parent = nullptr);

signals:

};

#endif // CAPTURER_H
