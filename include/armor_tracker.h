#ifndef _ARMOR_TRACKER_H_
#define _ARMOR_TRACKER_H_

#include "armor_detector.h"
#include "Kalman.h"

class ArmorTracker
{
    private:
        queue<armors> m_armor_que;
        queue<armors> m_predict_que;
        kalman m_k;
        float m_roi_factor_w = 3; //roi系数
        float m_roi_factor_h = 4;
        int m_predictCount = 0;
    public:
        void track(armors &armor, bool isDetected,Mat frame, Mat originFrame);
        bool isArmorSwitched(armors armor, Mat frame, Mat originFrame);
};


#endif