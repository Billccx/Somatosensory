#include "calibrator.h"

Calibrator::Calibrator(QObject *parent)
    : QObject{parent},align_to_color(RS2_STREAM_COLOR)
{
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

}



bool Calibrator::init(){
    auto list = ctx.query_devices();
    if (list.size() == 0){
        return false;
    }
    std::cout << list.size() << std::endl;
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
    return true;
}
