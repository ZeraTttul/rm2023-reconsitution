//
// Created by JYSimilar on 2022/11/3.
//
// todo: 加注释

#include "../include/find_armor_factory.h"


FindArmorFactory ::FindArmorFactory(cv::Mat &frame, cv::Mat &originFrame) {
    m_armor.m_maxh=100;
    m_armor.m_t=-1;
    m_armor.selectLightbar(frame, originFrame, m_armors_possible);
    if(m_armors_possible.size()!=0) {
        m_armor.selectrightarmor(m_armors_possible, m_armors, originFrame);
    }

    if(!m_armors.size()) {
        m_isDetected = true;
    }
    m_armor.selectfinalarmor(m_finalarmor, m_armors, originFrame);
}

armors FindArmorFactory :: getFinalArmor(){
    return m_finalarmor;
}

std::vector<armors> FindArmorFactory :: getArmors(){
    return m_armors;
}
