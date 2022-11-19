//
// Change by JYSimilar on 2022/9/25
//
// 该文件已整理完成

#include "opencv2/opencv.hpp"
#include <queue>
#include <vector>
#include "../define/define.h"

using namespace std;
using namespace cv;


class Rgb {
private:

    cv::Mat _src;
    cv::Mat _maxColor;
    cv::Mat _graySrc;
    cv::Mat _separationSrcGreen;
    cv::Mat _separationSrc;        //色彩分离后的图像

    std::vector<cv::Mat> _splitSrc;
        
    struct BuffPara {
        int grayThreshold_RED = 40;                            //灰度二值化阈值-红色
        int grayThreshold_BLUE = 50;                            //灰度二值化阈值-蓝色
        int separationThreshold_RED = 50;                    //色彩分离二值化阈值-红色
        int separationThreshold_BLUE = 90;                    //色彩分离二值化阈值-蓝色
        float imageBright_RED = -80;                         //亮度削减-红色
        float imageBright_BLUE = -50;                         //亮度削减-蓝色
        float imageBright_GRAY = -60;                         //亮度削减-灰度二值化
    };

    BuffPara _para;

    const cv::Mat StructuringElement2 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));
    const cv::Mat StructuringElement3 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    const cv::Mat StructuringElement5 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    const cv::Mat StructuringElement7 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));
    const cv::Mat StructuringElement9 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9));
    const cv::Mat StructuringElement35 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 5));

    static Rgb &instance() {
        static Rgb util;
        return util;
    }

    static inline cv::Mat structuringElement2() {
        return instance().StructuringElement2;
    }

    static inline cv::Mat structuringElement3() {
        return instance().StructuringElement3;
    }

    static inline cv::Mat structuringElement5() {
        return instance().StructuringElement5;
    }

    static inline cv::Mat structuringElement7() {
        return instance().StructuringElement7;
    }

    static inline cv::Mat structuringElement9() {
        return instance().StructuringElement9;
    }
    static inline cv::Mat structuringElement35() {
        return instance().StructuringElement35;
    }

public:

    /**
     * 对图片进行二值化处理
     * @param src 输入图片
     * @param ownColor 己方颜色
     */

    Mat imagePreprocess_rgb(const cv::Mat &src, bool flag);
    Mat EqualHist(Mat image);
    void imagePreprocess_gray(Mat &src);
    bool isRightColor(const cv::Mat &frame, const RotatedRect &rrect, bool enemyColor);
};