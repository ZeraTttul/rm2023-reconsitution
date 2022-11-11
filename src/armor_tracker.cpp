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

void ArmorTracker :: track(armors &final_armor, bool isDetected, Mat binary)
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
			m_predict_que.push(armor);

                // cout << "not predirct" << endl;
                // cout << "center = " << center <<endl;
                
                cv::circle(binary,
                        center,
                        3, cv::Scalar(255, 0, 0), -1);
                // cv::Point2f* vertices1 = new cv::Point2f[4];
                // armor.l.points(vertices1);

                // for (int j = 0; j < 4; j++) 
                // {
                //     cv::line(binary,
                //             vertices1[j],
                //             vertices1[(j + 1) % 4],
                //             cv::Scalar(255, 0, 0),
                //             4);
                // }

                // cv::Point2f* vertices2 = new cv::Point2f[4];
                // armor.r.points(vertices2);

                // for (int j = 0; j < 4; j++)
                // {
                //     cv::line(binary,
                //             vertices2[j],
                //             vertices2[(j + 1) % 4],
                //             cv::Scalar(255, 0, 0),
                //             4);
                // }
		
			final_armor = armor;                                        //更新finalarmor传出 
			m_predict_que.pop();
		}
    }
    else 
    {
		while(!m_predict_que.empty()) m_predict_que.pop();
		m_armor_que.push(armor);

		if(fabs(armor.center.x - m_armor_que.front().center.x) > 2*armor.length)                                                           //装甲板中心点瞬移 x 个装甲板宽度后(暂定方案) 
		{                                                               //认为是一块新的装甲板 init卡尔曼滤波器
			m_k.reInit(m_k.m_KF);
			cout << "new armor" <<endl;
			while(!m_armor_que.empty()) m_armor_que.pop();
		}                                                          

		Point2f predict_pt = m_k.kal(armor.center.x, armor.center.y);
		armor.center = predict_pt;                                      //预测值
        // cout << "armor.center = " << armor.center <<endl;
		m_predict_que.push(armor);
	
        cout << "armor.center = " << predict_pt <<endl;
        circle(binary, predict_pt, 3, Scalar(34, 255, 255), -1);
        // cout << "done" << endl;
        // line(binary, Point(100,100), predict_pt, Scalar(255, 0, 0), 3);

		final_armor = armor;

    }
}

#endif