#ifndef _ARMOR_TRACKER_H_
#define _ARMOR_TRACKER_H_

#include "armor_detector.h"
#include "Kalman.h"
#include "../include/kalmanfilter.h"
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

        angle::KalmanFilterMatrices kf_matrices_;
        std::unique_ptr<angle::KalmanFilter> kf_;
        Eigen::Vector3d tracking_velocity_;
    public:
        void track(armors &armor, bool isDetected,Mat &frame, Mat originFrame);
        bool isArmorSwitched(armors armor, Mat &frame, Mat originFrame);
};

namespace angle
{
    
class ArmorTracker
{
    private:
        angle::KalmanFilterMatrices kf_matrices_;
        std::unique_ptr<angle::KalmanFilter> kf_;
        Eigen::Vector3d tracking_velocity_;
        double max_match_distance_;
    public:
        ArmorTracker(const KalmanFilterMatrices & kf_matrices, double max_match_distance, int tracking_threshold,
  int lost_threshold);    
        void init(const armors &armor);
        void update(const armors &armor, const double & dt);
        Eigen::VectorXd target_state;
        int tracking_id;
        enum State 
        {
            LOST,
            DETECTING,
            TRACKING,
            TEMP_LOST,
        } tracker_state;
        int tracking_threshold_;
        int lost_threshold_;

        int detect_count_;
        int lost_count_;
};
}

#endif