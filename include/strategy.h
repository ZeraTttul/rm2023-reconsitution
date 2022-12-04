

#ifndef RM2023_RECONSITUTION_STRATEGY_H
#define RM2023_RECONSITUTION_STRATEGY_H


#include <opencv2/opencv.hpp>
#include "../include/define/define.h"


class Strategy {
protected:
    cv::KalmanFilter m_KF;

public:
    Strategy();
    virtual void algorithmInterface();

};


#endif //RM2023_RECONSITUTION_STRATEGY_H
