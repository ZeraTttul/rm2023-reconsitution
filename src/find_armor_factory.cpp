//
// Created by JYSimilar on 2022/11/3.
//
// todo: 加注释

#include "../include/find_armor_factory.h"


void FindArmorFactory :: findArmorFactory(cv::Mat &frame, cv::Mat &originFrame) {
    m_armor.m_maxh=100;
    m_armor.m_t=-1;
    m_armors_possible.clear();
    m_armors.clear();
    m_armor.selectLightbar(frame, originFrame, m_armors_possible);
    if(m_armors_possible.size()!=0) {
        m_armor.selectrightarmor(m_armors_possible, m_armors, originFrame);
    }

    if(!m_armors.size()) {
        m_isDetected = true;
    }
    m_armor.selectfinalarmor(m_finalarmor, m_armors, originFrame);
}

void FindArmorFactory :: findArmorFromROI(cv::Mat &frame, cv::Mat &originFrame)
{
    roi.init(originFrame);
    Rect roiRect = roi.getRoi();
    cout<<"get rect = " << roiRect <<endl;
    Mat roiFrame = frame(roiRect);
    m_armors_possible.clear();
    m_armors.clear();
    imshow("roiframe", roiFrame);
    m_armor.selectLightbar(roiFrame, originFrame, m_armors_possible);
    if(m_armors_possible.size()!=0) {
        m_armor.selectrightarmor(m_armors_possible, m_armors, originFrame);
    }

    m_isDetected = false;
    if(!m_armors.empty()) {
        m_isDetected = true;
        m_armor.selectfinalarmor(m_finalarmor, m_armors, originFrame);
    }
    roi.findRoi(m_finalarmor,m_isDetected);
}

// 返回最终确定的最优装甲板
armors FindArmorFactory :: getFinalArmor(){
    return m_finalarmor;
}

// 返回所有可能的装甲板
std::vector<armors> FindArmorFactory :: getArmors(){
    return m_armors;
}
