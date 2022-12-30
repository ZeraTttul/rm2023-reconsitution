
// todo: 加注释

#ifndef RM2023_RECONSITUTION_FIND_ARMOR_FACTORY_H
#define RM2023_RECONSITUTION_FIND_ARMOR_FACTORY_H


#include "../include/armor_detector.h"
#include "../include/roi_finder.h"
#include <opencv2/opencv.hpp>


class FindArmorFactory {
private:
    armors m_finalarmor;
    ArmorDetector m_armor;
    std::vector<armors> m_armors_possible;
    std::vector<armors> m_armors;
    bool m_isDetected = false;
    RoiFinder roi;

public:
    void findArmorFactory(cv::Mat &frame, cv::Mat &originFrame);
    void findArmorFromROI(cv::Mat &frame, cv::Mat &originFrame);
    struct armors getFinalArmor();
    std::vector<armors> getArmors();

};


#endif //RM2023_RECONSITUTION_FIND_ARMOR_FACTORY_H
