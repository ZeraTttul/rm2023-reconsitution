#ifndef _ROI_FINDER_H_
#define _ROI_FINDER_H_

#include "../include/define/define.h"
#include "armor_detector.h"

using namespace cv;
using namespace std;

class RoiFinder
{
    private:
        cv::Rect originRect;
        const float m_roi_factor_w = 3; //roi系数
        const float m_roi_factor_h = 4;
        std::queue<cv::Rect> m_roi_que;
        Mat m_originFrame;
        void getOrigin();
    
    public:
        void findRoi(armors armor, Mat originFrame);
        Rect getRoi();
        void init(Mat originFrame);
};



#endif