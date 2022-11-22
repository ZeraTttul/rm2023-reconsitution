//
// Created by JYSimilar on 2022/11/21.
//

#include "../include/resistance_top.h"

resistanceTop :: resistanceTop() {
    m_curCenter = {-1, -1};
}

void resistanceTop :: resTop(cv::Point2f newCenter, int armorTimes, bool isChange) {
    m_isChange = isChange;
    m_curCenter = newCenter;
    if (!m_isChange) {
        m_centerList.push_back(m_curCenter);
    } else {
        if (m_centerList.size() < armorTimes) {
            m_isTop = true;
        } else {
            m_isTop = false;
        }
        m_centerList.clear();
    }
}

bool resistanceTop :: isTopStatus() {
    if (m_isTop) return true;
    else return false;
}
