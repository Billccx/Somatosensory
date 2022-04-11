#include <Python.h>
#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    cv::Mat t=cv::Mat();
    if(!t.data){
        std::cout<<"mat is empty"<<std::endl;
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}



















































































































/*
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "opencv2/opencv.hpp"
#include "apriltag/apriltag_pose.h"
#include <librealsense2/rs.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
//#include <example.h>
#include <pose.h>


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

using namespace std;
using namespace cv;

static cv::Mat frame_to_mat(const rs2::frame& f);
void printIntrinsics(rs2_intrinsics intrin);

int main(int argc, char *argv[])
{

    mediapipe::Pose* mp=mediapipe::Pose::Create("/home/cuichenxi/mediapipe/mediapipe/examples/desktop/mypose9/mypose_tracking9.pbtxt");
    rs2::align align_to_color(RS2_STREAM_COLOR);
    rs2::context ctx;
    std::vector<rs2::pipeline> pipelines;
    std::map<std::string, rs2::colorizer> colorizers;
    std::vector<std::string> serials; //相机id

    auto list = ctx.query_devices(); // Get a snapshot of currently connected devices
    if (list.size() == 0){
        throw std::runtime_error("No device detected. Is it plugged in?");
    }
    else{
        std::cout << list.size() << std::endl;
    }

    for (auto &&item : list){
        std::string serial = item.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);
        std::cout << serial << std::endl;
        serials.push_back(serial);
    }

    for (auto &&serial : serials){
        rs2::pipeline pipe(ctx);
        rs2::config cfg;
        cfg.enable_device(serial);
        cfg.enable_stream(rs2_stream::RS2_STREAM_DEPTH, 640, 480, rs2_format::RS2_FORMAT_Z16);
        cfg.enable_stream(rs2_stream::RS2_STREAM_COLOR, 640, 480, rs2_format::RS2_FORMAT_RGB8);
        pipe.start(cfg);
        pipelines.emplace_back(pipe);
        // Map from each device's serial number to a different colorizer
        colorizers[serial] = rs2::colorizer();
    }

    int cnt = 0;
    int validcnt = 0;
    std::cout << "ready to fetch" << std::endl;

    while (cnt < 10000){
        cnt++;
        rs2::frameset fs0, fs1;
        fs0 = pipelines[0].wait_for_frames();
        fs1 = pipelines[1].wait_for_frames();

        // Send image packet into the graph.
        absl::Status run_status=mp->Process(fs0,fs1);
    }
}


void printIntrinsics(rs2_intrinsics intrin){
    std::cout<<"\ncolor intrinsics: ";
    std::cout<<"(width,height) "<<intrin.width<<"  "<<intrin.height<<"  ";
    std::cout<<"(ppx,ppy) "<<intrin.ppx<<"  "<<intrin.ppy<<"  ";
    std::cout<<"(fx,fy) "<<intrin.fx<<"  "<<intrin.fy<<std::endl;
    std::cout<<"coeffs: ";
    for(auto value : intrin.coeffs) std::cout<<value<<"  ";
    std::cout<<std::endl;
    std::cout<<"distortion model: "<<(int)intrin.model<<std::endl;
}
*/


