/*
 * @Description: 
 * @Version: 1.0
 * @Author: Barimu
 * @Date: 2022-10-30 00:10:23
 * @LastEditors: Barimu
 * @LastEditTime: 2022-12-05 11:55:11
 */

#ifndef RM2022_SOLVEPNP_H
#define RM2022_SOLVEPNP_H

#include "opencv2/opencv.hpp"
#include "../include/armor_detector.h"
#include <queue>
#include <vector>
#include <iostream>

#ifdef NX
    #include "serial.h"
#endif

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

    float PNP(armors &finalarmor,int flag);

    void caculate(armors &finalarmor);

    void calAngle(Mat cam,Mat dis,int x,int y);

    float xishu;

    double yaw;
    double pitch;
    double rxNew;
    double ryNew;
   
private:
    vector<Point3d> model_points;

    // 相机内参矩阵
    Mat camera_matrix = (Mat_<double>(3, 3) << 1.201371857055914e+03, 0, 7.494419594994199e+02,
        0, 1.201435954410725e+03, 5.508546827593877e+02,
        0, 0, 1);
    
    //畸变系数
    Mat dist_coeffs = (Mat_<double>(5, 1) << -0.098380553375716, 0.006115203108383,
        -4.766609631726518e-04, -0.001862163979558, 0);

    // 旋转向量
    Mat rotation_vector;

    // 平移向量
    Mat translation_vector;


    const double P = 3.1415926;

};
#endif