/*
 * @Description: 
 * @Version: 1.0
 * @Author: Barimu
 * @Date: 2022-10-30 00:10:23
 * @LastEditors: Barimu
 * @LastEditTime: 2022-11-01 00:50:16
 */

#ifndef RM2022_SOLVEPNP_H
#define RM2022_SOLVEPNP_H

#include "opencv2/opencv.hpp"
#include "../include/armor_detector.h"
#include <queue>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

/**
 * @description: pnp类
 */
class SOLVEPNP
{
public:
   
    vector<Point2d> picture_points;           //相机坐标下的四个点
    
    float distance;

    float PNP(int flag);

    void caculate(armors finalarmor);

    float xishu;
   
private:
    vector<Point3d> model_points;

    // 相机内参矩阵
    Mat camera_matrix = (Mat_<double>(3, 3) << 1.220026691835674e+03, 0, 7.445508543026164e+02,
        0, 1.235231878687026e+03, 5.286821156891253e+02,
        0, 0, 1);
    
    //畸变系数
    Mat dist_coeffs = (Mat_<double>(5, 1) << -0.101891217424125, 0.115296033070786,
        -0.014708119282963, -0.003748512171184, -0.001974641177284);

    // 旋转向量
    Mat rotation_vector;

    // 平移向量
    Mat translation_vector;

    const double P = 3.1415926;

};
#endif