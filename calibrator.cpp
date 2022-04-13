#include "calibrator.h"

Calibrator::Calibrator(QObject *parent)
    : QObject{parent},align_to_color(RS2_STREAM_COLOR)
{
    isFirstInit=true;
}


bool Calibrator::init(){
    getopt_t *getopt = getopt_create();
    getopt_add_bool(getopt, 'h', "help", 0, "Show this help");
    getopt_add_bool(getopt, 'd', "debug", 0, "Enable debugging output (slow)");
    getopt_add_bool(getopt, 'q', "quiet", 0, "Reduce output");
    getopt_add_string(getopt, 'f', "family", "tag36h11", "Tag family to use");
    getopt_add_int(getopt, 't', "threads", "1", "Use this many CPU threads");
    getopt_add_double(getopt, 'x', "decimate", "2.0", "Decimate input image by this factor");
    getopt_add_double(getopt, 'b', "blur", "0.0", "Apply low-pass blur to input");
    getopt_add_bool(getopt, '0', "refine-edges", 1, "Spend more time trying to align edges of tags");

    tf = tag36h11_create();

    td0 = apriltag_detector_create();
    apriltag_detector_add_family(td0, tf);
    td0->quad_decimate = getopt_get_double(getopt, "decimate");
    td0->quad_sigma = getopt_get_double(getopt, "blur");
    td0->nthreads = getopt_get_int(getopt, "threads");
    td0->debug = getopt_get_bool(getopt, "debug");
    td0->refine_edges = getopt_get_bool(getopt, "refine-edges");

    td1 = apriltag_detector_create();
    apriltag_detector_add_family(td1, tf);
    td1->quad_decimate = getopt_get_double(getopt, "decimate");
    td1->quad_sigma = getopt_get_double(getopt, "blur");
    td1->nthreads = getopt_get_int(getopt, "threads");
    td1->debug = getopt_get_bool(getopt, "debug");
    td1->refine_edges = getopt_get_bool(getopt, "refine-edges");



    auto list = ctx.query_devices();
    if (list.size() == 0){
        return false;
    }
    std::cout << list.size() << std::endl;
    serials.clear();
    for (auto &&item : list){
        std::string serial = item.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);
        std::cout << serial << std::endl;
        serials.push_back(serial);
    }

    int cnt=0;
    for (auto &&serial : serials){
        rs2::pipeline pipe(ctx);
        rs2::config cfg;
        cfg.enable_device(serial);
        if(cnt) cfg.enable_stream(rs2_stream::RS2_STREAM_DEPTH, 640, 480, rs2_format::RS2_FORMAT_Z16);
        cfg.enable_stream(rs2_stream::RS2_STREAM_COLOR, 640, 480, rs2_format::RS2_FORMAT_RGB8);
        pipe.start(cfg);
        pipelines.emplace_back(pipe);
        cnt++;
    }

    for(auto&& pipeline : pipelines){
        rs2::video_frame rgb = pipeline.wait_for_frames().get_color_frame();
        rs2::stream_profile profile =  rgb.get_profile();
        rs2::video_stream_profile profile_(profile);
        rs2_intrinsics intrin =  profile_.get_intrinsics();
        intrins.emplace_back(intrin);
    }

    return true;
}


