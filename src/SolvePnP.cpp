/*
 * @Description: 姿态解算
 * @Version: 1.0
 * @Author: Barimu
 * @Date: 2022-10-30 00:10:23
 * @LastEditors: Barimu
 * @LastEditTime: 2022-10-31 22:20:36
 */
#ifndef RM2022_SOLVEPNP_CPP
#define RM2022_SOLVEPNP_CPP

#include "../include/SolvePnP.h"

using namespace std;
using namespace cv;

/**
 * @description: 调用pnp
 * @param {armors} finalarmor
 */
void SOLVEPNP ::caculate(armors finalarmor) {

    //Serial uart(BR115200, WORDLENGTH_8B, STOPBITS_1, PARITY_NONE, HWCONTROL_NONE);
    //uart.sOpen("/dev/ttyTHS2");
        static float final_distance;
        float tmp;

        picture_points.push_back(finalarmor.corner[1]);
        picture_points.push_back(finalarmor.corner[2]);
        picture_points.push_back(finalarmor.corner[3]);
        picture_points.push_back(finalarmor.corner[4]);

        //判断是大装甲板还是小装甲板
        if (finalarmor.board_ratio < 4) {
            //小装甲板            
            xishu = (13.5 / finalarmor.boardw + 5.4 / finalarmor.boardh) / 2;
                //世界坐标
            tmp = PNP(0);
            if(tmp > 10) final_distance = tmp;

        } else {
            //大装甲板
            xishu = (23.5 / finalarmor.boardw + 5.4 / finalarmor.boardh) / 2;
            tmp = PNP(1);
            if(tmp > 10) final_distance = tmp;
        }
        double distance_to_midboard_x, distance_to_midboard_y;
        distance_to_midboard_x = xishu * (finalarmor.center.x - 320);
        distance_to_midboard_y = xishu * (finalarmor.center.y - 160);

        double angle_x = atan2(distance_to_midboard_x, final_distance);
        double angle_y = atan2(distance_to_midboard_y, final_distance);

        double final_angle_x = angle_x / P * 180;
        double final_angle_y = angle_y / P * 180;
        //cout << "final_distance  " << final_distance<<endl;
#ifdef NX
        if(tmp > 10)
            uart.sSendData(final_angle_x, final_angle_y,final_distance,1);

#endif
}

/**
 * @description: 进行pnp解算
 * @param {int} flag 通过flag的值判断大小装甲板
 */
float SOLVEPNP::PNP(int flag)
{
    //写入真实值
    if(flag==0)
    {
            model_points.push_back(Point3d(-66.75f, -24.25f, 0));
            model_points.push_back(Point3d(+66.75f, -24.25f, 0));
            model_points.push_back(Point3d(-66.75f, +24.25f, 0));
            model_points.push_back(Point3d(+66.75f, +24.25f, 0));
    }
    if(flag==1)
    {
            model_points.push_back(Point3d(-114.0f, -24.25f, 0));
            model_points.push_back(Point3d(+114.0f, -24.25f, 0));
            model_points.push_back(Point3d(-114.0f, +24.25f, 0));
            model_points.push_back(Point3d(+114.0f, +24.25f, 0));
    }
    solvePnP(model_points, picture_points, camera_matrix, dist_coeffs,
        rotation_vector, translation_vector, 0, SOLVEPNP_ITERATIVE);
    // 默认ITERATIVE方法，可尝试修改为EPNP（CV_EPNP）,P3P（CV_P3P）

    Mat Rvec;
    Mat_<float> Tvec;
    rotation_vector.convertTo(Rvec, CV_32F);  // 旋转向量转换格式
    translation_vector.convertTo(Tvec, CV_32F); // 平移向量转换格式 

    Mat_<float> rotMat(3, 3);
    Rodrigues(Rvec, rotMat);
    // 旋转向量转成旋转矩阵

    Mat P_oc;
    P_oc = -rotMat.inv() * Tvec;
    // 求解相机的世界坐标，得出p_oc的第三个元素即相机到物体的距离即深度信息，单位是mm

    //迭代器版
    // MatIterator_<float> it = P_oc.begin<float>();
    // MatIterator_<float> it_end = P_oc.end<float>();
    // for(int i = 1;it != it_end;it++,i++)
    // {
    //     if(i == 3)
    //     {
    //          distance = (float)(*it);
    //          break;
    //     }

    // }
    //at版
    distance = P_oc.at<float>(2,0);
    distance /= 10; 

    //cout << "P_oc " << endl << P_oc << endl;
    // cout << "distance " << abs(distance)<< endl;
    //cout<<flag<<endl;
    return abs(distance);
}


#endif
