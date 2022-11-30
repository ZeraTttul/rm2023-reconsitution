//
// Created by JYSimilar on 2022/11/21.
//

#ifndef RM2023_RECONSITUTION_RESISTANCE_TOP_H
#define RM2023_RECONSITUTION_RESISTANCE_TOP_H

#include <opencv2/opencv.hpp>


class resistanceTop {
private:
    std::deque<cv::Point2f> m_centerList;           // 存储中心点列表
    // cv::Point2f m_preCenter;                        // 上一个中心点，弃用
    cv::Point2f m_curCenter;                        // 当前中心点
    // bool m_direction = true;                        // 旋转方向，左到右为false，右到左为true，弃用
    bool m_isChange = false;                        // 是否切换了装甲板，默认没有切换
    bool m_isTop = false;                           // 陀螺状态，默认关闭
    int m_cnt = 0;                                  // 用于记录处在旋转状态的装甲板个数
public:
    resistanceTop();
    // 运行函数
    void resTop(cv::Point2f newCenter, int armorTimes, bool isChange);
    // 获取陀螺状态接口
    bool isTopStatus();
};


#endif //RM2023_RECONSITUTION_RESISTANCE_TOP_H
