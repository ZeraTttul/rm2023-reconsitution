/*
 * @Description: 
 * @Version: 1.0
 * @Author: Barimu
 * @Date: 2022-12-05 20:45:49
 * @LastEditors: Barimu
 * @LastEditTime: 2022-12-18 14:15:58
 */
#include"../include/predict.h"

namespace angle
{
    void predict::initialize()
    {
        // Kalman Filter initial matrix
        // A - state transition matrix
        // clang-format off
        Eigen::Matrix<double, 6, 6> f;
        f <<  1,  0,  0, dt_, 0,  0,
                0,  1,  0,  0, dt_, 0,
                0,  0,  1,  0,  0, dt_,
                0,  0,  0,  1,  0,  0,
                0,  0,  0,  0,  1,  0,
                0,  0,  0,  0,  0,  1;
        // clang-format on

        // H - measurement matrix
        Eigen::Matrix<double, 3, 6> h;
        h.setIdentity();

        // Q - process noise covariance matrix
        Eigen::DiagonalMatrix<double, 6> q;
        q.diagonal() << 0.01, 0.01, 0.01, 0.1, 0.1, 0.1;

        // R - measurement noise covariance matrix
        Eigen::DiagonalMatrix<double, 3> r;
        r.diagonal() << 0.05, 0.05, 0.05;

        // P - error estimate covariance matrix
        Eigen::DiagonalMatrix<double, 6> p;
        p.setIdentity();

        kf_matrices_ = KalmanFilterMatrices{f, h, q, r, p};

        // Tracker
        double max_match_distance =  0.2;
        int tracking_threshold =  5;
        int lost_threshold =  5;
        last_time=0.0;
        dt_=0.0;
        tracker_ =  std::make_unique<ArmorTracker>(kf_matrices_, max_match_distance,
                     tracking_threshold, lost_threshold);
    }

    void predict::armortrack(armors armor)
    {

        if (tracker_->tracker_state == ArmorTracker::LOST) 
        {
            tracker_->init(armor);
            last_time=clock();
            target.tracking = false;
        } 
        else 
        {   
            // Set dt
            dt_ = (clock() - last_time)/CLOCKS_PER_SEC;
            last_time=clock();
            // Update state
            tracker_->update(armor, dt_);
            cout<<tracker_->tracker_state<<endl;
        }

        if (tracker_->tracker_state == ArmorTracker::DETECTING)
        {
            target.tracking = false;
        } 
        else if (
            tracker_->tracker_state == ArmorTracker::TRACKING ||
            tracker_->tracker_state == ArmorTracker::TEMP_LOST) 
        {
            target.tracking = true;
            target.id = tracker_->tracking_id;
        }

        if (target.tracking)
        {
            target.position[0] = tracker_->target_state(0);
            target.position[1] = tracker_->target_state(1);
            target.position[2] = tracker_->target_state(2);
            target.velocity[0] = tracker_->target_state(3);
            target.velocity[1] = tracker_->target_state(4);
            target.velocity[2] = tracker_->target_state(5);
        }
    }
}