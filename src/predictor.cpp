/*
 * @Description: 
 * @Version: 1.0
 * @Author: Barimu
 * @Date: 2022-12-05 10:58:35
 * @LastEditors: Barimu
 * @LastEditTime: 2022-12-18 14:14:37
 */
#include"../include/armor_tracker.h"
#include<cfloat>
#include<iostream>

namespace angle
{

ArmorTracker::ArmorTracker(const KalmanFilterMatrices & kf_matrices, double max_match_distance, int tracking_threshold,
  int lost_threshold)
: tracker_state(LOST),
  tracking_id(0),
  kf_matrices_(kf_matrices),
  tracking_velocity_(Eigen::Vector3d::Zero()),
  max_match_distance_(max_match_distance),
  tracking_threshold_(tracking_threshold),
  lost_threshold_(lost_threshold),
  detect_count_(0),
  lost_count_(0)
{
}

void ArmorTracker::init(const armors &armor)
{
  // KF init
  kf_ = std::make_unique<KalmanFilter>(kf_matrices_);
  Eigen::VectorXd init_state(6);
  const auto position = armor.position;
  init_state << position[0], position[1], position[2], 0, 0, 0;
  kf_->init(init_state);
  tracking_id = armor.number;
  tracker_state = DETECTING;
}

void ArmorTracker::update(const armors &armor, const double & dt)
{
  // KF predict
  kf_matrices_.F(0, 3) = kf_matrices_.F(1, 4) = kf_matrices_.F(2, 5) = dt;
  Eigen::VectorXd kf_prediction = kf_->predict(kf_matrices_.F);

  bool matched = false;        //实际值与预测值是否匹配
  // Use KF prediction as default target state if no matched armor is found
  target_state = kf_prediction;

    armors matched_armor;
    double min_position_diff = DBL_MAX;
    Eigen::Vector3d position_vec(armor.position[0], armor.position[1], armor.position[2]);
    Eigen::Vector3d predicted_position = kf_prediction.head(3);
    // Difference of the current armor position and tracked armor's predicted position
    double position_diff = (predicted_position - position_vec).norm();
    if (position_diff < min_position_diff) 
    {
        min_position_diff = position_diff;
        matched_armor = armor;
    }

    if (min_position_diff < max_match_distance_)
    {
      // Matching armor found
      matched = true;
      Eigen::Vector3d position_vec(
      matched_armor.position[0], matched_armor.position[1], matched_armor.position[2]);
      target_state = kf_->update(position_vec);
    } else 
    {
      if(tracking_id==armor.number)
      {
          matched = true;
          // Reset KF
          kf_ = std::make_unique<KalmanFilter>(kf_matrices_);
          Eigen::VectorXd init_state(6);
          // Set init state with current armor position and tracking velocity before
          init_state << armor.position[0], armor.position[1], armor.position[2], tracking_velocity_;
          kf_->init(init_state);
          target_state = init_state;
      }
    }
    //cout<<matched<<endl;
  // Save tracking target velocity
  tracking_velocity_ = target_state.tail(3);
  // Tracking state machine
  if (tracker_state == DETECTING) {
    // DETECTING
    //cout<<detect_count_<<endl;
    if (matched) {
      detect_count_++;
      if (detect_count_ > tracking_threshold_) {
        detect_count_ = 0;
        tracker_state = TRACKING;
      }
    } else {
      detect_count_ = 0;
      tracker_state = LOST;
    }

  } else if (tracker_state == TRACKING) {
    // TRACKING
    if (!matched) {
      tracker_state = TEMP_LOST;
      lost_count_++;
    }

  } else if (tracker_state == TEMP_LOST) {
    // LOST
    if (!matched) {
      lost_count_++;
      if (lost_count_ > lost_threshold_) {
        lost_count_ = 0;
        tracker_state = LOST;
      }
    } else {
      tracker_state = TRACKING;
      lost_count_ = 0;
    }
  }
}
}