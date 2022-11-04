//
// Created by JYSimilar on 2022/11/3.
//
// todo: 加注释

#include "../include/find_armor_factory.h"


FindArmorFactory ::FindArmorFactory(cv::Mat &frame, cv::Mat &binary) {
    m_armor.m_maxh=100;
    m_armor.m_t=-1;
    m_armor.selectLightbar(frame, binary, m_armors_possible);
    if(m_armors_possible.size()!=0) {
        m_armor.selectrightarmor(m_armors_possible, m_armors, binary);
    }

    if(!m_armors.size()) {
        m_isDetected = true;
    }
    m_armor.selectfinalarmor(m_finalarmor, m_armors, binary);
}

armors FindArmorFactory :: getFinalArmor(){
    return m_finalarmor;
}

std::vector<armors> FindArmorFactory :: getArmors(){
    return m_armors;
}
