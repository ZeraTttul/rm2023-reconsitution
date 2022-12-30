//
// Created by JYSimilar on 2022/11/3.
//

#ifndef RM2023_RECONSITUTION_VIDEO_CAP_H
#define RM2023_RECONSITUTION_VIDEO_CAP_H

#include "../include/define/define.h"
#include <string>
#include <opencv2/opencv.hpp>
// #include "../hikvision/include/hikvision_camera.h"


class VideoCap{
private:
    // const std::string m_filePath = "../testVideo/blueVideo5.mp4";       // 用来存储测试视频路，需要调用摄像头的时候删除后面的路径即可
    const std::string m_filePath = "E:\\VSCode\\production\\rgb\\Video\\blueVideo3.mp4";
    const int m_capIndex = CAP_INDEX;                                   // 当检测到 是摄像头 时，会使用这个值来表示是哪个摄像头
    cv::VideoCapture m_cap;                                             // 存储视频资源
    cv::Mat m_frame;                                                    // 用于存储当前帧图像
    //camera :: HikCamera MVS_Cap;

public:
    VideoCap();
    // VideoCap(int mode);
    void runCap();
    // void runHikCap();

    // 获取当前图像接口
    cv::Mat getCurrentImage();

};

#endif //RM2023_RECONSITUTION_VIDEO_CAP_H
