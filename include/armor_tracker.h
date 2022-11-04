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
        float target_change_threshold = 0;
    public:
        void track(armors &armor, bool isDetected, Mat binary);
};


#endif