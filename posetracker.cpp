#include "posetracker.h"

PoseTracker::PoseTracker(QObject *parent)
    : QObject{parent},align_to_color(RS2_STREAM_COLOR)
{
    shouldRun=false;

}


void PoseTracker::setshouldRun(bool x){
    this->shouldRun=x;
}

bool PoseTracker::init(const std::string graphPath){

    ptracker=QSharedPointer<mediapipe::Pose>(mediapipe::Pose::Create(graphPath));
    auto list = ctx.query_devices();
    if (list.size() == 0){
        throw std::runtime_error("No device detected. Is it plugged in?");
        return false;
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
    }
    return true;
}



bool PoseTracker::solve(){


    while(this->shouldRun){
        cv::Mat result;
        rs2::frameset fs0, fs1;
        fs0 = pipelines[0].wait_for_frames();
        fs1 = pipelines[1].wait_for_frames();
        result=ptracker->Process(fs0,fs1);
        if(!result.data){
            std::cout<<"[error] the output frame is empty"<<std::endl;
            return false;
        }

        //cv::cvtColor(result,result,cv::COLOR_BGR2RGB);
        //cv::resize(result,result,cv::Size(result.cols*0.8, result.rows*0.8), 0, 0,cv::INTER_NEAREST);

        //QImage disImage=QImage((const unsigned char*)(result.data),result.cols,result.rows,QImage::Format_RGB888);
        //std::cout<<"in solve is disImage null? "<<disImage.isNull()<<std::endl;
        //emit sendImage(disImage);
        cv::imshow("pose", result);
        if (!shouldRun || cv::waitKey(30) >= 0){
            cv::destroyWindow("Tag Detections");
            break;
        }
    }

    return true;
}


bool PoseTracker::release(){
    for(auto&& item : pipelines){
        item.stop();
    }
    pipelines.clear();
    serials.clear();
    return true;
}


