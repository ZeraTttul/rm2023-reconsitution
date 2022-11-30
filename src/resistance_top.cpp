//
// Created by JYSimilar on 2022/11/21.
// Latest change by JYSimilar on 2022/11/30.
//

#include "../include/resistance_top.h"

resistanceTop :: resistanceTop() {
    m_curCenter = {-1, -1};
}

void resistanceTop :: resTop(cv::Point2f newCenter, int armorTimes, bool isChange) {
    m_isChange = isChange;
    m_curCenter = newCenter;
    // 判断是否切换装甲板
    if (!m_isChange) {
        // 没切换，就push装甲板中心坐标
        m_centerList.push_back(m_curCenter);
    } else {
        // 切换了，判断上一块装甲板有多少帧
        if (m_centerList.size() < armorTimes) {
            // 帧数小于这个传入的参数就认为是高速旋转状态
            // 认为装甲板旋转次数增加一次
            ++m_cnt;
            // 当达到三次的时候，认为进入陀螺状态
            if (m_cnt > 2) m_isTop = true;
        } else {
            // 超过该参数的帧数就重置下面两个参数
            m_cnt = 0;
            m_isTop = false;
        }
        // 清空上一块装甲板的中心坐标列表
        m_centerList.clear();
    }
}

/*
 * @brief: 是否是陀螺状态接口，可以查看正在关注的机器人是否处于陀螺状态
 */
bool resistanceTop :: isTopStatus() {
    if (m_isTop) return true;
    else return false;
}