cv::Mat Calibrator::frame_to_mat(const rs2::frame& f){
    using namespace cv;
    using namespace rs2;

    auto vf = f.as<video_frame>();
    const int w = vf.get_width();
    const int h = vf.get_height();

    if (f.get_profile().format() == RS2_FORMAT_BGR8){
        return Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_RGB8){
        auto r_rgb = Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
        Mat r_bgr;
        cvtColor(r_rgb, r_bgr, COLOR_RGB2BGR);
        return r_bgr;
    }
    else if (f.get_profile().format() == RS2_FORMAT_Z16){
        return Mat(Size(w, h), CV_16UC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_Y8){
        return Mat(Size(w, h), CV_8UC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_DISPARITY32){
        return Mat(Size(w, h), CV_32FC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }

    throw std::runtime_error("Frame format is not supported yet!");
}


bool Calibrator::solve(){
    cv::Mat gray0,gray1;
    double cm0[9]= {600.764,         0,   331.948,
                          0,   600.986,   248.697,
                          0,         0,         1 };
    cv::Mat cameraMatrix0 (3, 3, CV_64F, cm0);


    bool isFind=false;
    std::vector<cv::Mat> tq;
    while (!isFind) {

        rs2::frameset fs0, fs1;
        fs0 = pipelines[0].wait_for_frames();
        fs1 = pipelines[1].wait_for_frames();
        //std::cout<<"wait frames"<<std::endl;

        //rs2::frameset aligned0 = align_to_color.process(fs0);
        rs2::frameset aligned1 = align_to_color.process(fs1);


        rs2::video_frame color0=fs0.get_color_frame();
        rs2::video_frame color1=aligned1.get_color_frame();
        rs2::depth_frame depth1=aligned1.get_depth_frame();

        // rs2::video_frame color0=fs0.get_color_frame();
        // rs2::video_frame color1=fs1.get_color_frame();
        cv::Mat frame0=frame_to_mat(color0);
        cv::Mat frame1=frame_to_mat(color1);

        cvtColor(frame0, gray0, cv::COLOR_BGR2GRAY);
        cvtColor(frame1, gray1, cv::COLOR_BGR2GRAY);

        // Make an image_u8_t header for the Mat data
        image_u8_t im0 = {
            .width = gray0.cols,
            .height = gray0.rows,
            .stride = gray0.cols,
            .buf = gray0.data
        };

        image_u8_t im1 = {
            .width = gray1.cols,
            .height = gray1.rows,
            .stride = gray1.cols,
            .buf = gray1.data
        };

        //std::cout<<"aligned"<<std::endl;

        zarray_t *detections0 = apriltag_detector_detect(td0, &im0);
        zarray_t *detections1 = apriltag_detector_detect(td1, &im1);

        //std::cout<<"detected"<<std::endl;

        std::vector<cv::Point3f> objectPoints;
        std::vector<cv::Point2f> imagePoints;

        cv::Mat r, t, R;

        // Draw detection outlines
        for (int i = 0; i < zarray_size(detections0); i++) {
            apriltag_detection_t *det;
            zarray_get(detections0, i, &det);
            line(frame0, cv::Point(det->p[0][0], det->p[0][1]),cv::Point(det->p[1][0], det->p[1][1]),cv::Scalar(0, 0xff, 0), 2);
            line(frame0, cv::Point(det->p[0][0], det->p[0][1]),cv::Point(det->p[3][0], det->p[3][1]),cv::Scalar(0, 0, 0xff), 2);
            line(frame0, cv::Point(det->p[1][0], det->p[1][1]),cv::Point(det->p[2][0], det->p[2][1]),cv::Scalar(0xff, 0, 0), 2);
            line(frame0, cv::Point(det->p[2][0], det->p[2][1]),cv::Point(det->p[3][0], det->p[3][1]),cv::Scalar(0xff, 0, 0), 2);

            imagePoints.push_back(cv::Point2f(det->p[0][0], det->p[0][1]));
            imagePoints.push_back(cv::Point2f(det->p[1][0], det->p[1][1]));
            imagePoints.push_back(cv::Point2f(det->p[2][0], det->p[2][1]));
            imagePoints.push_back(cv::Point2f(det->p[3][0], det->p[3][1]));

            std::stringstream ss;
            ss << det->id;
            cv::String text = ss.str();
            int fontface = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
            double fontscale = 1.0;
            int baseline;
            cv::Size textsize = getTextSize(text, fontface, fontscale, 2,&baseline);
            putText(frame0, text,
            cv::Point(det->c[0]-textsize.width/2,det->c[1]+textsize.height/2),
            fontface, fontscale, cv::Scalar(0xff, 0x99, 0), 2);
        }
        apriltag_detections_destroy(detections0);



        for (int i = 0; i < zarray_size(detections1); i++) {
            apriltag_detection_t *det;
            zarray_get(detections1, i, &det);
            line(frame1, cv::Point(det->p[0][0], det->p[0][1]),cv::Point(det->p[1][0], det->p[1][1]),cv::Scalar(0, 0xff, 0), 2);
            line(frame1, cv::Point(det->p[0][0], det->p[0][1]),cv::Point(det->p[3][0], det->p[3][1]),cv::Scalar(0, 0, 0xff), 2);
            line(frame1, cv::Point(det->p[1][0], det->p[1][1]),cv::Point(det->p[2][0], det->p[2][1]),cv::Scalar(0xff, 0, 0), 2);
            line(frame1, cv::Point(det->p[2][0], det->p[2][1]),cv::Point(det->p[3][0], det->p[3][1]),cv::Scalar(0xff, 0, 0), 2);


            for(int j=0;j<4;j++){
                float point3d[3]={0,0,0};
                float point2d[2]={(float)det->p[j][0],(float)det->p[j][1]};
                //cout<<"pixel:("<<(int)det->p[i][0]<<','<<(int)det->p[i][1]<<")\n";
                float depth=depth1.get_distance((int)det->p[j][0],(int)det->p[j][1]);

                if(depth==0){
                    objectPoints.clear();
                    break;
                }
                //cout<<"depth: "<<depth<<endl;
                rs2_deproject_pixel_to_point(point3d,&intrins[1],point2d,depth);
                //cout<<"("<<point3d[0]<<','<<point3d[1]<<','<<point3d[2]<<")\n";
                objectPoints.push_back(cv::Point3f(point3d[0],point3d[1],point3d[2]));
            }

            std::stringstream ss;
            ss << det->id;
            cv::String text = ss.str();
            int fontface = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
            double fontscale = 1.0;
            int baseline;
            cv::Size textsize = getTextSize(text, fontface, fontscale, 2,&baseline);
            putText(
                    frame1,
                    text,
                    cv::Point(det->c[0]-textsize.width/2,det->c[1]+textsize.height/2),
                    fontface,
                    fontscale,
                    cv::Scalar(0xff, 0x99, 0),
                    2
                    );
        }
        apriltag_detections_destroy(detections1);


        if(objectPoints.size()==4 && imagePoints.size()==4){
            solvePnP(
                objectPoints,
                imagePoints,
                cameraMatrix0,
                cv::Mat(),
                r,
                t,
                false,
                cv::SOLVEPNP_P3P
            );
            std::cout<<"3d:"<<std::endl;
            for(int i=0;i<4;i++){
                std::cout<<objectPoints[i]<<',';
            }
            std::cout<<std::endl;
            std::cout<<"2d:"<<std::endl;
            for(int i=0;i<4;i++){
                std::cout<<imagePoints[i]<<',';
            }
            std::cout<<std::endl;
            std::cout<<"r:\n"<<r<<std::endl;
            Rodrigues(r, R);
            std::cout<<"rotation:\n"<<R<<std::endl;
            std::cout<<"trans:\n"<<t<<std::endl;
            //cout<<endl<<endl<<endl;

            //vector<double> t_={t.at<double>(0,0),t.at<double>(0,1),t.at<double>(0,2)};

            tq.push_back(t);
            //cout<<"tq size:"<<tq.size()<<endl;
            if(tq.size()>3){
                tq.erase(tq.begin());
            }
            if(tq.size()==3){
                double d01=norm(tq[0]-tq[1]);
                double d02=norm(tq[0]-tq[2]);
                double d12=norm(tq[1]-tq[2]);
                std::cout<<"d01:"<<d01<<" d02:"<<d02<<" d12:"<<d12<<std::endl;
                if(d01<0.03 && d02<0.03 && d12<0.03){
                    std::cout<<"finished:"<<std::endl;
                    std::cout<<"rotation:\n"<<R<<std::endl;
                    std::cout<<"trans:\n"<<t<<std::endl;
                    std::ofstream outfile("/home/cuichenxi/code/Qt/qmakeProj/extrinsics/RT.txt", std::ios::trunc);
                    // outfile<<"rotation:\n"<<R<<endl;
                    // outfile<<"trans:\n"<<t<<endl;
                    for(int i=0;i<3;i++){
                        for(int j=0;j<3;j++){
                            outfile<<R.at<double>(i,j)<<' ';
                        }
                        outfile<<std::endl;
                    }

                    for(int i=0;i<3;i++){
                        outfile<<t.at<double>(i,0)<<' ';
                    }
                    outfile<<std::endl;


                    outfile.close();
                    isFind=true;
                }
            }
            std::cout<<std::endl<<std::endl<<std::endl;
        }

        //cv::Mat merge;
        hconcat(frame0,frame1,this->merge);
        std::cout<<"captured frame"<<std::endl;

        /*
        cv::cvtColor(merge,merge,cv::COLOR_BGR2RGB);
        cv::resize(merge,merge,cv::Size(merge.cols*0.8, merge.rows*0.8), 0, 0,cv::INTER_NEAREST);
        QImage disImage=QImage((const unsigned char*)(merge.data),merge.cols,merge.rows,QImage::Format_RGB888);
        std::cout<<"in solve is disImage null? "<<disImage.isNull()<<std::endl;
        emit sendImage(disImage);
        */

        imshow("Tag Detections", merge);
        if (isFind || cv::waitKey(30) >= 0){
            cv::destroyWindow("Tag Detections");
            break;
        }

        if(isFind) break;
    }
    return true;
}



bool Calibrator::release(){
    apriltag_detector_destroy(td0);
    apriltag_detector_destroy(td1);
    for(auto&& item : pipelines){
        item.stop();
    }
    pipelines.clear();
    serials.clear();
    return true;
}


cv::Mat Calibrator::getMerge(){
    return this->merge;
}


