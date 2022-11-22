/*
 * @Author: ZeraTul ZeraTttul@gmail.com
 * @Date: 2022-11-01 23:10:00
 * @LastEditors: ZeraTul ZeraTttul@gmail.com
 * @LastEditTime: 2022-11-02 00:44:13
 * @FilePath: \rm2022update\lib\armor_tracker.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _ARMOR_TRACTER_CPP_
#define _ARMOR_TRACTER_CPP

#include "../include/armor_tracker.h"
#include "../define/define.h"

void ArmorTracker :: track(armors &final_armor, bool isDetected, Mat frame, Mat originFrame)
{
	armors armor = final_armor;
	Point2f center;
    if(!isDetected)
    {
		if(!m_predict_que.empty())
		{
			center = m_predict_que.front().center;
			center = m_k.kal(center.x, center.y);
			armor.center = center;

                // cout << "not predirct" << endl;
                // cout << "center = " << center <<endl;

            if(m_predictCount < 4) {
                
                m_predict_que.push(armor);
                m_predictCount++;
#ifdef IMSHOW
                cv::circle(originFrame,
                    center,
                    3, cv::Scalar(255, 0, 0), -1);
#endif
                final_armor = armor;                                        //更新finalarmor传出 
            }
                
            m_predict_que.pop();
		}
    }
    else 
    {
		while(!m_predict_que.empty()) m_predict_que.pop();
        m_predictCount = 0;
		m_armor_que.push(armor);

		if(isArmorSwitched(armor, frame, originFrame))                                                           //装甲板中心点瞬移 x 个装甲板宽度后(暂定方案) 
		//fabs(armor.center.x - m_armor_que.front().center.x) > 2*armor.length
		{                                                               //认为是一块新的装甲板 init卡尔曼滤波器
			// m_k.reInit(m_k.m_KF);
			// cout << "new armor" <<endl;
			// while(!m_armor_que.empty()) m_armor_que.pop();
            imshow("roi", originFrame);
		}                                                          

		Point2f predict_pt = m_k.kal(armor.center.x, armor.center.y);
		armor.center = predict_pt;                                      //预测值
        // cout << "armor.center = " << armor.center <<endl;
		m_predict_que.push(armor);
	
        cout << "armor.center = " << predict_pt <<endl;
        circle(originFrame, predict_pt, 3, Scalar(34, 255, 255), -1);
        // cout << "done" << endl;
        // line(originFrame, Point(100,100), predict_pt, Scalar(255, 0, 0), 3);

		final_armor = armor;

    }
}

bool ArmorTracker:: isArmorSwitched(armors armor, Mat frame, Mat originFrame)
{
    Size2i roi_size = Size2i(m_roi_factor_w * (int)armor.boardw, m_roi_factor_h * (int)armor.boardh);
    // cout << size <<endl;
    Point2i roi_point = Point2i((m_roi_factor_w/2) * armor.boardw, (m_roi_factor_h/2) * armor.boardh);
    // cout << point <<endl;
    Rect ROIRect = Rect(armor.corner[1], armor.corner[3]) + roi_size - roi_point ;
#ifdef IMSHOW
    rectangle(originFrame, ROIRect, Scalar(255, 0, 0));
#endif
    Mat ROIImage(frame, ROIRect);
    imshow("roiframe", frame);

    return true;
}

#endif