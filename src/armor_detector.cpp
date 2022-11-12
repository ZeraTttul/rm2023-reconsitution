//
// Created by JYSimilar on 2022/11/3.
//

#include "../include/armor_detector.h"

/*
 * @Description: 检测装甲板并筛选出有效装甲板
 * @Version: 1.0
 * @Author: Barimu
 * @Date: 2022-10-31 22:08:46
 * @LastEditors: Barimu
 * @LastEditTime: 2022-11-01 00:51:39
 */
#ifndef ARMOR_DECTOR_CPP
#define ARMOR_DECTOR_CPP


#include "../include/armor_detector.h"
#include "../include/rgb.h"


/**
 * @brief 筛选灯条并组合 然后筛选出可能的装甲板
 * @param Mat frame
 * @param Mat binary
 * @param vector &armors_possible 有可能是装甲板的灯条组合
 */
void ArmorDetector :: selectLightbar(cv::Mat frame, cv::Mat binary, std::vector<armors>&armors_possible){
    Rgb rgb;

// #ifdef DETECT_BLUE
//     frame = rgb.imagePreprocess_rgb(frame, BLUE);//flag=1识别红色 else 识别蓝色
// #endif
// #ifdef DETECT_RED
//     frame = rgb.imagePreprocess_rgb(frame, RED);//flag=1识别红色 else 识别蓝色
// #endif

// #ifdef IMSHOW
//     imshow("double", frame);
// #endif

    // rgb.imagePreprocess_gray(frame);
    std::vector<std::vector<cv::Point> > contours;               //画出轮廓
    std::vector<std::vector<cv::Point> > select_contours;        //筛选出的正确的灯条轮廓
    findContours(frame,
                 contours,
                 cv::RETR_EXTERNAL,                              //只检测外围轮廓
                 cv::CHAIN_APPROX_NONE);


    m_hi=-1;                                          //可能的装甲板的数量 视觉祖传的hi 没有特殊原因不准修改！

    for (size_t i = 0; i < contours.size(); i++) {


        float light_contour_area = contourArea(contours[i]);

        if (light_contour_area < 80||contours.size()<=5 ){
            continue;
        }

        drawContours(frame,
                     contours,
                     static_cast<int>(i),
                     cv::Scalar(0),
                     2);

        cv::RotatedRect rec = minAreaRect(contours[i]);                                       //最小外接矩阵拟合
        float angle = rec.size.width > rec.size.height ? rec.angle - 90 : rec.angle;                //通过矩形角度筛出一些非灯条轮廓
        if(angle>20||angle<-20) continue;

        float ratio=MAX(rec.size.width, rec.size.height) / MIN(rec.size.width, rec.size.height);
        if(ratio>15||ratio<1) continue;

        select_contours.push_back(contours[i]);                                                     //存入
    }

    std::vector<std::vector<bool>> is_armor(select_contours.size(), std::vector<bool>(select_contours.size(), true));
    for (int i = 0; i < select_contours.size(); i++)                                                //灯条两两匹配模拟装甲板
    {
        drawContours(frame, select_contours, static_cast<int>(i), cv::Scalar(0), 2);
        cv::RotatedRect rect = minAreaRect(select_contours[i]);

        if (i == select_contours.size() - 1) {
            continue;
        }

        
#ifdef DETECT_BLUE
        if(!rgb.isRightColor(frame, rect, BLUE)) continue;//flag=1识别红色 else 识别蓝色
#endif
#ifdef DETECT_RED
        if(!rgb.isRightColor(frame, rect, BLUE)) continue;//flag=1识别红色 else 识别蓝色
#endif

        for (int j = i + 1; j < select_contours.size(); j++)
        {
            cv::RotatedRect rectA = minAreaRect(select_contours[j]);

            float r_angle, rA_angle, diff;
            r_angle=rect.size.width > rect.size.height ? rect.angle - 90 : rect.angle;
            rA_angle=rectA.size.width > rectA.size.height ? rectA.angle - 90 : rectA.angle;

            diff = abs(r_angle - rA_angle);                                                        //角度差
            if (diff >10 && diff < 70)
            {
                is_armor[i][j]==false;
                continue;
            }
            float Diff=abs(rect.center.y-rectA.center.y);                                          //y方向上的差
            if(Diff>30)
            {
                is_armor[i][j]==false;
                continue;
            }
            armors temp;
            temp.angle_diff=diff;
            temp.center=(rect.center+rectA.center)/2;
            if(rect.center.x<rectA.center.x)
            {
                temp.l = rect;
                temp.r = rectA;
            }
            else
            {
                temp.l = rectA;
                temp.r = rect;
            }
            cv::Point2f verticesR[4];
            rect.points(verticesR);
            cv::Point2f verticesRA[4];
            rectA.points(verticesRA);

            if (abs(rect.angle) > 45)
            {
                temp.corner[1] = (verticesR[0] + verticesR[1]) / 2;
                temp.corner[4] = (verticesR[3] + verticesR[2]) / 2;
            } else
            {
                temp.corner[1] = (verticesR[1] + verticesR[2]) / 2;
                temp.corner[4] = (verticesR[0] + verticesR[3]) / 2;
            }

            if (abs(rectA.angle) > 45)
            {
                temp.corner[2] = (verticesRA[1] + verticesRA[0]) / 2;
                temp.corner[3] = (verticesRA[2] + verticesRA[3]) / 2;
            }
            else
            {
                temp.corner[2] = (verticesRA[1] + verticesRA[2]) / 2;
                temp.corner[3] = (verticesRA[0] + verticesRA[3]) / 2;
            }
            if(is_armor[i][j])
            {
                m_hi++;
                armors_possible.push_back(temp);
            }
            if(is_armor[i][j])
            {
#ifdef IMSHOW
                cv::Point2f* vertices1 = new cv::Point2f[4];
                rect.points(vertices1);

                for (int j = 0; j < 4; j++)
                {
                    cv::line(binary,
                            vertices1[j],
                            vertices1[(j + 1) % 4],
                            cv::Scalar(0, 255, 0),
                            4);
                }
                
                cv::Point2f* vertices2 = new cv::Point2f[4];
                rectA.points(vertices2);

                for (int j = 0; j < 4; j++)
                {
                    cv::line(binary,
                            vertices2[j],
                            vertices2[(j + 1) % 4],
                            cv::Scalar(0, 255, 0),
                            4);
                }
#endif
            }
        }
    }
}


