/*
 * @Description: 
 * @Version: 1.0
 * @Author: Barimu
 * @Date: 2022-12-05 12:34:59
 * @LastEditors: Barimu
 * @LastEditTime: 2022-12-05 22:19:44
 */
#ifndef PREDICT_H
#define PREDICT_H

#include<string>
#include<vector>
#include"../include/kalmanfilter.h"
#include"../include/armor_tracker.h"
#include"../include/armor_tracker.h"

namespace angle
{
    class target
    {
        public:
            bool tracking;
            double position[3];
            double velocity[3];
            int id;
    };
    class predict
    {
        public:
            void initialize();
            void armortrack(armors armor);
            void angletransfer();
            target target;
        private:

             // Initial KF matrices
            KalmanFilterMatrices kf_matrices_;

            // Armor tracker
            std::unique_ptr<ArmorTracker> tracker_;

            //TODO:add angle
            double last_time;
            double dt_;
    };
}
#endif