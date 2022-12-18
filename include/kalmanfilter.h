/*
 * @Description: 
 * @Version: 1.0
 * @Author: Barimu
 * @Date: 2022-12-05 10:59:25
 * @LastEditors: Barimu
 * @LastEditTime: 2022-12-05 11:55:08
 */
/*
 * @Description: 
 * @Version: 1.0
 * @Author: Barimu
 * @Date: 2022-11-25 10:56:16
 * @LastEditors: Barimu
 * @LastEditTime: 2022-11-25 11:57:05
 */
#ifndef ARMOR_PROCESSOR__KALMANFILTER_H
#define ARMOR_PROCESSOR__KALMANFILTER_H

#include<Eigen/Dense>

namespace angle
{

struct KalmanFilterMatrices
{
    Eigen::MatrixXd F;  // state transition matrix
    Eigen::MatrixXd H;  // measurement matrix
    Eigen::MatrixXd Q;  // process noise covariance matrix
    Eigen::MatrixXd R;  // measurement noise covariance matrix
    Eigen::MatrixXd P;  // error estimate covariance matrix
};

class KalmanFilter
{
public:
    explicit KalmanFilter(const KalmanFilterMatrices & matrices);

    // Initialize the filter with a guess for initial states.
    void init(const Eigen::VectorXd & x0);

    // Computes a predicted state
    Eigen::MatrixXd predict(const Eigen::MatrixXd & F);

    // Update the estimated state based on measurement
    Eigen::MatrixXd update(const Eigen::VectorXd & z);

private:
    // Invariant matrices
    Eigen::MatrixXd F, H, Q, R;

    // Priori error estimate covariance matrix
    Eigen::MatrixXd P_pre;
    // Posteriori error estimate covariance matrix
    Eigen::MatrixXd P_post;

    // Kalman gain
    Eigen::MatrixXd K;

    // System dimensions
    int n;

    // N-size identity
     Eigen::MatrixXd I;

    // Predicted state
    Eigen::VectorXd x_pre;
    // Updated state
    Eigen::VectorXd x_post;
};

}
#endif