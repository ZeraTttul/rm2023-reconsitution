#include "opencv2/opencv.hpp"
#include "../include/rgb.h"

using namespace std;
using namespace cv;

//图片准备
Mat Rgb::imagePreprocess_rgb(const cv::Mat &src, bool enemyColor) {

    _src = src;

    _splitSrc.clear();

    cv::split(_src, _splitSrc);

    if (enemyColor == RED) {
        //识别红色

        //削减亮度
        Mat BrightnessLut(1, 256, CV_8UC1);

        for (int i = 0; i < 256; i++) {
            //you can change "_para.imageBright_RED" to adjust bright level(in the file rgb.h)
            BrightnessLut.at<uchar>(i) = saturate_cast<uchar>((double)i + _para.imageBright_RED);
        }

        LUT(_src, BrightnessLut, _src);

        GaussianBlur( _src,
                      _src,
                      Size(11, 11),
                      0,
                      0 );

        //分离色彩通道
        cv::cvtColor(_src, _graySrc, cv::COLOR_BGR2GRAY);

        //灰度二值化
        cv::threshold(_graySrc,
                      _graySrc,
                      _para.grayThreshold_RED,
                      255,
                      cv::THRESH_BINARY);


        //红蓝通道相减
        cv::subtract(_splitSrc[2],
                     _splitSrc[0],
                    _separationSrc);

        //红蓝二值化
        cv::threshold(_separationSrc,
                      _separationSrc,
                      _para.separationThreshold_RED,
                      255,
                      cv::THRESH_BINARY);

        //红绿通道相减
        cv::subtract(_splitSrc[2],
                     _splitSrc[1],
                     _separationSrcGreen);

        cv::erode(_separationSrcGreen,
                  _separationSrcGreen,
                  Rgb:: structuringElement2());

        cv::dilate(_separationSrcGreen,
                   _separationSrcGreen,
                   Rgb:: structuringElement3());

        // imshow("1",_separationSrc);

        //逻辑与获得最终二值化图像
        _maxColor = _separationSrc & _graySrc & _separationSrcGreen;

        cv::dilate(_maxColor, _maxColor,Rgb::structuringElement3());

    } else if(enemyColor == BLUE)
    {
        //识别蓝色

        //削减亮度
        Mat BrightnessLut(1, 256, CV_8UC1);

        for (int i = 0; i < 256; i++) {
            //you can change "_para.imageBright_BLUE" to adjust bright level(in the file rgb.h)
            BrightnessLut.at<uchar>(i) = saturate_cast<uchar>((double)i + _para.imageBright_BLUE);
        }

        LUT(_src, BrightnessLut, _src);

        //高斯滤波
        GaussianBlur( _src,
                      _src,
                      Size(11, 11),
                      0,
                      0 );

        //分离色彩通道
        cv::cvtColor(_src, _graySrc, cv::COLOR_BGR2GRAY);

        //高斯滤波
        // GaussianBlur( _graySrc,
        //               _graySrc,
        //               Size(11, 11),
        //               0,
        //               0 );
        

        //直方图均质化 能把灰度图像几乎变回没灰度的时候
        // Mat _equalseparaSrc;
        // _equalseparaSrc = EqualHist(_graySrc);

        //灰度二值化
        cv::threshold(_graySrc,
                      _graySrc,
                      _para.grayThreshold_BLUE,
                      255,
                      cv::THRESH_BINARY);
                      
        // imshow("gray", _graySrc);

        //红蓝通道相减
        cv::subtract(_splitSrc[0],
                     _splitSrc[2],
                     _separationSrc);

        //Mat _grayseparaSrc;
        // equalizeHist(_grayseparaSrc, _equalseparaSrc);
        //imshow("_separationSrc", _separationSrc);

        //红蓝二值化
        cv::threshold(_separationSrc,
                      _separationSrc,
                      _para.separationThreshold_BLUE,
                      255,
                      cv::THRESH_BINARY);

        //imshow("separationsrc",_separationSrc);
        // imshow("chanel", _separationSrc);

        cv::subtract(_splitSrc[0], _splitSrc[1], _separationSrcGreen);
        cv::erode(_separationSrcGreen, _separationSrcGreen, Rgb::structuringElement3());
        cv::dilate(_separationSrcGreen, _separationSrcGreen, Rgb::structuringElement5());

        //逻辑与获得最终二值化图像
        _maxColor = _separationSrc & _graySrc & _separationSrcGreen;

        //膨胀
        cv::dilate(_maxColor, _maxColor, Rgb::structuringElement5());

    }
    return _maxColor;
}

Mat Rgb:: EqualHist(Mat image) //直方图均衡化
{
    Mat equalImg;
    Mat grayImage;
    equalizeHist(image, equalImg);
    imshow("equalImg", equalImg);
    return equalImg;
}

void Rgb:: imagePreprocess_gray(Mat &frame)
{
    //削减亮度
    Mat BrightnessLut(1, 256, CV_8UC1);

    for (int i = 0; i < 256; i++) {
        //you can change "_para.imageBright_BLUE" to adjust bright level(in the file rgb.h)
        BrightnessLut.at<uchar>(i) = saturate_cast<uchar>((double)i + _para.imageBright_GRAY);
    }

    LUT(frame, BrightnessLut, frame);

    Mat gray;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    threshold(gray, gray, 100, 255, THRESH_BINARY);
    // erode(gray, gray, StructuringElement35); //去除噪点，考虑不要腐蚀(在黑暗的环境不需要，白天环境可能需要)
    dilate(gray, gray, StructuringElement35);
    gray.copyTo(frame);

    // imshow("gray", gray);
}

bool Rgb:: isRightColor(const cv::Mat &frame, const RotatedRect &rrect, bool enemyColor)
{
    cv::Rect rect = rrect.boundingRect();
    rect.x = rect.x < 0 ? 0 : (rect.x > frame.cols ? frame.cols : rect.x);
    rect.y = rect.y < 0 ? 0 : (rect.y > frame.rows ? frame.rows : rect.y);
    rect.width = rect.x + rect.width > frame.cols ? frame.cols - rect.x : rect.width;
    rect.height = rect.y + rect.height > frame.rows ? frame.rows - rect.y : rect.height;
    cv::Scalar roi_color_bgr_mean = mean(frame(rect));
    if (enemyColor == BLUE)
    {
        // std::cout << "roi_color: " << roi_color_bgr_mean[0] - roi_color_bgr_mean[1] << " " << roi_color_bgr_mean[0] - roi_color_bgr_mean[2] << std::endl;

        return roi_color_bgr_mean[0] > roi_color_bgr_mean[2] &&
               roi_color_bgr_mean[0] > roi_color_bgr_mean[1];
    }
    else
    {
        // std::cout << "roi_color: " << roi_color_bgr_mean[2] - roi_color_bgr_mean[1] << " " << roi_color_bgr_mean[2] - roi_color_bgr_mean[0] << std::endl;
        return roi_color_bgr_mean[2] > roi_color_bgr_mean[0] && roi_color_bgr_mean[2] > roi_color_bgr_mean[1];
    }
}