/**
 * @brief 筛选出正确的装甲板
 * @param {Mat} binary
 * @param {vector} &armors_possible
 * @param {vector} &armors 正确的装甲板
 */
void ArmorDetector::selectrightarmor(std::vector<armors> &armors_possible, std::vector<armors> &armors, cv::Mat binary)
{
    std::vector<bool> is_armor(armors_possible.size(), true);
    int kkk=-1;
    for(int mark=0;mark<armors_possible.size();mark++)
    {
        float x1=armors_possible[mark].corner[1].x;
        float x2=armors_possible[mark].corner[2].x;
        float x3=armors_possible[mark].corner[3].x;
        float x4=armors_possible[mark].corner[4].x;

        float y1=armors_possible[mark].corner[1].y;
        float y2=armors_possible[mark].corner[2].y;
        float y3=armors_possible[mark].corner[3].y;
        float y4=armors_possible[mark].corner[4].y;

#ifdef IMSHOW
        vector<cv::Point2f> imagePoints;
        imagePoints.push_back(Point2f(x1, y1));
        imagePoints.push_back(Point2f(x2, y2));
        imagePoints.push_back(Point2f(x4, y4));
        imagePoints.push_back(Point2f(x3, y3));

        for (int n = 0; n < imagePoints.size(); n++) {
            circle(binary, imagePoints[n], 3, Scalar(255, 0, 0), -1, 8);
        }
#endif

        cv::Point2f left_light = cv::Point2f(x1-x4, y1-y4);
        cv::Point2f right_light = cv::Point2f(x2-x3, y2-y3);
        cv::Point2f center_diff = armors_possible[mark].l.center - armors_possible[mark].r.center;          //中心点坐标的差

        float left_length = sqrt(left_light.x * left_light.x + left_light.y * left_light.y);                //左灯条长度
        float right_length = sqrt(right_light.x * right_light.x + right_light.y * right_light.y);           //右灯条长度
        float center_diff_length = sqrt(center_diff.x * center_diff.x + center_diff.y * center_diff.y);     //两中心点距离
        float rectangle_likely = (left_light.x * center_diff.x + left_light.y * center_diff.y) / (left_length * center_diff_length); //长方形契合度
        float length_ratio = MAX(left_length, right_length) / MIN(left_length, right_length);
        rectangle_likely += (right_light.x * center_diff.x + right_light.y * center_diff.y) / (right_length * center_diff_length);

        if (length_ratio > 2 || fabs(rectangle_likely) > 0.3){
            is_armor[mark]=false;
        }

        float height = fmax(armors_possible[mark].l.size.width, armors_possible[mark].l.size.height);        //高
        cv::Rect re= cv::Rect(armors_possible[mark].l.center.x,armors_possible[mark].l.center.y - height / 2,
                      armors_possible[mark].r.center.x - armors_possible[mark].l.center.x, height);
        int armor_area = re.size().area();                                                                   //装甲板面积

        if (armor_area < 100 || armor_area > 80000){
            is_armor[mark]=false;
        }

        float boardw_up = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
        float boardw_down = sqrt((x3 - x4) * (x3 - x4) + (y3 - y4) * (y3 - y4));                             //装甲板长
        armors_possible[mark].boardw = (boardw_up + boardw_down) / 2;

        float boardh_left = sqrt((x1 - x4) * (x1 - x4) + (y1 - y4) * (y1 - y4));
        float boardh_right = sqrt((x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3));
        armors_possible[mark].boardh = (boardh_left + boardh_right) / 2;                                     //高

        float board_width = abs(armors_possible[mark].l.center.x - armors_possible[mark].r.center.x);
        float board_height = (cv::max(armors_possible[mark].l.size.width,
                                  armors_possible[mark].l.size.height) + cv::max(armors_possible[mark].r.size.width,
                                                                             armors_possible[mark].r.size.height)) / 2;
        armors_possible[mark].board_ratio = armors_possible[mark].boardw/armors_possible[mark].boardh;        //长宽比

        if(armors_possible[mark].board_ratio>2.5){
            is_armor[mark]=false;
        }

        if(is_armor[mark]==true)
        {
            armors.push_back(armors_possible[mark]);
        }
    }
}


/**
 * @description: 选出有效的装甲板
 * @param {armors} &final_armor 有效的装甲板
 * @param {vector} armors
 * @param {Mat} binary
 */
void ArmorDetector::selectfinalarmor(armors &final_armor, std::vector<armors> &armors, cv::Mat binary)
{
    m_maxh=100;
    m_t=-1;
    for(int i=0;i<armors.size();i++)                                 //找角度差最小的
    {
        if(armors[i].angle_diff < m_maxh)
        {
            m_maxh=armors[i].angle_diff;
            m_t=i;
        }
    }
    if(m_t == -1)return;
    final_armor = armors[m_t];

#ifdef IMSHOW
    cv::circle(binary,final_armor.center,15, cv::Scalar(0, 0, 255), 4);
#endif
}


#endif
