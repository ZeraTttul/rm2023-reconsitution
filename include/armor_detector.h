#ifndef RM2023_RECONSITUTION_ARMOR_DETECTOR_H
#define RM2023_RECONSITUTION_ARMOR_DETECTOR_H


#include <vector>
#include <opencv2/opencv.hpp>
#include "../include/define/define.h"


/**
 * @description: 装甲板结构体
 */
struct armors {
public:
    cv::RotatedRect l;                   //左灯条
    cv::RotatedRect r;                   //右灯条
    float length;                    //装甲板长
    float height;                    //装甲板高
    float boardw;
    float boardh;
    float angle_diff;                //左右灯条角度差
    cv::Point2f corner[5];               //装甲板四个角点   [1][3]为对角
    float board_ratio;               //装甲板长宽比
    cv::Point2f center;                  //装甲板中心点
    double position[3];               //现实坐标x,y,z
    int number;                  //装甲板上数字
};


/**
 * @description: 检测装甲板类
 */
class ArmorDetector {
public:
    int m_hi;                           //视觉组的传承 ~by xyc
    int m_maxh;
    int m_t;
    void selectLightbar(cv::Mat &frame, cv::Mat originFrame, std::vector<armors> &armors_possible);
    void selectrightarmor(std::vector<armors> &armors_possible, std::vector<armors>&armors, cv::Mat originFrame);
    void selectfinalarmor(armors &finalarmor, std::vector<armors> &armors, cv::Mat originFrame);
};


#endif //RM2023_RECONSITUTION_ARMOR_DETECTOR_H
