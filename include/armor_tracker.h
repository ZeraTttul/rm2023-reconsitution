#ifndef _ARMOR_TRACKER_H_
#define _ARMOR_TRACKER_H_

#include "armor_detector.h"
#include "Kalman.h"
#include "roi_finder.h"

class ArmorTracker
{
    private:
        queue<armors> m_armor_que;
        queue<armors> m_predict_que;
        kalman m_k;
        int m_predictCount = 0;
        bool m_isArmorChanged = false;
        RoiFinder roi;
    public:
        void track(armors &armor, bool isDetected,Mat &frame, Mat originFrame);
        bool isArmorSwitched(armors armor, Mat &frame, Mat originFrame);
};


#endif